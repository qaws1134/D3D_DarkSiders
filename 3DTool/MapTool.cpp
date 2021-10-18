// MapTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "3DTool.h"
#include "MapTool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "Form.h"
#include "ToolCam.h"

// CMapTool ��ȭ �����Դϴ�.

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


// CMapTool �޽��� ó�����Դϴ�.


void CMapTool::OnTimer(UINT_PTR nIDEvent)
{

	if (0 != m_pForm->m_iCurSel)
		return;

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.


	CDialog::OnTimer(nIDEvent);
}
