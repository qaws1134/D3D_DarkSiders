// ColliderTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "3DTool.h"
#include "ColliderTool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "Form.h"
#include "3DToolView.h"
#include "ToolCam.h"
#include "DynamicMeshObj.h"
#include "DynamicMesh.h"

// CColliderTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CColliderTool, CDialog)

CColliderTool::CColliderTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_COLLIDERTOOL, pParent)
	, m_wstrColTag(L"")
	, m_fPositionX(0)
	, m_fPositionY(0)
	, m_fPositionZ(0)
	, m_fRadius(0)
{

}

CColliderTool::~CColliderTool()
{
}

void CColliderTool::Release_Tools()
{

	DeleteMultiMap(m_mapReadyMesh);
	Safe_Release(m_pSelectedGameObject);
	KillTimer(COLLIDERTOOL_TIMER);
	Safe_Release(m_pToolCam);
	Safe_Release(m_pDevice);
}

void CColliderTool::DeleteMultiMap(map<wstring, map<wstring, CGameObject*>> mapDelete)
{
	for (auto iterFirst : mapDelete)
	{
		for_each(iterFirst.second.begin(), iterFirst.second.end(), CDeleteMap());
	}
}
void CColliderTool::ExpandTree(CTreeCtrl * pTreeCtrl, HTREEITEM htreeItem)
{
	htreeItem = pTreeCtrl->GetFirstVisibleItem();
	while (htreeItem != NULL)
	{
		pTreeCtrl->Expand(htreeItem, TVE_EXPAND);
		htreeItem = pTreeCtrl->GetNextItem(htreeItem, TVGN_NEXTVISIBLE);
	}
}
void CColliderTool::Set_Camera(CToolCam * pToolcam)
{
	m_pToolCam = pToolcam;
	memcpy(&m_tCameraDesc, &m_pToolCam->Get_CameraDesc(), sizeof(CAMERA_DESC));
	m_pToolCam->AddRef();
}

const CAMERA_DESC & CColliderTool::Get_CameraDesc(void)
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_tCameraDesc;
}

void CColliderTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeMeshList);
	DDX_Control(pDX, IDC_TREE5, m_TreeBoneList);
	DDX_Control(pDX, IDC_TREE6, m_TreeAttached);
	DDX_Control(pDX, IDC_LIST2, m_ListAniIdx);
	DDX_Control(pDX, IDC_LIST1, m_ListSave);

	DDX_Text(pDX, IDC_EDIT10, m_wstrColTag);
	DDX_Text(pDX, IDC_EDIT11, m_fPositionX);
	DDX_Text(pDX, IDC_EDIT12, m_fPositionY);
	DDX_Text(pDX, IDC_EDIT13, m_fPositionZ);
	DDX_Text(pDX, IDC_EDIT14, m_fRadius);
}


BEGIN_MESSAGE_MAP(CColliderTool, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CColliderTool::OnBnClickedAddCol)
	ON_BN_CLICKED(IDC_BUTTON2, &CColliderTool::OnBnClickedDeleteCol)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CColliderTool::OnDeltaposSpinPositionX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CColliderTool::OnDeltaposSpinPositionY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN11, &CColliderTool::OnDeltaposSpinPositionZ)
	ON_BN_CLICKED(IDC_BUTTON3, &CColliderTool::OnBnClickedAddSaveList)
	ON_BN_CLICKED(IDC_BUTTON4, &CColliderTool::OnBnClickedDeleteSaveList)
	ON_BN_CLICKED(IDC_BUTTON5, &CColliderTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON6, &CColliderTool::OnBnClickedLoad)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CColliderTool::OnNMDblclkMeshList)
	ON_NOTIFY(NM_CLICK, IDC_TREE5, &CColliderTool::OnNMClickBoneList)
	ON_NOTIFY(NM_CLICK, IDC_TREE6, &CColliderTool::OnNMClickAttachedColList)
	ON_LBN_SELCHANGE(IDC_LIST2, &CColliderTool::OnLbnSelchangeAnimationList)
	ON_LBN_SELCHANGE(IDC_LIST1, &CColliderTool::OnLbnSelchangeSaveList)
END_MESSAGE_MAP()


// CColliderTool �޽��� ó�����Դϴ�.

BOOL CColliderTool::OnInitDialog()
{
	CDialog::OnInitDialog();


	m_pDevice = GetDevice();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->AddRef();

	CMainFrame*	pFrame = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	m_pToolView = dynamic_cast<CMy3DToolView*>(pFrame->m_tMainSplitter.GetPane(0, 0));
	m_pForm = dynamic_cast<CForm*>(pFrame->m_tMainSplitter.GetPane(0, 1));


	Ready_LightInfo();
	SetTimer(1, 1000 / 60000, NULL);


	HTREEITEM h_Mesh;
	h_Mesh = m_TreeMeshList.InsertItem(_T("Mesh"), NULL, NULL);
	CreateTree(L"DynamicMesh", h_Mesh);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CColliderTool::OnTimer(UINT_PTR nIDEvent)
{
	if (1 != m_pForm->m_iCurSel)
		return;

	Set_TimeDelta(L"Timer_DeltaTime");
	m_fDeltaTime = Get_TimeDelta(L"Timer_DeltaTime");



	if (m_pToolCam)
		m_pToolCam->Update_Object(m_fDeltaTime);

	if(m_pSelectedGameObject)
		m_pSelectedGameObject->Update_Object(m_fDeltaTime);


	CDialog::OnTimer(nIDEvent);
}


void CColliderTool::CreateTree(CString strPath, HTREEITEM Parents)
{
	HTREEITEM h_Layer;
	HTREEITEM h_Obj;

	CString strMeshTypePath = L"../Resource/Mesh/";
	CString strAllFileDot = L"/*.*";
	CString strFolderPath;
	CString strFilePath;

	h_Layer = m_TreeMeshList.InsertItem(strPath, Parents, NULL);

	//m_queFolder�� ������ ���� 
	Find_FolderList(strMeshTypePath + strPath + strAllFileDot);

	while (m_queFolder.size() > 0)
	{
		strFolderPath = m_queFolder.front();
		m_queFolder.pop();

		h_Obj = m_TreeMeshList.InsertItem(strFolderPath, h_Layer, NULL);
		//������ �ִ� ���� ���
		strFilePath = strMeshTypePath + strPath + L"/" + strFolderPath + strAllFileDot;

		Find_FolderList(strFilePath);

		while (m_queFile.size() > 0)
		{
			//���ϸ� ���� 
			strFilePath = m_queFile.front();
			m_queFile.pop();
			//��� ���� 
			m_TreeMeshList.InsertItem(strFilePath, h_Obj, NULL);
			if (strPath == L"DynamicMesh")
			{
				map<wstring, CGameObject*> mapFileProto;
				for (auto iter : m_mapReadyMesh)
				{
					for (auto mapProto : iter.second)
					{
						mapFileProto.emplace(mapProto.first, mapProto.second);
					}
				}
				CGameObject* pGameObject = SpawnDynamicMesh(strFilePath.GetString());

				if (!pGameObject)
					continue;
				mapFileProto.emplace(strFilePath.GetString(), pGameObject);
				m_mapReadyMesh[strPath.GetString()] = mapFileProto;
			}
		
		}
	}
	ExpandTree(&m_TreeMeshList, h_Layer);

}

CGameObject * CColliderTool::SpawnDynamicMesh(wstring wstrMeshObjKey)
{
	CGameObject* pGameObject = nullptr;
	pGameObject = CDynamicMeshObj::Create(m_pDevice, wstrMeshObjKey);

	return pGameObject;
}
HRESULT CColliderTool::Ready_LightInfo()
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	Ready_Light(m_pDevice, &tLightInfo, 0);

	return S_OK;
}
void CColliderTool::Find_FolderList(CString strPath)
{
	CFileFind finder;

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	BOOL bWorking = finder.FindFile(strPath);

	CString fileName;
	CString DirName;

	CString PathName;

	while (bWorking)
	{
		//���� ���� or ���� �� �����ϸ�ٸ� TRUE ��ȯ
		bWorking = finder.FindNextFile(); // folder �� ���� continue 

		if (finder.IsDirectory() && !finder.IsDots())
		{
			DirName = finder.GetFilePath();
			//Find_FolderList(strPath);
			DirName = finder.GetFileTitle();
			m_queFolder.emplace(DirName);
		}

		if (finder.IsArchived())
		{
			fileName = finder.GetFileName();

			CString file = fileName.Right(1);

			//X����-> �޽� �̸��� ������
			if (file == L"x" || file == L"X")
			{
				fileName = finder.GetFileTitle();
				m_queFile.emplace(fileName);
			}
		}

	}
}

void CColliderTool::Set_TreeBoneName(CTreeCtrl * pTreeCtrl, HTREEITEM h_Parants, D3DXFRAME_DERIVED* pFrame)
{
	//pTreeCtrl->DeleteAllItems();
	USES_CONVERSION;

	HTREEITEM h_MeshType;
	wstring wstrBoneName;
	wstrBoneName = A2W(pFrame->Name);
	h_MeshType = pTreeCtrl->InsertItem(wstrBoneName.c_str(), h_Parants, NULL);

	if (nullptr != pFrame->pFrameSibling)
		Set_TreeBoneName(pTreeCtrl, h_MeshType,(D3DXFRAME_DERIVED*)pFrame->pFrameSibling);

	if (nullptr != pFrame->pFrameFirstChild)
		Set_TreeBoneName(pTreeCtrl, h_MeshType,(D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);


	ExpandTree(pTreeCtrl, h_MeshType);
}






void CColliderTool::OnBnClickedAddCol()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CColliderTool::OnBnClickedDeleteCol()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CColliderTool::OnDeltaposSpinPositionX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}


void CColliderTool::OnDeltaposSpinPositionY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}


void CColliderTool::OnDeltaposSpinPositionZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}


void CColliderTool::OnBnClickedAddSaveList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CColliderTool::OnBnClickedDeleteSaveList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CColliderTool::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CColliderTool::OnBnClickedLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}




void CColliderTool::OnNMDblclkMeshList(NMHDR *pNMHDR, LRESULT *pResult)
{
	POINT pt;
	GetCursorPos(&pt);
	UINT flag;
	m_TreeMeshList.ScreenToClient(&pt);
	HTREEITEM hItem = m_TreeMeshList.HitTest(pt, &flag);

	if (!m_TreeMeshList.ItemHasChildren(hItem))
	{
		CString wstrMeshKey = m_TreeMeshList.GetItemText(hItem);
		auto iter_find = m_mapReadyMesh.find(L"DynamicMesh");

		auto iter_Obj = iter_find->second.find(wstrMeshKey.GetString());

		//������ Ű������ ���̳��� �Ž� �����ؼ� �� ����Ʈ ����� 
		if (m_pSelectedGameObject != nullptr)
			Safe_Release(m_pSelectedGameObject);
		m_pSelectedGameObject = CDynamicMeshObj::Create(m_pDevice, wstrMeshKey.GetString());

		D3DXFRAME* pRootFrame = dynamic_cast<CDynamicMesh*>(m_pSelectedGameObject->Get_Component(L"Com_Mesh", ID_STATIC))->GetRootFrame();

		Set_TreeBoneName(&m_TreeBoneList, NULL,(D3DXFRAME_DERIVED*)pRootFrame);
	}
	*pResult = 0;
}


void CColliderTool::OnNMClickBoneList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	USES_CONVERSION;

	POINT pt;
	GetCursorPos(&pt);
	UINT flag;
	m_TreeBoneList.ScreenToClient(&pt);
	HTREEITEM hItem = m_TreeBoneList.HitTest(pt, &flag);

	CString wstrBoneKey = m_TreeMeshList.GetItemText(hItem);
	
	const char* pFrameName = W2A(wstrBoneKey.GetString());

	const D3DXFRAME_DERIVED* pFrame  = dynamic_cast<CDynamicMesh*>(m_pSelectedGameObject->Get_Component(L"Com_Mesh", ID_STATIC))->Get_FrameByName(pFrameName);

	m_tColInfo.vCenterPos = { pFrame->CombinedTransformMatrix._41,pFrame->CombinedTransformMatrix._42 ,pFrame->CombinedTransformMatrix._43};


	*pResult = 0;
}


void CColliderTool::OnNMClickAttachedColList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}


void CColliderTool::OnLbnSelchangeAnimationList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CColliderTool::OnLbnSelchangeSaveList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
