// EffectTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "3DTool.h"
#include "EffectTool.h"
#include "afxdialogex.h"


// CEffectTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CEffectTool, CDialog)

CEffectTool::CEffectTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_EFFECTTOOL, pParent)
{

}

CEffectTool::~CEffectTool()
{
}

void CEffectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEffectTool, CDialog)
END_MESSAGE_MAP()


// CEffectTool �޽��� ó�����Դϴ�.
