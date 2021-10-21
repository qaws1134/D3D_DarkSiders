// CameraTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "3DTool.h"
#include "CameraTool.h"
#include "afxdialogex.h"


// CCameraTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCameraTool, CDialog)

CCameraTool::CCameraTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CAMERATOOL, pParent)
{

}

CCameraTool::~CCameraTool()
{
}

void CCameraTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCameraTool, CDialog)
END_MESSAGE_MAP()


// CCameraTool 메시지 처리기입니다.
