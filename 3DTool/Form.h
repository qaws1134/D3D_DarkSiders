#pragma once

#include "afxcmn.h"

// CForm �� ���Դϴ�.
#include "ColliderTool.h"
#include "MeshTool.h"
class CForm : public CFormView
{
	DECLARE_DYNCREATE(CForm)

protected:
	CForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
public:
	void Release_Tools(void);

public:
	void Picking_View(RAY& MouseRay);
	void Picking_Moving(RAY & MouseRay);
	void Picking_Off(void);

	HRESULT Ready_Layer(const _tchar* pLayerTag);
public :
	
public:
	CColliderTool*			m_pColliderTool = nullptr;
	CMeshTool*			m_pMeshTool = nullptr;


	CGraphicDev*		m_pDeviceClass = nullptr;
	LPDIRECT3DDEVICE9	m_pDevice = nullptr;

	class CToolCam*		m_pCamera = nullptr;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	CTabCtrl m_tTabCtrl;
	int				m_iCurSel = 0;

	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};


