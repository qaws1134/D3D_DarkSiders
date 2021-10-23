#include "stdafx.h"
#include "NaviMesh.h"

#include "Export_Function.h"

CNaviMesh::CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CNaviMesh::CNaviMesh(const CNaviMesh& rhs)
	: CGameObject(rhs)
{

}

CNaviMesh::~CNaviMesh(void)
{

}

HRESULT CNaviMesh::Ready_Object( )
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	return S_OK;
}

void CNaviMesh::Late_Ready_Object()
{
}

_int CNaviMesh::Update_Object(const _float& fTimeDelta)
{

	_int iExit = CGameObject::Update_Object(fTimeDelta);


	Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CNaviMesh::Render_Object(void)
{	
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

}

CNaviMesh* CNaviMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 vPos)
{
	CNaviMesh*	pInstance = new CNaviMesh(pGraphicDev);
	pInstance->SetPos(vPos);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}


void CNaviMesh::Free(void)
{
	CGameObject::Free();
}

HRESULT CNaviMesh::Add_Component()
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
	pComponent = m_pColliderCom = CColliderSphere::Create(m_pGraphicDev, &m_vPos, 1.f);
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Collider", pComponent);

	return S_OK;
}