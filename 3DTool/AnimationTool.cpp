// AnimationTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "3DTool.h"
#include "AnimationTool.h"
#include "afxdialogex.h"

#include "ToolCam.h"



// CAnimationTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAnimationTool, CDialog)

CAnimationTool::CAnimationTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ANIMATIONTOOL, pParent)
{

}

CAnimationTool::~CAnimationTool()
{
}

const CAMERA_DESC & CAnimationTool::Get_CameraDesc(void)
{
	return m_tCameraDesc;
}

void CAnimationTool::Release_Tools()
{


	KillTimer(ANIMATION_TIMER);

	Safe_Release(m_pToolCam);
	Safe_Release(m_pDevice);
}

void CAnimationTool::Set_Camera(CToolCam * pToolcam)
{
	m_pToolCam = pToolcam;
	memcpy(&m_tCameraDesc, &m_pToolCam->Get_CameraDesc(), sizeof(CAMERA_DESC));
	m_pToolCam->AddRef();
}

void CAnimationTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}



BEGIN_MESSAGE_MAP(CAnimationTool, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CAnimationTool �޽��� ó�����Դϴ�.


void CAnimationTool::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnTimer(nIDEvent);
}
