#include "stdafx.h"
#include "ColSphereMesh.h"

#include "Export_Function.h"

CColSphereMesh::CColSphereMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CColSphereMesh::CColSphereMesh(const CColSphereMesh& rhs)
	: CGameObject(rhs)
{

}

CColSphereMesh::~CColSphereMesh(void)
{

}

HRESULT CColSphereMesh::Ready_Object( )
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);



	return S_OK;
}

void CColSphereMesh::Late_Ready_Object()
{
}

_int CColSphereMesh::Update_Object(const _float& fTimeDelta)
{
	USES_CONVERSION;

	if (m_wstrBone != L"")
	{
		CDynamicMesh*	pPlayerMeshCom = dynamic_cast<CDynamicMesh*>(m_pTarget->Get_Component(L"Com_Mesh", ID_STATIC));
		NULL_CHECK_RETURN(pPlayerMeshCom, 0);

		const D3DXFRAME_DERIVED*		pFrame = pPlayerMeshCom->Get_FrameByName(W2A(m_wstrBone.c_str()));
		m_pParentBoneMatrix = &pFrame->CombinedTransformMatrix;

		CTransform*	pPlayerTransformCom = dynamic_cast<CTransform*>(m_pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC));
		NULL_CHECK_RETURN(pPlayerTransformCom, 0);
		m_pParentWorldMatrix = pPlayerTransformCom->Get_WorldMatrix() ;


	}
	_int iExit = CGameObject::Update_Object(fTimeDelta);

	if ( m_wstrBone != L"")
	{
		m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));
	}
	
	Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CColSphereMesh::Render_Object(void)
{	
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pColliderCom->Render_Collider(COLTYPE(m_bCol), m_pTransformCom->Get_WorldMatrix());

}


CColSphereMesh * CColSphereMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _float fRadius,wstring pBone)
{
	CColSphereMesh*	pInstance = new CColSphereMesh(pGraphicDev);
	pInstance->SetPos(vPos);
	pInstance->SetBone(pBone);
	pInstance->SetRadius(fRadius);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

CColSphereMesh * CColSphereMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, COLLIDERSPHERE tColSphere, wstring pBone)
{
	CColSphereMesh*	pInstance = new CColSphereMesh(pGraphicDev);

	pInstance->SetBone(pBone);
	pInstance->SetColSphere(tColSphere);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}


void CColSphereMesh::Free(void)
{
	CGameObject::Free();
}




HRESULT CColSphereMesh::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Pos(&m_vPos);
	m_pTransformCom->Update_Component(0.f);

	// Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(m_pRendererCom, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// Collider
	pComponent = m_pColliderCom = CColliderSphere::Create(m_pGraphicDev, &m_vPos, m_fRadius);
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Collider", pComponent);

	return S_OK;
}