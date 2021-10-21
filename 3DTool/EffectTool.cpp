// EffectTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "3DTool.h"
#include "EffectTool.h"
#include "afxdialogex.h"


// CEffectTool 대화 상자입니다.

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


// CEffectTool 메시지 처리기입니다.
