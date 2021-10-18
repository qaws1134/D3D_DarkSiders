#include "stdafx.h"
#include "NaviTri.h"

#include "Export_Function.h"

CNaviTri::CNaviTri(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CNaviTri::CNaviTri(const CNaviTri& rhs)
	: CGameObject(rhs)
{

}

CNaviTri::~CNaviTri(void)
{

}

HRESULT CNaviTri::Ready_Object(_vec3 *pAryPos)
{
	memcpy(m_pTriPos, pAryPos, sizeof(_vec3) * 3);
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

void CNaviTri::Late_Ready_Object()
{
}

_int CNaviTri::Update_Object(const _float& fTimeDelta)
{

	_int iExit = CGameObject::Update_Object(fTimeDelta);


	Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CNaviTri::Render_Object(void)
{	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());


}

CNaviTri* CNaviTri::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 *pAryPos)
{
	CNaviTri*	pInstance = new CNaviTri(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pAryPos)))
		Safe_Release(pInstance);

	return pInstance;
}


void CNaviTri::Free(void)
{
	CGameObject::Free();
}

HRESULT CNaviTri::Add_Component()
{
	CComponent*		pComponent = nullptr;

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

	pComponent = m_pTriColCom = CTriCol::Create(m_pGraphicDev,m_pTriPos);
	NULL_CHECK_RETURN(m_pTriColCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"com_TriBuffer", pComponent);


	return S_OK;
}