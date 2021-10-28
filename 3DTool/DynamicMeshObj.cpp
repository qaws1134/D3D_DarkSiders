#include "stdafx.h"
#include "DynamicMeshObj.h"

#include "Export_Function.h"

CDynamicMeshObj::CDynamicMeshObj(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CDynamicMeshObj::CDynamicMeshObj(const CDynamicMeshObj& rhs)
	: CGameObject(rhs)
{

}

CDynamicMeshObj::~CDynamicMeshObj(void)
{

}

HRESULT CDynamicMeshObj::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	//m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	m_pTransformCom->Update_Component(0.f);
	m_pMeshCom->Set_AnimationIndex(0);



	return S_OK;
}

void CDynamicMeshObj::Late_Ready_Object()
{
}

_int CDynamicMeshObj::Update_Object(const _float& fTimeDelta)
{

	_int iExit = CGameObject::Update_Object(fTimeDelta);

	m_pMeshCom->Play_Animation(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CDynamicMeshObj::Render_Object(void)
{	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pColliderCom->Render_Collider(COLTYPE(m_bCol), m_pTransformCom->Get_WorldMatrix());
	m_pMeshCom->Render_Meshes();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CDynamicMeshObj* CDynamicMeshObj::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDynamicMeshObj*	pInstance = new CDynamicMeshObj(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

CDynamicMeshObj * CDynamicMeshObj::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh)
{
	CDynamicMeshObj*	pInstance = new CDynamicMeshObj(pGraphicDev);

	pInstance->SetProtoMesh(ProtoMesh);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CDynamicMeshObj::Free(void)
{
	CGameObject::Free();
}

HRESULT CDynamicMeshObj::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Prototype(m_wstrProtoMesh.c_str()));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

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
	_vec3 vPos; 
	m_pTransformCom->Get_INFO(INFO_POS, &vPos);
	_float fRadius = 100.f; 
	pComponent = m_pColliderCom = CColliderSphere::Create(m_pGraphicDev,&vPos, fRadius);
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Collider", pComponent);


	return S_OK;
}