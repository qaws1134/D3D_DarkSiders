// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "3DTool.h"
#include "MapTool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "Form.h"
#include "ToolCam.h"

// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
{

}

CMapTool::~CMapTool()
{
}

void CMapTool::Release_Tools()
{
	KillTimer(MAPTOOL_TIMER);
	Safe_Release(m_pToolCam);
	Safe_Release(m_pDevice);

}

void CMapTool::Set_Camera(CToolCam * pToolcam)
{
	m_pToolCam = pToolcam;
	memcpy(&m_tCameraDesc, &m_pToolCam->Get_CameraDesc(), sizeof(CAMERA_DESC));
	m_pToolCam->AddRef();
}

const CAMERA_DESC & CMapTool::Get_CameraDesc(void)
{
	return m_tCameraDesc;
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMapTool 메시지 처리기입니다.


void CMapTool::OnTimer(UINT_PTR nIDEvent)
{

	if (0 != m_pForm->m_iCurSel)
		return;

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


	CDialog::OnTimer(nIDEvent);
}
