// Form.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "3DTool.h"
#include "Form.h"
#include "ToolCam.h"

// CForm


IMPLEMENT_DYNCREATE(CForm, CFormView)

CForm::CForm()
	: CFormView(IDD_FORM)
{

}

CForm::~CForm()
{


	//Release_Utility();
	//Release_System();
}

void CForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tTabCtrl);
}

BEGIN_MESSAGE_MAP(CForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CForm::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CForm 진단입니다.

#ifdef _DEBUG
void CForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

void CForm::Release_Tools(void)
{
	m_pMapTool->Release_Tools();
	m_pMeshTool->Release_Tools();


	Safe_Delete(m_pMapTool);
	Safe_Delete(m_pMeshTool);

	Safe_Release(m_pCamera);

	Safe_Release(m_pDevice);
}

void CForm::Picking_View(RAY & MouseRay)
{
	_int iCursel = m_tTabCtrl.GetCurSel();

	/* 클릭했을시 수행할 작업들 */
	// 여기서 각자 피킹작업 하면 될듯
	switch (iCursel)
	{
	case 0:
		//m_pMapTool->Set_PickingOn();
		break;
	case 1:
		//m_pMeshTool
		m_pMeshTool->PickNavi(MouseRay);
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}
}

void CForm::Picking_Moving(RAY & MouseRay)
{
}

void CForm::Picking_Off(void)
{
}

HRESULT CForm::Ready_Layer(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;


	return S_OK;
}


#endif
#endif //_DEBUG


// CForm 메시지 처리기입니다.


void CForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();


	/* 넘겨줄 그래픽장치 준비 */


	m_pDevice=GetDevice();

	NULL_CHECK_RETURN(m_pDevice, );
	m_pDevice->AddRef();

	UpdateData(TRUE);

	CAMERA_DESC CameraDesc;
	CameraDesc.fFovY = D3DXToRadian(60.f);
	CameraDesc.fAspect = (_float)WINCX / WINCY;
	CameraDesc.fNear = 1.f;
	CameraDesc.fFar = 1000.f;
	CameraDesc.vEye = _vec3(0.f, 10.f, -5.f);
	CameraDesc.vAt = _vec3(0.f, 0.f, 0.f);

	//CGameObject*		pGameObject = nullptr;




	CRect rect;
	m_tTabCtrl.GetWindowRect(&rect);
	m_tTabCtrl.InsertItem(0, L"MapTool");
	m_tTabCtrl.InsertItem(1, L"MeshTool");

	m_tTabCtrl.SetCurSel(0);

	m_pCamera = CToolCam::Create(m_pDevice, CameraDesc);
	NULL_CHECK_RETURN(m_pCamera, );

	m_pMapTool = new CMapTool;
	m_pMapTool->Create(IDD_MAPTOOL,
		&m_tTabCtrl);
	m_pMapTool->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pMapTool->ShowWindow(SW_SHOW);
	m_pMapTool->Set_Camera(m_pCamera);

	/* For ObjectTool */

	m_pMeshTool = new CMeshTool;
	m_pMeshTool->Create(IDD_MESHTOOL,
		&m_tTabCtrl);
	m_pMeshTool->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pMeshTool->ShowWindow(SW_HIDE);
	m_pMeshTool->Set_Camera(m_pCamera);


	UpdateData(FALSE);


}


void CForm::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_iCurSel = m_tTabCtrl.GetCurSel();

	switch (m_iCurSel)
	{
	case 0:
		m_pMapTool->ShowWindow(SW_SHOW);
		m_pMeshTool->ShowWindow(SW_HIDE);

		m_pCamera->Resetting_Camera(m_pMapTool->Get_CameraDesc());
		break;
	case 1:
		m_pMapTool->ShowWindow(SW_HIDE);
		m_pMeshTool->ShowWindow(SW_SHOW);

		m_pCamera->Resetting_Camera(m_pMeshTool->Get_CameraDesc());
		break;
	}
	*pResult = 0;
}
