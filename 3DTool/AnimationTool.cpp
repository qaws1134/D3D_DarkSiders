// AnimationTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "3DTool.h"
#include "AnimationTool.h"
#include "afxdialogex.h"

#include "ToolCam.h"



// CAnimationTool 대화 상자입니다.

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


// CAnimationTool 메시지 처리기입니다.


void CAnimationTool::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnTimer(nIDEvent);
}
