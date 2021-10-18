
// 3DToolView.h : CMy3DToolView Ŭ������ �������̽�
//

#pragma once

#include "3DToolDoc.h"
class CMy3DToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMy3DToolView();
	DECLARE_DYNCREATE(CMy3DToolView)

// Ư���Դϴ�.
public:
	CMy3DToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	class CGraphicDev*			m_pGraphicDev = nullptr;
	class CTerrain*				m_pTerrain= nullptr;
	LPDIRECT3DDEVICE9			m_pDevice = nullptr;


	class	 CTerrain* GetTerrain() { return m_pTerrain; }

public :

	class CForm*			m_pForm = nullptr;
// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);


private:
	HRESULT SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
// �����Դϴ�.
public:
	virtual ~CMy3DToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:



// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // 3DToolView.cpp�� ����� ����
inline CMy3DToolDoc* CMy3DToolView::GetDocument() const
   { return reinterpret_cast<CMy3DToolDoc*>(m_pDocument); }
#endif

//HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
//{
//	FAILED_CHECK_RETURN(Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);
//	m_pDeviceClass->AddRef();
//
//	*ppGraphicDev = m_pDeviceClass->GetDevice();
//	(*ppGraphicDev)->AddRef();
//
//	(*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	return S_OK;
//}
