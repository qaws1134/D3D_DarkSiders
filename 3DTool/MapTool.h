#pragma once


// CMapTool 대화 상자입니다.
#define MAPTOOL_TIMER 100

#include "Camera.h"

USING(Engine)
class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();

	const CAMERA_DESC& Get_CameraDesc(void);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

public:
	void Release_Tools();
	void  Set_Camera(class CToolCam * pToolcam);
public:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;


	class CForm*			m_pForm = nullptr;
	class CToolCam*			m_pToolCam = nullptr;


	_int					m_iCursel = 0;
	_float					m_fDeltaTime = 0.f;
	CAMERA_DESC				m_tCameraDesc;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()




public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
