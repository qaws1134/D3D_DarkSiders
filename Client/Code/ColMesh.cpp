#include "stdafx.h"
#include "ColMesh.h"

#include "Export_Function.h"

CColMesh::CColMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CColMesh::CColMesh(const CColMesh& rhs)
	: CGameObject(rhs)
{

}

CColMesh::~CColMesh(void)
{

}

HRESULT CColMesh::Ready_Object( )
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

void CColMesh::Late_Ready_Object()
{
}

_int CColMesh::Update_Object(const _float& fTimeDelta)
{
	USES_CONVERSION;
	_int iExit = CGameObject::Update_Object(fTimeDelta);

	if (m_wstrBone != L"")
	{
		CDynamicMesh*	pTargetMeshCom = dynamic_cast<CDynamicMesh*>(m_pTarget->Get_Component(L"Com_Mesh", ID_STATIC));
		NULL_CHECK_RETURN(pTargetMeshCom, 0);

		const D3DXFRAME_DERIVED*		pFrame = pTargetMeshCom->Get_FrameByName(W2A(m_wstrBone.c_str()));
		m_pParentBoneMatrix = &pFrame->CombinedTransformMatrix;

		CTransform*	pTargetTransformCom = dynamic_cast<CTransform*>(m_pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC));
		NULL_CHECK_RETURN(pTargetTransformCom, 0);
		m_pParentWorldMatrix = pTargetTransformCom->Get_WorldMatrix() ;


	}

	if ( m_wstrBone != L"")
	{
		m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));
	}

	_vec3 vPos;
	m_pTransformCom->Get_INFO(INFO_POS, &vPos);
	m_pColliderCom->Set_Center(vPos);
	Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CColMesh::Render_Object(void)
{	
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pColliderCom->Render_Collider(COLTYPE(m_bCol), m_pTransformCom->Get_WorldMatrix());

}


CColMesh * CColMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _float fRadius,wstring pBone)
{
	CColMesh*	pInstance = new CColMesh(pGraphicDev);
	pInstance->SetPos(vPos);
	pInstance->SetBone(pBone);
	pInstance->SetRadius(fRadius);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

CColMesh * CColMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, COLLIDERSPHERE tColSphere, wstring pBone)
{
	CColMesh*	pInstance = new CColMesh(pGraphicDev);

	pInstance->SetBone(pBone);
	pInstance->SetColSphere(tColSphere);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}


void CColMesh::Free(void)
{
	CGameObject::Free();
}




HRESULT CColMesh::Add_Component()
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