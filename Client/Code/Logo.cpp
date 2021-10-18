#include "stdafx.h"
#include "Logo.h"

#include "Export_Function.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{

}

CLogo::~CLogo(void)
{

}

HRESULT CLogo::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Environment"), E_FAIL);


	// 임시 카메라

	_matrix		matView, matProj;

	D3DXMatrixLookAtLH(&matView, &_vec3(0.f, 0.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f));
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	return S_OK;
}

Engine::_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	return CScene::Update_Scene(fTimeDelta);
}

void CLogo::Render_Scene(void)
{
	CScene::Render_Scene();
}

HRESULT CLogo::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;

	// BackGround
	/*pGameObject = CBackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround", pGameObject), E_FAIL);*/

	// Player
	pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);

	// Monster
	pGameObject = CMonster::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);
	
	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CLogo::Ready_Prototype(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Buffer_TriCol", CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Buffer_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Texture_Logo", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Logo/sana.jpg", TEX_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Texture_Player", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player/Ma.jpg", TEX_NORMAL, 1)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);

	return S_OK;
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo*	pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CLogo::Free(void)
{
	CScene::Free();
}


