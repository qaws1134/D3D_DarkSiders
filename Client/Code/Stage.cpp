#include "stdafx.h"
#include "Stage.h"

#include "Export_Function.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{

}

CStage::~CStage(void)
{

}

HRESULT CStage::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);


	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"UI"), E_FAIL);
	
	return S_OK;
}

Engine::_int CStage::Update_Scene(const _float& fTimeDelta)
{
	m_fTime += fTimeDelta;

	return CScene::Update_Scene(fTimeDelta);
}

void CStage::Render_Scene(void)
{
	// DEBUG 용
	m_dwRenderCnt++;

	if (m_fTime >= 1.f)
	{
		wsprintf(m_szFPS, L"FPS : %d", m_dwRenderCnt);
		m_dwRenderCnt = 0;
		m_fTime = 0.f;
	}

	Render_Font(L"Font_Jinji", m_szFPS, &_vec2(400.f, 10.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
}

HRESULT CStage::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;


	//CAMERA_DESC CameraDesc;
	//CameraDesc.fFovY = D3DXToRadian(60.f);
	//CameraDesc.fAspect = (_float)WINCX / WINCY;
	//CameraDesc.fNear = 1.f;
	//CameraDesc.fFar = 1000.f;
	//CameraDesc.vEye = _vec3(0.f, 10.f, -5.f);
	//CameraDesc.vAt = _vec3(0.f, 0.f, 0.f);
	//// DynamicCamera
	//pGameObject = CDynamicCamera::Create(m_pGraphicDev, CameraDesc);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);

	//// SkyBox
	//pGameObject = CSkyBox::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);


	
	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;

	// Terrain
	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);
	//
	//// Stone
	//pGameObject = CStone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Stone", pGameObject), E_FAIL);

	//// effect

	//for (_ulong i = 0; i < 100; ++i)
	//{
	//	pGameObject = CEffect::Create(m_pGraphicDev);
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Effect", pGameObject), E_FAIL);
	//}
	//


//#pragma region PLAYER
	// Player
	pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);
//
//	// Sword
//	pGameObject = CSword::Create(m_pGraphicDev);
//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Sword", pGameObject), E_FAIL);
//
//#pragma endregion PLAYER



	m_mapLayer.emplace(pLayerTag, pLayer);
	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;

	//CAMERA_DESC CameraDesc;
	//CameraDesc.fFovY = D3DXToRadian(60.f);
	//CameraDesc.fAspect = (_float)WINCX / WINCY;
	//CameraDesc.fNear = 1.f;
	//CameraDesc.fFar = 1000.f;
	//CameraDesc.vEye = _vec3(0.f, 10.f, -5.f);
	//CameraDesc.vAt = _vec3(0.f, 0.f, 0.f);


	pGameObject = CStaticCamera::Create(m_pGraphicDev, CameraDesc);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	auto iter_find = m_mapLayer.find(L"GameLogic");
	pGameObject->SetTarget(iter_find->second->Get_GameObject(L"Player"));
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StaticCamera", pGameObject), E_FAIL);


	//pGameObject = CUI_WeaponElement::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Element", pGameObject), E_FAIL);

	//매니져 돌면서 레이어 추가

	//리스트를 반환 

	
	for (auto iter : CUIMgr::GetInstance()->InitCreateUI(m_pGraphicDev))
	{
		for (auto iter_second : iter)
		{
			auto iter_find = m_mapLayer.find(L"GameLogic");
			iter_second->SetTarget(iter_find->second->Get_GameObject(L"Player"));
			NULL_CHECK_RETURN(iter_second, E_FAIL);
			//Obj 태그 반환 
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(dynamic_cast<CUI*>(iter_second)->GetObjTag().c_str(), iter_second), E_FAIL);
		}
	}


	m_mapLayer.emplace(pLayerTag, pLayer);
	return S_OK;
}


HRESULT CStage::Ready_LightInfo(void)
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage*	pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CStage::Free(void)
{

	CScene::Free();
}


