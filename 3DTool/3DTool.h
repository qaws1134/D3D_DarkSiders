
// 3DTool.h : 3DTool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMy3DToolApp:
// �� Ŭ������ ������ ���ؼ��� 3DTool.cpp�� �����Ͻʽÿ�.
//

class CMy3DToolApp : public CWinAppEx
{
public:
	CMy3DToolApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMy3DToolApp theApp;
