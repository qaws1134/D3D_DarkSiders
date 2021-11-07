#include "stdafx.h"
#include "MainApp.h"
#include "Define.h"
#include "Logo.h"


CMainApp::CMainApp(void)
{

}

CMainApp::~CMainApp(void)
{

}

HRESULT CMainApp::Ready_MainApp(void)
{
	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Renderer(m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);

	return S_OK;
}

_int CMainApp::Update_MainApp(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pManagementClass, -1);

	Update_InputDev();
	Key_Update();

	m_pManagementClass->Update_Scene(fTimeDelta);

	return 0;
}

void CMainApp::Render_MainApp(void)
{
	NULL_CHECK(m_pManagementClass);

	Render_Begin(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.f));

	m_pManagementClass->Render_Scene(m_pGraphicDev);

	Render_End();

}

HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	FAILED_CHECK_RETURN(Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);
	m_pDeviceClass->AddRef();

	*ppGraphicDev = m_pDeviceClass->GetDevice();
	(*ppGraphicDev)->AddRef();


	(*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Font Ãß°¡
	FAILED_CHECK_RETURN(Ready_Font(*ppGraphicDev, L"Font_Default", L"¹ÙÅÁ", 15, 15, FW_NORMAL), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Font(*ppGraphicDev, L"Font_Jinji", L"±Ã¼­", 30, 30, FW_HEAVY), E_FAIL);


	// Input Ãß°¡
	FAILED_CHECK_RETURN(Ready_InputDev(g_hInst, g_hWnd), E_FAIL);



	return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9& pGraphicDev, CManagement** ppManagement)
{
	CScene*				pScene = nullptr;

	FAILED_CHECK_RETURN(Create_Management(pGraphicDev,ppManagement), E_FAIL);
	(*ppManagement)->AddRef();
	
	pScene = CLogo::Create(pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	FAILED_CHECK_RETURN((*ppManagement)->Set_Scene(pScene), E_FAIL);
	
	return S_OK;
}

CMainApp* CMainApp::Create(void)
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->Ready_MainApp()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMainApp::Free(void)
{
	Safe_Release(m_pGraphicDev);

	Safe_Release(m_pManagementClass);
	Safe_Release(m_pDeviceClass);

	Release_Utility();
	Release_System();
}

