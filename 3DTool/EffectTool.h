#pragma once


// CEffectTool 대화 상자입니다.

class CEffectTool : public CDialog
{
	DECLARE_DYNAMIC(CEffectTool)

public:
	CEffectTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEffectTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFFECTTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
