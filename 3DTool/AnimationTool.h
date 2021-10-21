#pragma once


// CAnimationTool ��ȭ �����Դϴ�.
#define ANIMATION_TIMER 100

#include "Camera.h"

USING(Engine)
class CAnimationTool : public CDialog
{
	DECLARE_DYNAMIC(CAnimationTool)

public:
	CAnimationTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAnimationTool();

	const CAMERA_DESC& Get_CameraDesc(void);
	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLLIDERTOOL };
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
