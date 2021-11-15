// ColliderTool.cpp : 구현 파일입니다.
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
#include "ColSphereMesh.h"
// CColliderTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CColliderTool, CDialog)

CColliderTool::CColliderTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_COLLIDERTOOL, pParent)
	, m_wstrColTag(L"")
	, m_fPositionX(0)
	, m_fPositionY(0)
	, m_fPositionZ(0)
	, m_fRadius(1.f)
{
	m_tColInfo.fRadius = 1.f;
	m_tColInfo.vCenterPos = _vec3{ 0.f,0.f,0.f };
}

CColliderTool::~CColliderTool()
{
}

void CColliderTool::Release_Tools()
{

	DeleteMultiMap(m_mapReadyMesh);
	for_each(m_mapCollider.begin(), m_mapCollider.end(), CDeleteMap());

	DeleteMultiMap(m_mapMeshCollider);
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
	// TODO: 여기에 반환 구문을 삽입합니다.
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
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN12, &CColliderTool::OnDeltaposSpinRadius)
	ON_BN_CLICKED(IDC_BUTTON3, &CColliderTool::OnBnClickedAddSaveList)
	ON_BN_CLICKED(IDC_BUTTON4, &CColliderTool::OnBnClickedDeleteSaveList)
	ON_BN_CLICKED(IDC_BUTTON5, &CColliderTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON6, &CColliderTool::OnBnClickedLoad)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CColliderTool::OnNMDblclkMeshList)
	ON_NOTIFY(NM_CLICK, IDC_TREE5, &CColliderTool::OnNMClickBoneList)
	ON_NOTIFY(NM_CLICK, IDC_TREE6, &CColliderTool::OnNMClickAttachedColList)
	ON_LBN_SELCHANGE(IDC_LIST2, &CColliderTool::OnLbnSelchangeAnimationList)
	ON_LBN_SELCHANGE(IDC_LIST1, &CColliderTool::OnLbnSelchangeSaveList)
	ON_EN_CHANGE(IDC_EDIT11, &CColliderTool::OnEnChangePositionX)
	ON_EN_CHANGE(IDC_EDIT12, &CColliderTool::OnEnChangePositionY)
	ON_EN_CHANGE(IDC_EDIT13, &CColliderTool::OnEnChangePositionZ)
	ON_EN_CHANGE(IDC_EDIT14, &CColliderTool::OnEnChangeRadius)
END_MESSAGE_MAP()


// CColliderTool 메시지 처리기입니다.

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
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CColliderTool::OnTimer(UINT_PTR nIDEvent)
{
	if (1 != m_pForm->m_iCurSel)
		return;

	Set_TimeDelta(L"Timer_DeltaTime");
	m_fDeltaTime = Get_TimeDelta(L"Timer_DeltaTime");

	if (m_pToolCam)
		m_pToolCam->Update_Object(m_fDeltaTime);


	if (m_pSelectedGameObject)
	{
		m_pSelectedGameObject->Update_Object(m_fDeltaTime);

		if (m_mapCollider.size() > 0)
		{
			if (m_pCtrlObj&& m_bSelect)
			{
				_vec3 vPos;
				m_pCtrlTransform->Get_INFO(INFO_POS, &vPos);
				
				m_pCtrlTransform->Set_Pos(&(vPos+m_tColInfo.vCenterPos));
				dynamic_cast<CColliderSphere*>(m_pCtrlObj->Get_Component(L"Com_Collider", ID_STATIC))->Update_Radius(m_tColInfo.fRadius);
				m_bSelect = false;
			}
			for (auto iter : m_mapCollider)
			{
				if(iter.second->GetTarget() == nullptr)
					iter.second->SetTarget(m_pSelectedGameObject);
				
				//dynamic_cast<CTransform*>(iter.second->Get_Component(L"Com_Transform",ID_DYNAMIC))->Set_Pos(&m_tColInfo.vCenterPos);


				iter.second->Update_Object(m_fDeltaTime);
			}
		}
	}


	//auto iter_find = m_mapCollider.find(wstrKey.GetString());




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

	//m_queFolder에 폴더명 저장 
	Find_FolderList(strMeshTypePath + strPath + strAllFileDot);

	while (m_queFolder.size() > 0)
	{
		strFolderPath = m_queFolder.front();
		m_queFolder.pop();

		h_Obj = m_TreeMeshList.InsertItem(strFolderPath, h_Layer, NULL);
		//파일이 있는 폴더 경로
		strFilePath = strMeshTypePath + strPath + L"/" + strFolderPath + strAllFileDot;

		Find_FolderList(strFilePath);

		while (m_queFile.size() > 0)
		{
			//파일명 저장 
			strFilePath = m_queFile.front();
			m_queFile.pop();
			//노드 생성 
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
	pGameObject = CDynamicMeshObj::Create(m_pDevice, wstrMeshObjKey,true);

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

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bWorking = finder.FindFile(strPath);

	CString fileName;
	CString DirName;

	CString PathName;

	while (bWorking)
	{
		//다음 파일 or 폴더 가 존재하면다면 TRUE 반환
		bWorking = finder.FindNextFile(); // folder 일 경우는 continue 

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

			//X파일-> 메쉬 이름만 가져옴
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

void CColliderTool::Set_TreeCollider(CTreeCtrl * pTreeCtrl, HTREEITEM h_Parants)
{
	USES_CONVERSION;
	HTREEITEM h_MeshType;
	wstring wstrBoneName;

	for (auto iter : m_mapCollider)
	{
		h_MeshType = pTreeCtrl->InsertItem(iter.first.c_str(), h_Parants, NULL);
	}
	
}

void CColliderTool::OnBnClickedAddCol()
{
	USES_CONVERSION;
	UpdateData(TRUE);

	m_tColInfo.fRadius = 0.f;
	m_tColInfo.vCenterPos = _vec3(0.f, 0.f, 0.f);
	m_fPositionX = 0.f;
	m_fPositionY = 0.f;
	m_fPositionZ = 0.f;
	m_fRadius = 0.f;

	m_pColliderObj = CColSphereMesh::Create(m_pDevice
											, m_tColInfo
											, m_wstrBoneName.c_str());
	auto iter_find = m_mapCollider.find(m_wstrColTag.GetString());
	if (iter_find != m_mapCollider.end())
		return;

	m_mapCollider.emplace(m_wstrColTag.GetString(), m_pColliderObj);

	m_wstrColTag = L"";
	m_TreeAttached.DeleteAllItems();
	Set_TreeCollider(&m_TreeAttached,NULL );
	
	UpdateData(FALSE);
}


void CColliderTool::OnBnClickedDeleteCol()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HTREEITEM hItem = m_TreeAttached.GetSelectedItem();
	CString wstrKey = m_TreeAttached.GetItemText(hItem);

	auto iter_find = m_mapCollider.find(wstrKey.GetString());

	Safe_Release(iter_find->second);
	m_mapCollider.erase(wstrKey.GetString());

	m_TreeAttached.DeleteItem(hItem);
}


void CColliderTool::OnLbnSelchangeSaveList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//세이브 리스트 클릭하면 컬라이더 생성
	UpdateData(TRUE);
	_uint iIdx = m_ListSave.GetCurSel();
	CString cstrSaveKey;
	m_ListSave.GetText(iIdx, cstrSaveKey);
	m_TreeAttached.DeleteAllItems();
	auto iter_find = m_mapMeshCollider.find(cstrSaveKey.GetString());
	m_mapCollider = iter_find->second;

	Set_TreeCollider(&m_TreeAttached, NULL);
	UpdateData(FALSE);


}


void CColliderTool::OnBnClickedAddSaveList()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_mapMeshCollider.emplace(m_cstrMeshName.GetString(), m_mapCollider);
	m_ListSave.AddString(m_cstrMeshName);
	m_TreeAttached.DeleteAllItems();
	m_pCtrlObj = nullptr;
	m_pCtrlTransform = nullptr;

	m_mapCollider.clear();	//리스트를 비워줌
	UpdateData(FALSE);
}


void CColliderTool::OnBnClickedDeleteSaveList()
{
	UpdateData(TRUE);

	_uint iIdx = m_ListSave.GetCurSel();
	CString cstrSaveKey;
	m_ListSave.GetText(iIdx, cstrSaveKey);
	
	auto iter_find = m_mapMeshCollider.find(cstrSaveKey.GetString());

	if (iter_find->second != m_mapCollider)
	{
		for_each(iter_find->second.begin(), iter_find->second.end(), CDeleteMap());
	}
	
	m_mapMeshCollider.erase(cstrSaveKey.GetString());
	m_ListSave.DeleteString(iIdx);
	UpdateData(FALSE);
}


void CColliderTool::OnBnClickedSave()
{
	CFileDialog Dlg(FALSE,
		L"dat",
		L"*.dat",
		OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	for (int i = lstrlenW(szFilePath) - 1; i >= 0; --i)
	{
		if (szFilePath[i] == '/' || szFilePath[i] == '\\')
		{
			memset(szFilePath + (i + 1), 0, sizeof(wchar_t) * (MAX_PATH - (i + 1)));
			break;
		}
	}
	lstrcat(szFilePath, L"Data");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(),
			GENERIC_WRITE,
			0,
			nullptr,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwStringSize = 0;
		DWORD dwMapSize = 0;
		DWORD dwbyte = 0;
		COLLIDERSPHERE tCol;

		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


		for (auto iter : m_mapMeshCollider)
		{
			//매시이름 
			dwStringSize = (iter.first.length() + 1) * sizeof(TCHAR);
			WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, iter.first.c_str(), dwStringSize, &dwbyte, nullptr);

			//충돌체 개수
			dwMapSize = iter.second.size();
			WriteFile(hFile, &dwMapSize, sizeof(DWORD), &dwbyte, nullptr);
			for (auto iter_second : iter.second)
			{
				// 태그이름
				dwStringSize = (iter_second.first.length() + 1) * sizeof(TCHAR);
				WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
				WriteFile(hFile, iter_second.first.c_str(), dwStringSize, &dwbyte, nullptr);

				//뼈 이름
				dwStringSize = ((dynamic_cast<CColSphereMesh*>(iter_second.second)->GetBone()).length()+1) * sizeof(TCHAR);
				WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
				WriteFile(hFile, dynamic_cast<CColSphereMesh*>(iter_second.second)->GetBone().c_str(), dwStringSize, &dwbyte, nullptr);

				//센터, 반지름
				CTransform* pTransform = dynamic_cast<CTransform*>(iter_second.second->Get_Component(L"Com_Transform", ID_DYNAMIC));
				tCol.vCenterPos =  pTransform->Get_CenterPos();
				tCol.fRadius = *dynamic_cast<CColliderSphere*>(iter_second.second->Get_Component(L"Com_Collider",ID_STATIC))->Get_Radius();
				WriteFile(hFile, &tCol, sizeof(COLLIDERSPHERE), &dwbyte, nullptr);
			}
		}
		CloseHandle(hFile);
	}


}


void CColliderTool::OnBnClickedLoad()
{
	CFileDialog Dlg(TRUE,// TRUE면 열기. 
		L"dat",
		L"*.dat",
		OFN_OVERWRITEPROMPT);

	TCHAR szFilePath[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	for (int i = lstrlenW(szFilePath) - 1; i >= 0; --i)
	{
		if (szFilePath[i] == '/' || szFilePath[i] == '\\')
		{
			memset(szFilePath + (i + 1), 0, sizeof(wchar_t) * (MAX_PATH - (i + 1)));
			break;
		}
	}
	lstrcat(szFilePath, L"Data");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwbyte = 0;
		DWORD dwStringSize = 0;
		DWORD dwMapSize = 0;
		TCHAR* pBuf = nullptr;

		TCHAR* pTagBuf = nullptr;
		TCHAR* pBoneBuf = nullptr;
		COLLIDERSPHERE tCol;
		DeleteMultiMap(m_mapMeshCollider);
		m_TreeAttached.DeleteAllItems();
		m_ListSave.ResetContent();
		while (true)
		{
			//ojb키
			ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			if (0 == dwbyte )
				break;
			pBuf = new TCHAR[dwStringSize];
			ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
			//사이즈 개수만큼 반복문 ㄱ
			ReadFile(hFile, &dwMapSize, sizeof(DWORD), &dwbyte, nullptr);
			m_ListSave.AddString(pBuf);
			for (_uint i = 0; i <dwMapSize; i++)
			{
				//태그
				ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
				pTagBuf = new TCHAR[dwStringSize];
				ReadFile(hFile, pTagBuf, dwStringSize, &dwbyte, nullptr);

				//뼈이름
				ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
				pBoneBuf = new TCHAR[dwStringSize];
				ReadFile(hFile, pBoneBuf, dwStringSize, &dwbyte, nullptr);

				ReadFile(hFile, &tCol, sizeof(COLLIDERSPHERE), &dwbyte, nullptr);

				m_pColliderObj = CColSphereMesh::Create(m_pDevice
					, tCol
					,pBoneBuf);
				dynamic_cast<CTransform*>(m_pColliderObj->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_CenterPos(&tCol.vCenterPos);

				m_mapCollider.emplace(pTagBuf, m_pColliderObj);
				Safe_Delete(pTagBuf);
				Safe_Delete(pBoneBuf);
			}

			m_mapMeshCollider.emplace(pBuf, m_mapCollider);
			m_mapCollider.clear();
			Safe_Delete(pBuf);

		}

		CloseHandle(hFile);
	}
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
		m_cstrMeshName = m_TreeMeshList.GetItemText(hItem);
		auto iter_find = m_mapReadyMesh.find(L"DynamicMesh");

		auto iter_Obj = iter_find->second.find(m_cstrMeshName.GetString());

		//가저온 키값으로 다이나믹 매시 접근해서 뼈 리스트 이즈고 
		if (m_pSelectedGameObject != nullptr)
			Safe_Release(m_pSelectedGameObject);
		m_pSelectedGameObject = CDynamicMeshObj::Create(m_pDevice, m_cstrMeshName.GetString(),false);

		D3DXFRAME* pRootFrame = dynamic_cast<CDynamicMesh*>(m_pSelectedGameObject->Get_Component(L"Com_Mesh", ID_STATIC))->GetRootFrame();

		m_TreeBoneList.DeleteAllItems();
		Set_TreeBoneName(&m_TreeBoneList, NULL,(D3DXFRAME_DERIVED*)pRootFrame);

		//애니메이션 셋팅
		_uint iAniIdex = dynamic_cast<CDynamicMesh*>(m_pSelectedGameObject->Get_Component(L"Com_Mesh", ID_STATIC))->GetMaxNumAnimationSet();

		for (_uint i = 0; i < iAniIdex; i++)
		{
			m_ListAniIdx.AddString(to_wstring(i).c_str());
		}

	}

	*pResult = 0;
}


void CColliderTool::OnNMClickBoneList(NMHDR *pNMHDR, LRESULT *pResult)
{
	POINT pt;
	GetCursorPos(&pt);
	UINT flag;
	m_TreeBoneList.ScreenToClient(&pt);
	HTREEITEM hItem = m_TreeBoneList.HitTest(pt, &flag);

	CString wstrBoneKey = m_TreeMeshList.GetItemText(hItem);
	m_wstrBoneName = wstrBoneKey.GetString();

	
	*pResult = 0;
}


void CColliderTool::OnNMClickAttachedColList(NMHDR *pNMHDR, LRESULT *pResult)
{
	//수정할 것 선택해서 저장하자 
	UpdateData(TRUE);

	POINT pt;
	GetCursorPos(&pt);
	UINT flag;
	m_TreeAttached.ScreenToClient(&pt);
	HTREEITEM hItem = m_TreeAttached.HitTest(pt, &flag);

	CString wstrColKey = m_TreeAttached.GetItemText(hItem);

	auto iter_find = m_mapCollider.find(wstrColKey.GetString());

	if (iter_find == m_mapCollider.end())
		return;

	m_pCtrlObj = iter_find->second;
	m_pCtrlTransform = dynamic_cast<CTransform*>(m_pCtrlObj->Get_Component(L"Com_Transform", ID_DYNAMIC));
	//애니메이션이 적용되고 가져옴- > 센터포즈 따로 저장하자 
	
	_vec3 vPos = (m_pCtrlTransform->Get_CenterPos());
	m_fPositionX = vPos.x;
	m_fPositionY = vPos.y;
	m_fPositionZ = vPos.z;
	m_tColInfo.vCenterPos = _vec3(m_fPositionX, m_fPositionY, m_fPositionZ);
	m_fRadius =  *(dynamic_cast<CColliderSphere*>(m_pCtrlObj->Get_Component(L"Com_Collider", ID_STATIC))->Get_Radius());
	m_tColInfo.fRadius = m_fRadius;
	m_wstrColTag = wstrColKey.GetString();
	m_bSelect = true;
	UpdateData(FALSE);
	OnEnChangePositionX();
	OnEnChangePositionY();
	OnEnChangePositionZ();

	*pResult = 0;
}


void CColliderTool::OnLbnSelchangeAnimationList()
{
	UpdateData(TRUE);
	CString cstrAniIdx;
	_uint iIdx = m_ListAniIdx.GetCurSel();

	m_ListAniIdx.GetText(iIdx, cstrAniIdx);

	dynamic_cast<CDynamicMesh*>(m_pSelectedGameObject->Get_Component(L"Com_Mesh", ID_STATIC))->Set_AnimationIndex(_ttoi(cstrAniIdx));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



void CColliderTool::OnEnChangePositionX()
{
	if (!m_pSelectedGameObject)
		return;
	UpdateData(TRUE);
	m_tColInfo.vCenterPos.x = m_fPositionX;
	_vec3 vPos = m_pCtrlTransform->Get_CenterPos();
	vPos.x = m_tColInfo.vCenterPos.x;
	m_pCtrlTransform->Set_CenterPos(&vPos);

	m_bSelect = true;
	UpdateData(FALSE);
}


void CColliderTool::OnEnChangePositionY()
{
	if (!m_pSelectedGameObject)
		return;
	UpdateData(TRUE);
	m_tColInfo.vCenterPos.y = m_fPositionY;
	_vec3 vPos = m_pCtrlTransform->Get_CenterPos();
	vPos.y = m_tColInfo.vCenterPos.y;
	m_pCtrlTransform->Set_CenterPos(&vPos);

	m_bSelect = true;
	UpdateData(FALSE);
}


void CColliderTool::OnEnChangePositionZ()
{
	if (!m_pSelectedGameObject)
		return;
	UpdateData(TRUE);
	m_tColInfo.vCenterPos.z = m_fPositionZ;
	_vec3 vPos = m_pCtrlTransform->Get_CenterPos();
	vPos.z = m_tColInfo.vCenterPos.z;
	m_pCtrlTransform->Set_CenterPos(&vPos);

	m_bSelect = true;
	UpdateData(FALSE);
}


void CColliderTool::OnEnChangeRadius()
{
	if (!m_pSelectedGameObject)
		return;
	UpdateData(TRUE);
	m_tColInfo.fRadius = m_fRadius;

	m_bSelect = true;

	UpdateData(FALSE);
}


void CColliderTool::OnDeltaposSpinPositionX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);
	if (pNMUpDown->iDelta < 0)	//->값 증가
	{
		m_fPositionX += 0.5f;
	}
	else
	{
		m_fPositionX -= 0.5f;
	}

	m_tColInfo.vCenterPos.x = m_fPositionX;
	_vec3 vPos = m_pCtrlTransform->Get_CenterPos();
	vPos.x = m_tColInfo.vCenterPos.x;
	m_pCtrlTransform->Set_CenterPos(&vPos);

	m_bSelect = true;
	UpdateData(FALSE);

	*pResult = 0;
}


void CColliderTool::OnDeltaposSpinPositionY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);
	if (pNMUpDown->iDelta < 0)	//->값 증가
	{
		m_fPositionY += 0.5f;
	}
	else
	{
		m_fPositionY -= 0.5f;
	}

	m_tColInfo.vCenterPos.y = m_fPositionY;
	_vec3 vPos = m_pCtrlTransform->Get_CenterPos();
	vPos.y = m_tColInfo.vCenterPos.y;
	m_pCtrlTransform->Set_CenterPos(&vPos);

	m_bSelect = true;
	UpdateData(FALSE);
	*pResult = 0;
}


void CColliderTool::OnDeltaposSpinPositionZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);
	if (pNMUpDown->iDelta < 0)	//->값 증가
	{
		m_fPositionZ += 0.5f;
	}
	else
	{
		m_fPositionZ -= 0.5f;
	}
	m_tColInfo.vCenterPos.z = m_fPositionZ;
	_vec3 vPos = m_pCtrlTransform->Get_CenterPos();
	vPos.z = m_tColInfo.vCenterPos.z;
	m_pCtrlTransform->Set_CenterPos(&vPos);

	UpdateData(FALSE);
	*pResult = 0;
}

void CColliderTool::OnDeltaposSpinRadius(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);
	if (pNMUpDown->iDelta < 0)	//->값 증가
	{
		m_fRadius += 0.5f;
	}
	else
	{
		m_fRadius -= 0.5f;
	}

	m_tColInfo.fRadius = m_fRadius;
	
	m_bSelect = true;
	UpdateData(FALSE);

	*pResult = 0;
}
