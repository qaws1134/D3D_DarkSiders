
// 3DToolView.cpp : CMy3DToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "3DTool.h"
#endif

#include "3DToolDoc.h"
#include "3DToolView.h"
#include "MainFrm.h"
#include "Form.h"
#include "Terrain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMy3DToolView
HWND g_hWnd;
HINSTANCE g_hInst;


IMPLEMENT_DYNCREATE(CMy3DToolView, CView)

BEGIN_MESSAGE_MAP(CMy3DToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CMy3DToolView ����/�Ҹ�

CMy3DToolView::CMy3DToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

HRESULT CMy3DToolView::SetUp_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{	
	Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pGraphicDev);
	m_pGraphicDev->AddRef();
	
	*ppGraphicDev = m_pGraphicDev->GetDevice();
	(*ppGraphicDev)->AddRef();
	
	(*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	return S_OK;
}



CMy3DToolView::~CMy3DToolView()
{
	Safe_Release(m_pTerrain);
	m_pForm->Release_Tools();
	m_pForm = nullptr;

	Safe_Release(m_pDevice);
	Safe_Release(m_pGraphicDev);

	Release_Utility();
	Release_System();
}

BOOL CMy3DToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMy3DToolView �׸���

void CMy3DToolView::OnDraw(CDC* /*pDC*/)
{
	CMy3DToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CMy3DToolView �μ�

BOOL CMy3DToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMy3DToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMy3DToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CMy3DToolView ����

#ifdef _DEBUG
void CMy3DToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMy3DToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy3DToolDoc* CMy3DToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy3DToolDoc)));
	return (CMy3DToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMy3DToolView �޽��� ó����


void CMy3DToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();


	g_hWnd = m_hWnd;
	g_hInst = AfxGetInstanceHandle();

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	RECT rcMain = {};
	pMain->GetWindowRect(&rcMain);

	RECT rcView{};
	GetClientRect(&rcView);

	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);

	int iGapX = rcMain.right - rcView.right;
	int iGapY = rcMain.bottom - rcView.bottom;

	pMain->SetWindowPos(nullptr, 0, 0, WINCX + iGapX, WINCY+ iGapY, SWP_NOZORDER);

	/* ��ġ�غ� */
	SetUp_DefaultSetting(&m_pDevice);

	/* Form ������ �����°� */
	CMainFrame*	pFrame = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	m_pForm = dynamic_cast<CForm*>(pFrame->m_tMainSplitter.GetPane(0, 1));

	NULL_CHECK_RETURN(m_pForm, );


	Ready_Timer(L"Timer_DeltaTime");

	SetTimer(0, 1000 / 60000, 0);

	Ready_InputDev(g_hInst, g_hWnd);

	Ready_Prototype(L"Proto_Transform", CTransform::Create(m_pDevice));

	Ready_Prototype(L"Proto_Calculator", CCalculator::Create(m_pDevice));
	Ready_Prototype(L"Proto_ColliderSphere", CTriCol::Create(m_pDevice));


	Ready_Prototype(L"Proto_Buffer_TerrainTex", CTerrainTex::Create(m_pDevice, VTXCNTX, VTXCNTZ));
	Ready_Prototype(L"Proto_Buffer_CubeTex", CCubeTex::Create(m_pDevice));
	Ready_Prototype(L"War", CDynamicMesh::Create(m_pDevice, L"../Resource/Mesh/DynamicMesh/War/", L"War.X"));


	// Terrain
	m_pTerrain = CTerrain::Create(m_pDevice);
	NULL_CHECK_RETURN(m_pTerrain, );

}


void CMy3DToolView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	Update_InputDev();
	m_pTerrain->Update_Object(0.f);
	Render_Begin(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.f));
	Render_GameObject(m_pDevice);
	Render_End();


	CView::OnTimer(nIDEvent);
}


void CMy3DToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	RAY rayMouse = CreateMouseRay(_vec2((_float)pt.x, (_float)pt.y), _vec2((_float)WINCX, (_float)WINCY));


	m_pForm->Picking_View(rayMouse);

	CView::OnLButtonDown(nFlags, point);
}


void CMy3DToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	RAY rayMouse = CreateMouseRay(_vec2((_float)pt.x, (_float)pt.y), _vec2((_float)WINCX, (_float)WINCY));


	m_pForm->Picking_Moving(rayMouse);

	CView::OnMouseMove(nFlags, point);
}
