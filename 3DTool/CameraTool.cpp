// CameraTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "3DTool.h"
#include "CameraTool.h"
#include "afxdialogex.h"


// CCameraTool ��ȭ �����Դϴ�.

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


// CCameraTool �޽��� ó�����Դϴ�.
