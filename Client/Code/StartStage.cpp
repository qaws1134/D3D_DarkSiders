#include "stdafx.h"
#include "StartStage.h"
#include "GameMgr.h"
#include "LoadMgr.h"
#include "WaterBoss_Orb.h"
#include "Stage.h"
#include "Logo.h"
#include "Export_Function.h"
#include "SkyBox.h"
#include "Effect_Trail.h"
CStartStage::CStartStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{

}

CStartStage::~CStartStage(void)
{

}

HRESULT CStartStage::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"UI"), E_FAIL);


	return S_OK;
}

Engine::_int CStartStage::Update_Scene(const _float& fTimeDelta)
{

	Begin_Scene();

	_int iExit = CScene::Update_Scene(fTimeDelta);
	if (Key_Down(KEY_L))
	{
		CGameMgr::GetInstance()->TakeSoul(100);
		_int i = CGameMgr::GetInstance()->GetSoul();
		//dynamic_cast<CUI*>(CGameMgr::GetInstance()->GetFontObj())->GetFont().wstrText = to_wstring(CGameMgr::GetInstance()->GetSoul());
	}

	if (Key_Down(KEY_K))
	{
		_uint i = rand() % 20;
		STONE tStone = CGameMgr::GetInstance()->GetStone(UI::STONE(i));
		CUIMgr::GetInstance()->SetStoneInfoUI(m_pGraphicDev, tStone);
		CUIMgr::GetInstance()->SetStoneListUI(m_pGraphicDev, tStone);
	}

	if (m_bSceneStart)
	{

		CScene*		pScene = nullptr;

		pScene = CLogo::Create(m_pGraphicDev, false);
		NULL_CHECK_RETURN(pScene, E_FAIL);
		dynamic_cast<CLogo*>(pScene)->SetLoading(CLoading::LOADING_STAGE);
		FAILED_CHECK_RETURN(Set_Scene(pScene), E_FAIL);

		return iExit;
		
	}
	return iExit;
}

void CStartStage::Render_Scene(void)
{
	// DEBUG ¿ë
	//m_dwRenderCnt++;

	//if (m_fTime >= 1.f)
	//{
	//	wsprintf(m_szFPS, L"FPS : %d", m_dwRenderCnt);
	//	m_dwRenderCnt = 0;
	//	m_fTime = 0.f;
	//}

	//Render_Font(L"Font_Jinji", m_szFPS, &_vec2(400.f, 10.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));



	//wsprintf(m_szPosX, L"X : %d", m_dwPosX);
	//wsprintf(m_szPosY, L"Y : %d", m_dwPosY);


	//Render_Font(L"Font_Default", m_szPosX, &_vec2(50.f, 15.f), D3DXCOLOR(1.f, 0.f, 1.f, 1.f));
	//Render_Font(L"Font_Default", m_szPosY, &_vec2(200.f, 15.f), D3DXCOLOR(1.f, 0.f, 1.f, 1.f));


}

void CStartStage::Begin_Scene()
{
	if (m_bBegin)
		return;
	CLoadMgr::GetInstance()->SpawnData();
	Get_GameObject(L"Environment", L"StaticCamera")->SetTarget(CGameMgr::GetInstance()->GetPlayer());
	CUIMgr::GetInstance()->BeginUISet();
	CUIMgr::GetInstance()->InitStore(m_pGraphicDev);
	CUIMgr::GetInstance()->InitToastInfo(m_pGraphicDev);
	CGameMgr::GetInstance()->InitObjPool();
	
	CScene::Begin_Scene();
}



HRESULT CStartStage::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;

	CAMERA_DESC CameraDesc;
	CameraDesc.fFovY = D3DXToRadian(60.f);
	CameraDesc.fAspect = (_float)WINCX / WINCY;
	CameraDesc.fNear = 1.f;
	CameraDesc.fFar = 1000.f;
	CameraDesc.vEye = _vec3(0.f, 10.f, -5.f);
	CameraDesc.vAt = _vec3(0.f, 0.f, 0.f);


	pGameObject = CStaticCamera::Create(m_pGraphicDev, CameraDesc);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StaticCamera", pGameObject), E_FAIL);

	CGameMgr::GetInstance()->SetCamera(pGameObject);
	


	// SkyBox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);



	pGameObject = CEffect_Trail::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Trail", pGameObject), E_FAIL);


	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CStartStage::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;

	// Terrain
	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);
	return S_OK;
}

HRESULT CStartStage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;

	for (auto iter : CUIMgr::GetInstance()->InitCreateUI(m_pGraphicDev))
	{
		for (auto iter_second : iter.second)
		{
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(dynamic_cast<CUI*>(iter_second)->GetObjTag().c_str(), iter_second), E_FAIL);
		}
	}

	m_mapLayer.emplace(pLayerTag, pLayer);
	return S_OK;
}


HRESULT CStartStage::Ready_LightInfo(void)
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

CStartStage* CStartStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStartStage*	pInstance = new CStartStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CStartStage::Free(void)
{

	CScene::Free();
}


