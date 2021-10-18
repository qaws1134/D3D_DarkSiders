// MeshTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "3DTool.h"
#include "MeshTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "Form.h"
#include "3DToolView.h"
#include "ToolCam.h"
#include "StaticMeshObj.h"
#include "NaviTri.h"
#include "NaviMesh.h"
#include "Terrain.h"

// CMeshTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMeshTool, CDialog)

CMeshTool::CMeshTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MESHTOOL, pParent)
	, m_fScaleX(0)
	, m_fScaleY(0)
	, m_fScaleZ(0)
	, m_fRotationX(0)
	, m_fRotationY(0)
	, m_fRotationZ(0)
	, m_fPositionX(0)
	, m_fPositionY(0)
	, m_fPositionZ(0)
{

}

CMeshTool::~CMeshTool()
{
}

const CAMERA_DESC & CMeshTool::Get_CameraDesc(void)
{
	return m_tCameraDesc;
}


void CMeshTool::Release_Tools()
{
	Clear_RenderGroup();

	DeleteMultiMap(m_mapReadyMesh);
	DeleteMultiMap(m_mapStaticMesh);
	DeleteMultiMap(m_mapDynamicMesh);
	DeleteMultiMap(m_mapNaviMesh);

	KillTimer(MESHTOOL_TIMER);

	Safe_Release(m_pToolCam);
	Safe_Release(m_pDevice);
}

void CMeshTool::Set_Camera(CToolCam * pToolcam)
{
	m_pToolCam = pToolcam;
	memcpy(&m_tCameraDesc, &m_pToolCam->Get_CameraDesc(), sizeof(CAMERA_DESC));
	m_pToolCam->AddRef();
}

void CMeshTool::Find_FolderList(CString strPath)
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
			if (file == L"x"|| file == L"X")
			{
				fileName = finder.GetFileTitle();
				m_queFile.emplace(fileName);
			}
		}
		
	}
}

void CMeshTool::Set_CreateTree(CString strPath, HTREEITEM Parents)
{
	HTREEITEM h_Layer;
	HTREEITEM h_Obj;

	CString strMeshTypePath = L"../Resource/Mesh/";
	CString strAllFileDot = L"/*.*";
	CString strFolderPath;
	CString strFilePath;

	h_Layer = m_TreeCreate.InsertItem(strPath, Parents, NULL);

	//m_queFolder�� ������ ���� 
	Find_FolderList(strMeshTypePath+strPath+strAllFileDot);

	while (m_queFolder.size() > 0)
	{
		strFolderPath = m_queFolder.front();
		m_queFolder.pop();

		h_Obj = m_TreeCreate.InsertItem(strFolderPath, h_Layer, NULL);
		//������ �ִ� ���� ���
		strFilePath = strMeshTypePath+strPath +L"/"+ strFolderPath+strAllFileDot;

		Find_FolderList(strFilePath);

		while (m_queFile.size() > 0)
		{
			//���ϸ� ���� 
			strFilePath = m_queFile.front();
			m_queFile.pop();
			//��� ���� 
			m_TreeCreate.InsertItem(strFilePath, h_Obj, NULL);
			if (strPath == L"DynamicMesh")
			{

			}
			else if(strPath == L"StaticMesh")
			{
				map<wstring, CGameObject*> mapStaticProto;
				for (auto iter : m_mapReadyMesh)
				{
					for (auto mapProto : iter.second)
					{
						mapStaticProto.emplace(mapProto.first, mapProto.second);
					}
				}

				CGameObject* pGameObject = SpawnStaticMesh(strFilePath.GetString());
				if(!pGameObject)
					continue;
				mapStaticProto.emplace(strFilePath.GetString(), pGameObject);
				m_mapReadyMesh[L"StaticMesh"]= mapStaticProto;
			}
		}
	}


}

void CMeshTool::Set_Tree(CTreeCtrl* pTreeCtrl, wstring wstrType)
{
	//�ʱ�ȭ
	pTreeCtrl->DeleteAllItems();

	HTREEITEM h_MeshType;
	HTREEITEM h_Layer;
	HTREEITEM h_MeshObj;

	h_MeshType= pTreeCtrl->InsertItem(wstrType.c_str(), NULL, NULL);

	for (auto& iter : m_mapStaticMesh)
	{
		h_Layer = pTreeCtrl->InsertItem(iter.first.c_str(), h_MeshType, NULL);
		for (auto map_iter : iter.second)
		{
			h_MeshObj= pTreeCtrl->InsertItem(map_iter.first.c_str(), h_Layer, NULL);
		}
	}
	ExpandTree(pTreeCtrl, h_MeshType);
}

void CMeshTool::Ready_MeshPrototype()
{

}

void CMeshTool::CheckNaviMod()
{
	UpdateData(TRUE);
	m_pCheck = (CButton*)GetDlgItem(IDC_RADIO4);
	if (!m_pCheck->GetCheck())
	{
		m_pCheck = (CButton*)GetDlgItem(IDC_RADIO7);
		m_pCheck->EnableWindow(false);
		m_pCheck->SetCheck(false);
		m_pCheck = (CButton*)GetDlgItem(IDC_RADIO8);
		m_pCheck->EnableWindow(false);
		m_pCheck->SetCheck(false);
		return;
	}
	m_pCheck= (CButton*)GetDlgItem(IDC_RADIO7);
	m_pCheck->EnableWindow(true);
	m_pCheck = (CButton*)GetDlgItem(IDC_RADIO8);
	m_pCheck->EnableWindow(true);


	m_pCheck = nullptr;
	UpdateData(FALSE);
}

void CMeshTool::PickNavi(RAY tRayMouse)
{

	_vec3 vPickPos;
	_matrix matInvWorld;
	D3DXMatrixIdentity(&matInvWorld);
	D3DXMatrixInverse(&matInvWorld, 0, &matInvWorld);
	D3DXVec3TransformCoord(&tRayMouse.vRayPos, &tRayMouse.vRayPos, &matInvWorld);
	D3DXVec3TransformNormal(&tRayMouse.vRayDir, &tRayMouse.vRayDir, &matInvWorld);
	CTerrain* pTerrain = m_pToolView->m_pTerrain;
	CTerrainTex* pTerrainBufferCom =  dynamic_cast<CTerrainTex*>(pTerrain->Get_Component(L"Com_Buffer", ID_STATIC));



	_ulong		dwVtXCntX = VTXCNTX;
	_ulong		dwVtXCntZ = VTXCNTZ;
	const	_vec3*		pTerrainVtx = pTerrainBufferCom->Get_VtxPos();

	_float	fU, fV, fDist;
	_ulong	dwVtxIdx[3];
	for (_ulong i = 0; i < dwVtXCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwVtXCntX - 1; ++j)
		{
			_ulong dwIndex = i * dwVtXCntX + j;

			// ������ �� �ﰢ��

			dwVtxIdx[0] = dwIndex + dwVtXCntX;
			dwVtxIdx[1] = dwIndex + dwVtXCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[0]],
				&pTerrainVtx[dwVtxIdx[2]],
				&tRayMouse.vRayPos,
				&tRayMouse.vRayDir,
				&fU, &fV, &fDist))
			{
				vPickPos =  _vec3(pTerrainVtx[dwVtxIdx[1]].x + fU * (pTerrainVtx[dwVtxIdx[0]].x - pTerrainVtx[dwVtxIdx[1]].x),
					0.f,
					pTerrainVtx[dwVtxIdx[1]].z + fV * (pTerrainVtx[dwVtxIdx[2]].z - pTerrainVtx[dwVtxIdx[1]].z));
			}

			// ���� �Ʒ� �ﰢ��

			dwVtxIdx[0] = dwIndex + dwVtXCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[0]],
				&tRayMouse.vRayPos,
				&tRayMouse.vRayDir,
				&fU, &fV, &fDist))
			{
				vPickPos =  _vec3(pTerrainVtx[dwVtxIdx[2]].x + fU * (pTerrainVtx[dwVtxIdx[1]].x - pTerrainVtx[dwVtxIdx[2]].x),
					0.f,
					pTerrainVtx[dwVtxIdx[2]].z + fV * (pTerrainVtx[dwVtxIdx[0]].z - pTerrainVtx[dwVtxIdx[2]].z));
			}

		}
	}




	m_pNaviPos[m_iNaviCount] = vPickPos;
	m_iNaviCount += 1;
	
	if (m_iNaviCount < MAX_NAVIVERTEX)
	{ 
		return;
	}
	//�׺� ī��Ʈ �ʱ�ȭ
	m_iNaviCount = 0;
	_uint iIdx = 0;
	while (true)
	{
		auto& iter_find = m_mapNaviMesh.find(iIdx);
		if (iter_find != m_mapNaviMesh.end())
		{
			iIdx++;
			continue;
		}
		map<_uint, CGameObject* > mapNaviObj;
		for (_uint i = 0; i < MAX_NAVIVERTEX; i++)
		{
			mapNaviObj.emplace(i, CNaviMesh::Create(m_pDevice,m_pNaviPos[i]));
		}
		m_mapNaviMesh.emplace(iIdx, mapNaviObj);
		break;
	}
}


HRESULT CMeshTool::Ready_MeshComponent()
{

	Ready_Prototype(L"Proto_Mesh_Stone", CStaticMesh::Create(m_pDevice, L"../Resource/Mesh/StaticMesh/TombStone/", L"TombStone.x"));
	Ready_Prototype(L"Proto_Mesh_Tree01", CStaticMesh::Create(m_pDevice, L"../Resource/Mesh/StaticMesh/Tree/", L"Tree01.X"));


	return S_OK;
}

HRESULT CMeshTool::Ready_LightInfo(void)
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	Ready_Light(m_pDevice, &tLightInfo, 0) ;

	return S_OK;
}

void CMeshTool::ExpandTree(CTreeCtrl * pTreeCtrl, HTREEITEM htreeItem)
{
	htreeItem = pTreeCtrl->GetFirstVisibleItem();
	while (htreeItem != NULL)
	{
		pTreeCtrl->Expand(htreeItem, TVE_EXPAND);
		htreeItem = pTreeCtrl->GetNextItem(htreeItem, TVGN_NEXTVISIBLE);
	}
}

void CMeshTool::DeleteMultiMap(map<wstring, map<wstring, CGameObject*>> mapDelete)
{
	for (auto iterFirst : mapDelete)
	{
		for_each(iterFirst.second.begin(), iterFirst.second.end(), CDeleteMap());
	}
}
void CMeshTool::DeleteMultiMap(map<_uint, map<_uint, CGameObject*>> mapDelete)
{
	for (auto iterFirst : mapDelete)
	{
		for_each(iterFirst.second.begin(), iterFirst.second.end(), CDeleteMap());
	}
}
CGameObject* CMeshTool::SpawnStaticMesh(wstring wstrMeshObjKey)
{
	CGameObject* pGameObject = nullptr;
	CTransform* pTransform = nullptr;
	if (wstrMeshObjKey == L"TombStone")
	{
		pGameObject = CStaticMeshObj::Create(m_pDevice, L"Proto_Mesh_Stone");
	}
	if (wstrMeshObjKey == L"Tree01")
	{
		pGameObject = CStaticMeshObj::Create(m_pDevice, L"Proto_Mesh_Tree01");
		pTransform= dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", Engine::ID_STATIC));
		pTransform->Set_Scale(0.01f, 0.01f, 0.01f);
		pTransform->Update_Component(0.f);
	}

	return pGameObject;
}

void CMeshTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);


	DDX_Text(pDX,IDC_EDIT2,m_fScaleX);
	DDX_Text(pDX,IDC_EDIT3,m_fRotationX);
	DDX_Text(pDX,IDC_EDIT4,m_fPositionX);
	DDX_Text(pDX,IDC_EDIT5,m_fScaleY);
	DDX_Text(pDX,IDC_EDIT6,m_fRotationY);
	DDX_Text(pDX,IDC_EDIT7,m_fPositionY);
	DDX_Text(pDX,IDC_EDIT8,m_fScaleZ);
	DDX_Text(pDX,IDC_EDIT9,m_fRotationZ);
	DDX_Text(pDX,IDC_EDIT10,m_fPositionZ);

	DDX_Control(pDX, IDC_TREE2, m_TreeCreate);
	DDX_Control(pDX, IDC_TREE1, m_TreeStatic);
	DDX_Control(pDX, IDC_TREE3, m_TreeDynamic);
	DDX_Control(pDX, IDC_TREE4, m_TreeNavi);

}


BEGIN_MESSAGE_MAP(CMeshTool, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO1, &CMeshTool::OnBnClickedRenderSolid)
	ON_BN_CLICKED(IDC_RADIO2, &CMeshTool::OnBnClickedRenderWireFrame)
	ON_BN_CLICKED(IDC_RADIO3, &CMeshTool::OnBnClickedMouseSelectObj)
	ON_BN_CLICKED(IDC_RADIO4, &CMeshTool::OnBnClickedNaviMesh)
	ON_BN_CLICKED(IDC_RADIO5, &CMeshTool::OnBnClickedStaticObj)
	ON_BN_CLICKED(IDC_RADIO6, &CMeshTool::OnBnClickedDynamicObj)
	ON_BN_CLICKED(IDC_RADIO7, &CMeshTool::OnBnClickedNaviTogether)
	ON_BN_CLICKED(IDC_RADIO8, &CMeshTool::OnBnClickedNaviOnly)
	ON_BN_CLICKED(IDC_RADIO10, &CMeshTool::OnBnClickedGizmoScale)
	ON_BN_CLICKED(IDC_RADIO11, &CMeshTool::OnBnClickedGizmoRotation)
	ON_BN_CLICKED(IDC_RADIO9, &CMeshTool::OnBnClickedGizmoPosition)
	ON_BN_CLICKED(IDC_BUTTON1, &CMeshTool::OnBnClickedObjStaticListDelete)
	ON_BN_CLICKED(IDC_BUTTON2, &CMeshTool::OnBnClickedObjDynamicAddSection)
	ON_BN_CLICKED(IDC_BUTTON3, &CMeshTool::OnBnClickedObjDynamicDelete)
	ON_BN_CLICKED(IDC_BUTTON4, &CMeshTool::OnBnClickedNaviDelete)
	ON_BN_CLICKED(IDC_BUTTON5, &CMeshTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON6, &CMeshTool::OnBnClickedLoad)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE2, &CMeshTool::OnNMDblclkCreate)
	ON_EN_CHANGE(IDC_EDIT2, &CMeshTool::OnEnChangeScaleX)
	ON_EN_CHANGE(IDC_EDIT5, &CMeshTool::OnEnChangeScaleY)
	ON_EN_CHANGE(IDC_EDIT8, &CMeshTool::OnEnChangeScaleZ)
	ON_EN_CHANGE(IDC_EDIT3, &CMeshTool::OnEnChangeRotationX)
	ON_EN_CHANGE(IDC_EDIT6, &CMeshTool::OnEnChangeRotationY)
	ON_EN_CHANGE(IDC_EDIT9, &CMeshTool::OnEnChangeRotationZ)
	ON_EN_CHANGE(IDC_EDIT4, &CMeshTool::OnEnChangePositionX)
	ON_EN_CHANGE(IDC_EDIT7, &CMeshTool::OnEnChangePositionY)
	ON_EN_CHANGE(IDC_EDIT10, &CMeshTool::OnEnChangePositionZ)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CMeshTool::OnNMClickStaticList)
END_MESSAGE_MAP()


// CMeshTool �޽��� ó�����Դϴ�.


void CMeshTool::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (1 != m_pForm->m_iCurSel)
		return;

	Set_TimeDelta(L"Timer_DeltaTime");
	_float fDeltaTime = Get_TimeDelta(L"Timer_DeltaTime");

	for (auto iter : m_mapStaticMesh)
	{
		for (auto map_iter : iter.second)
		{
			map_iter.second->Update_Object(fDeltaTime);
		}
	}


	for (auto iter : m_mapNaviMesh)
	{
		for (auto map_iter : iter.second)
		{
			map_iter.second->Update_Object(fDeltaTime);
		}
	}



	if(m_pToolCam)
		m_pToolCam->Update_Object(fDeltaTime);


	//������ �Ž� transform Update
	if (m_pCtrlObject)
	{
		m_pCtrlTransform->Update_Component(fDeltaTime);
	}

	//�׺��� üũ 
	CheckNaviMod();


	CDialog::OnTimer(nIDEvent);
}


BOOL CMeshTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pDevice = GetDevice();
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->AddRef();

	SetRaycastDevice(m_pDevice);


	CMainFrame*	pFrame = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	m_pToolView = dynamic_cast<CMy3DToolView*>(pFrame->m_tMainSplitter.GetPane(0, 0));
	m_pForm = dynamic_cast<CForm*>(pFrame->m_tMainSplitter.GetPane(0, 1));

	Ready_MeshComponent();
	Ready_LightInfo();
	SetTimer(1, 1000 / 60000, NULL);


	CGameObject*			pGameObject = nullptr;

	//pGameObject = CStaticMeshObj::Create(m_pDevice, L"Proto_Mesh_Stone");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//m_mapStaticMesh.emplace(L"Stone", pGameObject);

	//pGameObject = CStaticMeshObj::Create(m_pDevice, L"Proto_Mesh_Tree01");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//m_mapStaticMesh.emplace(L"Tree01", pGameObject);



	HTREEITEM h_Mesh;
	h_Mesh = m_TreeCreate.InsertItem(_T("Mesh"), NULL, NULL);
	Set_CreateTree(L"DynamicMesh", h_Mesh);
	Set_CreateTree(L"StaticMesh", h_Mesh);


	//Render_State
	m_pCheck = (CButton*)GetDlgItem(IDC_RADIO1);
	m_pCheck->SetCheck(TRUE);

	//MouseSelectType
	m_pCheck = (CButton*)GetDlgItem(IDC_RADIO3);
	m_pCheck->SetCheck(TRUE);

	//Obj_Type
	m_pCheck = (CButton*)GetDlgItem(IDC_RADIO5);
	m_pCheck->SetCheck(TRUE);

	//MouseSelectType
	m_pCheck = (CButton*)GetDlgItem(IDC_RADIO12);
	m_pCheck->SetCheck(TRUE);

	m_pCheck = nullptr;

	return TRUE; 	 
}


void CMeshTool::OnBnClickedRenderSolid()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnBnClickedRenderWireFrame()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnBnClickedMouseSelectObj()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnBnClickedNaviMesh()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnBnClickedStaticObj()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnBnClickedDynamicObj()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnBnClickedNaviTogether()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnBnClickedNaviOnly()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnBnClickedGizmoScale()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnBnClickedGizmoRotation()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnBnClickedGizmoPosition()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnBnClickedObjStaticListDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnBnClickedObjDynamicAddSection()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnBnClickedObjDynamicDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CMeshTool::OnBnClickedNaviDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTool::OnBnClickedLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

//Create����Ŭ��
void CMeshTool::OnNMDblclkCreate(NMHDR *pNMHDR, LRESULT *pResult)
{
	POINT pt;
	GetCursorPos(&pt);
	UINT flag;
	m_TreeCreate.ScreenToClient(&pt);
	HTREEITEM hItem = m_TreeCreate.HitTest(pt, &flag);

	// ������ ������� üũ 
	if (!m_TreeCreate.ItemHasChildren(hItem))
	{
		m_pCheck = (CButton*)GetDlgItem(IDC_RADIO5);
		if (m_pCheck->GetCheck())
		{
			//Ű�� �޾ƿ�
			CString wstrMeshKey = m_TreeCreate.GetItemText(hItem);
			//static_mesh
			//mapReadyMesh�� �ش� X������ �ִ��� Ȯ��
			//���� �����鼭 ������Ÿ�� ��������
			
			auto iter_find = m_mapReadyMesh.find(L"StaticMesh");

			auto ObjMapIter =  iter_find->second.find(wstrMeshKey.GetString());

			if (ObjMapIter == iter_find->second.end())
				return;
			CString wstrIdxingMeshKey;	//�ε����� Ű�� ����
			CString wstrToken = L"_";
			_uint iIdxMesh = 0;
			while (true)
			{
				CString strIdx;
				strIdx.Format(_T("%d"), iIdxMesh);

				wstrIdxingMeshKey = wstrMeshKey + wstrToken + strIdx;

				auto iter_find = m_mapStaticMesh.find(wstrMeshKey.GetString());

				//Ʈ���� �����ϱ����� �ε��� object ����
				map<wstring, CGameObject*> mapMesh;
				//���� �� ����

				if (iter_find != m_mapStaticMesh.end())
				{
					for (auto iter : iter_find->second)
					{
						mapMesh.emplace(iter.first, iter.second);
					}
				}


				while (true)
				{
					auto find_Key = mapMesh.find(wstrIdxingMeshKey.GetString());
					if (find_Key != mapMesh.end())
					{
						iIdxMesh++;
						CString strIdx;
						strIdx.Format(_T("%d"), iIdxMesh);
						wstrIdxingMeshKey = wstrMeshKey + wstrToken + strIdx;
						continue;
					}
					break;
				}
				mapMesh.emplace(wstrIdxingMeshKey, SpawnStaticMesh(wstrMeshKey.GetString()));
		
				m_mapStaticMesh[wstrMeshKey.GetString()] =mapMesh;
				Set_Tree(&m_TreeStatic, L"StaticMesh");
				break;
			}
		}
		else
		{
			//dynamic_mesh

		}
		m_pCheck = nullptr;
	}
	*pResult = 0;
}

//
void CMeshTool::OnEnChangeScaleX()
{
	if (!m_pCtrlTransform)
		return;
	UpdateData(TRUE);
	m_vScale.x = m_fScaleX;
	m_pCtrlTransform->Set_Scale(&m_vScale);

	UpdateData(FALSE);
}

void CMeshTool::OnEnChangeScaleY()
{
	if (!m_pCtrlTransform)
		return;
	UpdateData(TRUE);
	m_vScale.y = m_fScaleY;
	m_pCtrlTransform->Set_Scale(&m_vScale);
	UpdateData(FALSE);
}


void CMeshTool::OnEnChangeScaleZ()
{
	if (!m_pCtrlTransform)
		return;
	UpdateData(TRUE);
	m_vScale.z = m_fScaleZ;
	m_pCtrlTransform->Set_Scale(&m_vScale);
	UpdateData(FALSE);
}


void CMeshTool::OnEnChangeRotationX()
{
	if (!m_pCtrlTransform)
		return;
	UpdateData(TRUE);
	m_vRot.x = m_fRotationX;
	m_pCtrlTransform->Set_Rot(&m_vRot);
	UpdateData(FALSE);
}


void CMeshTool::OnEnChangeRotationY()
{
	if (!m_pCtrlTransform)
		return;
	UpdateData(TRUE);
	m_vRot.y = m_fRotationY;
	m_pCtrlTransform->Set_Rot(&m_vRot);
	UpdateData(FALSE);
}


void CMeshTool::OnEnChangeRotationZ()
{
	if (!m_pCtrlTransform)
		return;
	UpdateData(TRUE);
	m_vRot.z = m_fRotationZ;
	m_pCtrlTransform->Set_Rot(&m_vRot);
	UpdateData(FALSE);
}


void CMeshTool::OnEnChangePositionX()
{
	if (!m_pCtrlTransform)
		return;

	UpdateData(TRUE);
	m_vPos.x = m_fPositionX;
	m_pCtrlTransform->Set_Pos(&m_vPos);
	UpdateData(FALSE);
}


void CMeshTool::OnEnChangePositionY()
{
	if (!m_pCtrlTransform)
		return;
	UpdateData(TRUE);
	m_vPos.y = m_fPositionY;
	m_pCtrlTransform->Set_Pos(&m_vPos);
	UpdateData(FALSE);
}

void CMeshTool::OnEnChangePositionZ()
{
	if (!m_pCtrlTransform)
		return;
	UpdateData(TRUE);
	m_vPos.z = m_fPositionZ;
	m_pCtrlTransform->Set_Pos(&m_vPos);
	UpdateData(FALSE);
}


void CMeshTool::OnNMClickStaticList(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	POINT pt;
	GetCursorPos(&pt);
	UINT flag;
	m_TreeStatic.ScreenToClient(&pt);
	HTREEITEM hItem = m_TreeStatic.HitTest(pt, &flag);


	// ������ ������� üũ 
	if (!m_TreeStatic.ItemHasChildren(hItem))
	{
		CString wstrIdxKey = m_TreeStatic.GetItemText(hItem);
		CString wstrToken = L"_";
		_int iTokenIdx = 0;
		CString wstrKey = wstrIdxKey.Tokenize(wstrToken, iTokenIdx);

		auto iter_find = m_mapStaticMesh.find(wstrKey.GetString());

		if (iter_find == m_mapStaticMesh.end())
			return;

		auto Objiter_find = iter_find->second.find(wstrIdxKey.GetString());

		m_pCtrlObject = Objiter_find->second;
		m_pCtrlTransform = dynamic_cast<CTransform*>(m_pCtrlObject->Get_Component(L"Com_Transform", Engine::ID_STATIC));
		
		_vec3 vPos;
		_vec3 vScale;
		_vec3 vRot;

		m_pCtrlTransform->Get_INFO(INFO_POS, &vPos);
		vRot = m_pCtrlTransform->Get_Rot();
		vScale = m_pCtrlTransform->Get_Scale();

		m_fScaleX	= vScale.x;
		m_fScaleY	= vScale.y;
		m_fScaleZ	= vScale.z;

		m_fRotationX= vRot.x;
		m_fRotationY= vRot.y;
		m_fRotationZ= vRot.z;

		m_fPositionX= vPos.x;
		m_fPositionY= vPos.y;
		m_fPositionZ= vPos.z;

	}

	*pResult = 0;
	UpdateData(FALSE);
}
