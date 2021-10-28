#include "stdafx.h"
#include "StaticMeshObj.h"

#include "Export_Function.h"

CStaticMeshObj::CStaticMeshObj(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CStaticMeshObj::CStaticMeshObj(const CStaticMeshObj& rhs)
	: CGameObject(rhs)
{

}

CStaticMeshObj::~CStaticMeshObj(void)
{

}

HRESULT CStaticMeshObj::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	//m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	return S_OK;
}

void CStaticMeshObj::Late_Ready_Object()
{
}

_int CStaticMeshObj::Update_Object(const _float& fTimeDelta)
{

	_int iExit = CGameObject::Update_Object(fTimeDelta);


	Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CStaticMeshObj::Render_Object(void)
{	
	_matrix m_matCamera;

	m_pGraphicDev->GetTransform(D3DTS_VIEW,&m_matCamera);


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	m_pColliderCom->Render_Collider(COLTYPE(m_bCol), m_pTransformCom->Get_WorldMatrix());
	m_pMeshCom->Render_Meshes();

	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	
}

CStaticMeshObj* CStaticMeshObj::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStaticMeshObj*	pInstance = new CStaticMeshObj(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

CStaticMeshObj * CStaticMeshObj::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh)
{
	CStaticMeshObj*	pInstance = new CStaticMeshObj(pGraphicDev);

	pInstance->SetProtoMesh(ProtoMesh);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CStaticMeshObj::Free(void)
{
	CGameObject::Free();
}

HRESULT CStaticMeshObj::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<CStaticMesh*>(Clone_Prototype(m_wstrProtoMesh.c_str()));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Transform", pComponent);

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
	pComponent = m_pColliderCom = CColliderSphere::Create(m_pGraphicDev, m_pMeshCom->Get_VtxPos(), m_pMeshCom->Get_VtxCnt());
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Collider", pComponent);


	return S_OK;
}