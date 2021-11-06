#include "stdafx.h"
#include "WaterBoss.h"

#include "Export_Function.h"

CWaterBoss::CWaterBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CWaterBoss::CWaterBoss(const CWaterBoss& rhs)
	: CGameObject(rhs)
{

}

CWaterBoss::~CWaterBoss(void)
{

}

HRESULT CWaterBoss::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CWaterBoss::Update_Object(const _float& fTimeDelta)
{
	
	_int iExit = CGameObject::Update_Object(fTimeDelta);

	return iExit;
}

void CWaterBoss::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pTextureCom->Set_Texture();

	//m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CWaterBoss* CWaterBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWaterBoss*	pInstance = new CWaterBoss(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CWaterBoss::Free(void)
{
	CGameObject::Free();
}

HRESULT CWaterBoss::Add_Component(void)
{
	//CComponent*		pComponent = nullptr;

	//// Mesh
	//pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Prototype(m_wstrProtoMesh.c_str()));
	//NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	//// Transform
	//pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	//NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	//m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	//// Renderer
	//pComponent = m_pRendererCom = Engine::Get_Renderer();
	//NULL_CHECK_RETURN(m_pRendererCom, E_FAIL);
	//pComponent->AddRef();
	//m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

	//// Calculator
	//pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Prototype(L"Proto_Calculator"));
	//NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Calculator", pComponent);


	//_vec3 vPos;
	//m_pTransformCom->Get_INFO(INFO_POS, &vPos);
	//_float fRadius = 100.f;
	//pComponent = m_pColliderCom = CColliderSphere::Create(m_pGraphicDev, &vPos, fRadius);
	//NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Collider", pComponent);
	//

	return S_OK;
}
