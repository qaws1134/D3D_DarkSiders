// MeshTool.cpp : 구현 파일입니다.
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
#include "DynamicMeshObj.h"

#include "NaviTri.h"

#include "ColSphereMesh.h"
#include "Terrain.h"

#include "NaviMesh.h"



// CMeshTool 대화 상자입니다.

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

	for_each(m_mapNaviTri.begin(), m_mapNaviTri.end(), CDeleteMap());
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

void CMeshTool::Set_CreateTree(CString strPath, HTREEITEM Parents)
{
	HTREEITEM h_Layer;
	HTREEITEM h_Obj;

	CString strMeshTypePath = L"../Resource/Mesh/";
	CString strAllFileDot = L"/*.*";
	CString strFolderPath;
	CString strFilePath;

	h_Layer = m_TreeCreate.InsertItem(strPath, Parents, NULL);

	//m_queFolder에 폴더명 저장 
	Find_FolderList(strMeshTypePath + strPath + strAllFileDot);

	while (m_queFolder.size() > 0)
	{
		strFolderPath = m_queFolder.front();
		m_queFolder.pop();

		h_Obj = m_TreeCreate.InsertItem(strFolderPath, h_Layer, NULL);
		//파일이 있는 폴더 경로
		strFilePath = strMeshTypePath + strPath + L"/" + strFolderPath + strAllFileDot;

		Find_FolderList(strFilePath);

		while (m_queFile.size() > 0)
		{
			//파일명 저장 
			strFilePath = m_queFile.front();
			m_queFile.pop();
			//노드 생성 
			m_TreeCreate.InsertItem(strFilePath, h_Obj, NULL);
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
			else if (strPath == L"StaticMesh")
			{
				map<wstring, CGameObject*> mapStaticProto;
				for (auto iter : m_mapReadyMesh)
				{
					for (auto mapProto : iter.second)
					{
						mapStaticProto.emplace(mapProto.first, mapProto.second);
					}
				}
				//스테틱 매시 스폰
				CGameObject* pGameObject = SpawnStaticMesh(strFilePath.GetString());
				if (!pGameObject)
					continue;
				mapStaticProto.emplace(strFilePath.GetString(), pGameObject);
				m_mapReadyMesh[strPath.GetString()] = mapStaticProto;
			}
		}
	}


}

void CMeshTool::Set_Tree(CTreeCtrl* pTreeCtrl, wstring wstrType, map<wstring, map<wstring, CGameObject*>> mapMesh)
{
	//초기화
	pTreeCtrl->DeleteAllItems();

	HTREEITEM h_MeshType;
	HTREEITEM h_Layer;
	HTREEITEM h_MeshObj;

	h_MeshType = pTreeCtrl->InsertItem(wstrType.c_str(), NULL, NULL);

	for (auto& iter : mapMesh)
	{
		h_Layer = pTreeCtrl->InsertItem(iter.first.c_str(), h_MeshType, NULL);
		for (auto map_iter : iter.second)
		{
			h_MeshObj = pTreeCtrl->InsertItem(map_iter.first.c_str(), h_Layer, NULL);
		}
	}
	ExpandTree(pTreeCtrl, h_MeshType);
}

void CMeshTool::Set_TreeNavi(CTreeCtrl* pTreeCtrl, wstring wstrType)
{
	//초기화
	pTreeCtrl->DeleteAllItems();

	HTREEITEM h_MeshType;
	HTREEITEM h_Layer;
	HTREEITEM h_MeshObj;

	h_MeshType = pTreeCtrl->InsertItem(wstrType.c_str(), NULL, NULL);

	for (auto& iter : m_mapNaviMesh)
	{
		h_Layer = pTreeCtrl->InsertItem(to_wstring(iter.first).c_str(), h_MeshType, NULL);
		for (auto map_iter : iter.second)
		{
			h_MeshObj = pTreeCtrl->InsertItem(to_wstring(map_iter.first).c_str(), h_Layer, NULL);
		}
	}
	ExpandTree(pTreeCtrl, h_MeshType);
}


void CMeshTool::Ready_MeshPrototype()
{

}
void CMeshTool::PickNavi(RAY tRayMouse)
{
	m_pCheck = (CButton*)GetDlgItem(IDC_RADIO4);
	if (m_pCheck->GetCheck())
	{
		_vec3 vPickPos;
		_matrix matInvWorld;
		D3DXMatrixIdentity(&matInvWorld);
		D3DXMatrixInverse(&matInvWorld, 0, &matInvWorld);
		D3DXVec3TransformCoord(&tRayMouse.vRayPos, &tRayMouse.vRayPos, &matInvWorld);
		D3DXVec3TransformNormal(&tRayMouse.vRayDir, &tRayMouse.vRayDir, &matInvWorld);
		//월드까지 내림?

	//	CTerrain* pTerrain = m_pToolView->m_pTerrain;
		CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(m_pToolView->m_pTerrain->Get_Component(L"Com_Buffer", ID_STATIC));

		

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

				// 오른쪽 위 삼각형

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
					vPickPos = _vec3(pTerrainVtx[dwVtxIdx[1]].x + fU * (pTerrainVtx[dwVtxIdx[0]].x - pTerrainVtx[dwVtxIdx[1]].x),
						0.f,
						pTerrainVtx[dwVtxIdx[1]].z + fV * (pTerrainVtx[dwVtxIdx[2]].z - pTerrainVtx[dwVtxIdx[1]].z));
				}

				// 왼쪽 아래 삼각형

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
					vPickPos = _vec3(pTerrainVtx[dwVtxIdx[2]].x + fU * (pTerrainVtx[dwVtxIdx[1]].x - pTerrainVtx[dwVtxIdx[2]].x),
						0.f,
						pTerrainVtx[dwVtxIdx[2]].z + fV * (pTerrainVtx[dwVtxIdx[0]].z - pTerrainVtx[dwVtxIdx[2]].z));
				}

			}
		}
		//vpickpos랑 충돌체랑 충돌 시 충돌체 pos 반환
		_vec3 vOutPos;
		if (NaviCol(vPickPos,&vOutPos))
		{
			m_pNaviPos[m_iNaviCount] = vOutPos;
		}
		else  // 충돌 안했을 시 
		{
			m_pNaviPos[m_iNaviCount] = vPickPos;
		}
		m_iNaviCount += 1;

			if (m_iNaviCount < MAX_NAVIVERTEX)
			{
				return;
			}
		//네비 카운트 초기화
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
			//피킹을 위한 collider 생성
			for (_uint i = 0; i < MAX_NAVIVERTEX; i++)
			{
				//정점을 가지고 있는 충돌체 생성
				//해당 매시 충돌 시 해당 메시의 pos 반환
				mapNaviObj.emplace(i, CColSphereMesh::Create(m_pDevice, m_pNaviPos[i]));

			}
			//수정 시 해당 메시와 동일 한 pos를 가지고 있는 네비매시를 찾음
			//해당 네비매시 전부 update

			//여기서는 점 3개만 뭉처서 넘겨주면? 로드할떄 네비매시로 찍으면 되잖아?
			//난 바본가?
			m_mapNaviMesh.emplace(iIdx, mapNaviObj);

			Make_NaviTri(iIdx);

			break;
		}
		Set_TreeNavi(&m_TreeNavi, L"NaviMesh");
	}
}

_bool CMeshTool::NaviCol(_vec3 vPickingPos, _vec3* vOutPos)
{
	for (auto iter : m_mapNaviMesh)
	{
		for (auto iter_second : iter.second)
		{
			_vec3 vPos;
			dynamic_cast<CTransform*>(iter_second.second->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_INFO(INFO_POS,&vPos);
			_float fRadius = dynamic_cast<CColSphereMesh*>(iter_second.second)->GetRafius();
			_float fSrcRadius = 0.1f;
			if (dynamic_cast<CCalculator*>(iter_second.second->Get_Component(L"Com_Calculator", ID_STATIC))->Collision_Sphere(&vPos, &fRadius, &vPickingPos, &fSrcRadius))
			{
				*vOutPos = vPos;
				return true;
			}
		}
	}
	return false;
}

void CMeshTool::Make_NaviTri(_uint iIdx)
{
	auto iter_find = m_mapNaviMesh.find(iIdx);
	_ulong Iindx = 0;
	_vec3 vPos[3];
	for (auto iter_second : iter_find->second)
	{
		dynamic_cast<CTransform*>(iter_second.second->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_INFO(INFO_POS, &vPos[Iindx]);
		Iindx++;
	}
	//요기서 점3개 받아서 
	//vpos 넘겨줌 
	m_mapNaviTri.emplace(iIdx, CNaviTri::Create(m_pDevice, vPos));

}


HRESULT CMeshTool::Ready_MeshComponent()
{

	Ready_Prototype(L"TombStone", CStaticMesh::Create(m_pDevice, L"../Resource/Mesh/StaticMesh/TombStone/", L"TombStone.x"));
	Ready_Prototype(L"Tree01", CStaticMesh::Create(m_pDevice, L"../Resource/Mesh/StaticMesh/Tree/", L"Tree01.X"));

	//	Ready_Prototype(L"Proto_Mesh_Player", CDynamicMesh::Create(m_pGraphicDev, L"../Resource/Mesh/DynamicMesh/PlayerXfile/", L"Player.x"));

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

	Ready_Light(m_pDevice, &tLightInfo, 0);

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
	pGameObject = CStaticMeshObj::Create(m_pDevice, wstrMeshObjKey);

	//예외처리

	return pGameObject;
}

CGameObject * CMeshTool::SpawnDynamicMesh(wstring wstrMeshObjKey)
{
	CGameObject* pGameObject = nullptr;
	pGameObject = CDynamicMeshObj::Create(m_pDevice, wstrMeshObjKey);

	return pGameObject;
}

void CMeshTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);


	DDX_Text(pDX, IDC_EDIT2, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT3, m_fRotationX);
	DDX_Text(pDX, IDC_EDIT4, m_fPositionX);
	DDX_Text(pDX, IDC_EDIT5, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT6, m_fRotationY);
	DDX_Text(pDX, IDC_EDIT7, m_fPositionY);
	DDX_Text(pDX, IDC_EDIT8, m_fScaleZ);
	DDX_Text(pDX, IDC_EDIT9, m_fRotationZ);
	DDX_Text(pDX, IDC_EDIT10, m_fPositionZ);

	DDX_Control(pDX, IDC_TREE2, m_TreeCreate);
	DDX_Control(pDX, IDC_TREE1, m_TreeStatic);
	DDX_Control(pDX, IDC_TREE3, m_TreeDynamic);
	DDX_Control(pDX, IDC_TREE4, m_TreeNavi);

}


BEGIN_MESSAGE_MAP(CMeshTool, CDialog)
	ON_WM_TIMER()
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
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CMeshTool::OnDeltaposSpinScaleX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CMeshTool::OnDeltaposSpinScaleY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CMeshTool::OnDeltaposSpinScaleZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CMeshTool::OnDeltaposSpinRotationX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CMeshTool::OnDeltaposSpinRotationY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CMeshTool::OnDeltaposSpinRotationZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN8, &CMeshTool::OnDeltaposSpinPositionX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN9, &CMeshTool::OnDeltaposSpinPositionY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN10, &CMeshTool::OnDeltaposSpinPositionZ)
	ON_NOTIFY(NM_CLICK, IDC_TREE3, &CMeshTool::OnNMClickDynamicList)
	ON_BN_CLICKED(IDC_RADIO3, &CMeshTool::OnBnClickedObject)
	ON_BN_CLICKED(IDC_RADIO4, &CMeshTool::OnBnClickedNaviMesh)
END_MESSAGE_MAP()


// CMeshTool 메시지 처리기입니다.


void CMeshTool::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (0 != m_pForm->m_iCurSel)
		return;

	Set_TimeDelta(L"Timer_DeltaTime");
	m_fDeltaTime = Get_TimeDelta(L"Timer_DeltaTime");

	for (auto iter : m_mapStaticMesh)
	{
		for (auto map_iter : iter.second)
		{
			map_iter.second->Update_Object(m_fDeltaTime);
		}
	}
	for (auto iter : m_mapDynamicMesh)
	{
		for (auto map_iter : iter.second)
		{
			map_iter.second->Update_Object(m_fDeltaTime);
		}
	}

	for (auto iter : m_mapNaviMesh)
	{
		for (auto map_iter : iter.second)
		{
			map_iter.second->Update_Object(m_fDeltaTime);
		}
	}

	for (auto iter : m_mapNaviTri)
	{
		iter.second->Update_Object(m_fDeltaTime);
	}


	if (m_pToolCam)
		m_pToolCam->Update_Object(m_fDeltaTime);


	//선택한 매쉬 transform Update
	if (m_pCtrlObject)
	{
		m_pCtrlTransform->Update_Component(m_fDeltaTime);
	}




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



void CMeshTool::OnBnClickedObjStaticListDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMeshTool::OnBnClickedObjDynamicAddSection()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMeshTool::OnBnClickedObjDynamicDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMeshTool::OnBnClickedNaviDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMeshTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMeshTool::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//Create더블클릭
void CMeshTool::OnNMDblclkCreate(NMHDR *pNMHDR, LRESULT *pResult)
{
	POINT pt;
	GetCursorPos(&pt);
	UINT flag;
	m_TreeCreate.ScreenToClient(&pt);
	HTREEITEM hItem = m_TreeCreate.HitTest(pt, &flag);

	// 마지막 노드인지 체크 
	if (!m_TreeCreate.ItemHasChildren(hItem))
	{
		m_pCheck = (CButton*)GetDlgItem(IDC_RADIO5);

		//스테틱 매시 
		if (m_pCheck->GetCheck())
		{
			//키값 받아옴
			CString wstrMeshKey = m_TreeCreate.GetItemText(hItem);
			//static_mesh
			//mapReadyMesh에 해당 X파일이 있는지 확인
			//파일 읽으면서 프로토타입 생성하자

			auto iter_find = m_mapReadyMesh.find(L"StaticMesh");

			auto ObjMapIter = iter_find->second.find(wstrMeshKey.GetString());

			if (ObjMapIter == iter_find->second.end())
				return;
			CString wstrIdxingMeshKey;	//인덱싱한 키값 저장
			CString wstrToken = L"_";
			_uint iIdxMesh = 0;
			while (true)
			{
				CString strIdx;
				strIdx.Format(_T("%d"), iIdxMesh);

				wstrIdxingMeshKey = wstrMeshKey + wstrToken + strIdx;

				auto iter_find = m_mapStaticMesh.find(wstrMeshKey.GetString());

				//트리를 저장하기위한 인덱싱 object 저장
				map<wstring, CGameObject*> mapMesh;
				//기존 값 복사

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

				m_mapStaticMesh[wstrMeshKey.GetString()] = mapMesh;
				Set_Tree(&m_TreeStatic, L"StaticMesh", m_mapStaticMesh);
				break;
			}
		}
		else
		{

			m_pCheck = (CButton*)GetDlgItem(IDC_RADIO6);
			if (m_pCheck->GetCheck())
			{
				//x파일 이름 
				CString wstrMeshKey = m_TreeCreate.GetItemText(hItem);
				auto iter_find = m_mapReadyMesh.find(L"DynamicMesh");

				auto ObjMapIter = iter_find->second.find(wstrMeshKey.GetString());

				if (ObjMapIter == iter_find->second.end())
					return;
				CString wstrIdxingMeshKey;	//인덱싱한 키값 저장
				CString wstrToken = L"_";
				_uint iIdxMesh = 0;
				while (true)
				{
					CString strIdx;
					strIdx.Format(_T("%d"), iIdxMesh);

					wstrIdxingMeshKey = wstrMeshKey + wstrToken + strIdx;

					auto iter_find = m_mapDynamicMesh.find(wstrMeshKey.GetString());

					//트리를 저장하기위한 인덱싱 object 저장
					map<wstring, CGameObject*> mapMesh;
					//기존 값 복사

					if (iter_find != m_mapDynamicMesh.end())
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
					mapMesh.emplace(wstrIdxingMeshKey, SpawnDynamicMesh(wstrMeshKey.GetString()));

					m_mapDynamicMesh[wstrMeshKey.GetString()] = mapMesh;
					Set_Tree(&m_TreeDynamic, L"DynamicMesh", m_mapDynamicMesh);
					break;
				}




				//Ready_Prototype(L"Proto_Mesh_Player", CDynamicMesh::Create(m_pDevice, L"../Resource/Mesh/DynamicMesh/", L"Player.x"));
			}


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
	m_vRot.x = D3DXToRadian(m_fRotationX);
	m_pCtrlTransform->Set_Rot(&m_vRot);
	UpdateData(FALSE);
}


void CMeshTool::OnEnChangeRotationY()
{
	if (!m_pCtrlTransform)
		return;
	UpdateData(TRUE);
	m_vRot.y = D3DXToRadian(m_fRotationY);
	m_pCtrlTransform->Set_Rot(&m_vRot);
	UpdateData(FALSE);
}


void CMeshTool::OnEnChangeRotationZ()
{
	if (!m_pCtrlTransform)
		return;
	UpdateData(TRUE);
	m_vRot.z = D3DXToRadian(m_fRotationZ);
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


	// 마지막 노드인지 체크 
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

		m_fScaleX = vScale.x;
		m_fScaleY = vScale.y;
		m_fScaleZ = vScale.z;

		m_fRotationX = vRot.x;
		m_fRotationY = vRot.y;
		m_fRotationZ = vRot.z;

		m_fPositionX = vPos.x;
		m_fPositionY = vPos.y;
		m_fPositionZ = vPos.z;

	}

	*pResult = 0;
	UpdateData(FALSE);
}


void CMeshTool::OnNMClickDynamicList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	POINT pt;
	GetCursorPos(&pt);
	UINT flag;
	m_TreeDynamic.ScreenToClient(&pt);
	HTREEITEM hItem = m_TreeDynamic.HitTest(pt, &flag);
	if (!m_TreeDynamic.ItemHasChildren(hItem))
	{
		CString wstrIdxKey = m_TreeDynamic.GetItemText(hItem);
		CString wstrToken = L"_";
		_int iTokenIdx = 0;
		CString wstrKey = wstrIdxKey.Tokenize(wstrToken, iTokenIdx);

		auto iter_find = m_mapDynamicMesh.find(wstrKey.GetString());

		if (iter_find == m_mapDynamicMesh.end())
			return;


		auto Objiter_find = iter_find->second.find(wstrIdxKey.GetString());

		m_pCtrlObject = Objiter_find->second;
		m_pCtrlTransform = dynamic_cast<CTransform*>(m_pCtrlObject->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));
		
		_vec3 vPos;
		_vec3 vScale;
		_vec3 vRot;

		m_pCtrlTransform->Get_INFO(INFO_POS, &vPos);
		vRot = m_pCtrlTransform->Get_Rot();
		vScale = m_pCtrlTransform->Get_Scale();

		m_fScaleX = vScale.x;
		m_fScaleY = vScale.y;
		m_fScaleZ = vScale.z;

		m_fRotationX = vRot.x;
		m_fRotationY = vRot.y;
		m_fRotationZ = vRot.z;

		m_fPositionX = vPos.x;
		m_fPositionY = vPos.y;
		m_fPositionZ = vPos.z;

	}

	*pResult = 0;
	UpdateData(FALSE);
}


void CMeshTool::OnDeltaposSpinScaleX(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!m_pCtrlTransform)
		return;
	UpdateData(TRUE);
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)	//->값 증가
	{
		m_fScaleX += 0.1f;
	}
	else		//감소
	{
		m_fScaleX -= 0.1f;
	}
	m_vScale.x = m_fScaleX;
	m_pCtrlTransform->Set_Scale(&m_vScale);
	*pResult = 0;
	UpdateData(FALSE);
}


void CMeshTool::OnDeltaposSpinScaleY(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!m_pCtrlTransform)
		return;
	UpdateData(TRUE);
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)	//->값 증가
	{
		m_fScaleY += 0.1f;
	}
	else		//감소
	{
		m_fScaleY -= 0.1f;
	}
	m_vScale.y = m_fScaleY;
	m_pCtrlTransform->Set_Scale(&m_vScale);
	*pResult = 0;
	UpdateData(FALSE);
}


void CMeshTool::OnDeltaposSpinScaleZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!m_pCtrlTransform)
		return;
	UpdateData(TRUE);
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)	//->값 증가
	{
		m_fScaleZ += 0.1f;
	}
	else		//감소
	{
		m_fScaleZ -= 0.1f;
	}
	m_vScale.z = m_fScaleZ;
	m_pCtrlTransform->Set_Scale(&m_vScale);
	*pResult = 0;
	UpdateData(FALSE);
}


void CMeshTool::OnDeltaposSpinRotationX(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!m_pCtrlTransform)
		return;
	UpdateData(TRUE);
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)	//->값 증가
	{
		m_fRotationX += m_fDeltaTime*100.f;
	}
	else		//감소
	{
		m_fRotationX -= m_fDeltaTime*100.f;
	}
	m_vRot.x = D3DXToRadian(m_fRotationX);
	m_pCtrlTransform->Set_Rot(&m_vRot);
	*pResult = 0;
	UpdateData(FALSE);
}


void CMeshTool::OnDeltaposSpinRotationY(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!m_pCtrlTransform)
		return;
	UpdateData(TRUE);
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)	//->값 증가
	{
		m_fRotationY += m_fDeltaTime*100.f;
	}
	else		//감소
	{
		m_fRotationY -= m_fDeltaTime*100.f;
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_vRot.y = D3DXToRadian(m_fRotationY);
	m_pCtrlTransform->Set_Rot(&m_vRot);
	*pResult = 0;
	UpdateData(FALSE);
}


void CMeshTool::OnDeltaposSpinRotationZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!m_pCtrlTransform)
		return;
	UpdateData(TRUE);
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)	//->값 증가
	{
		m_fRotationZ += m_fDeltaTime*100.f;
	}
	else		//감소
	{
		m_fRotationZ -= m_fDeltaTime*100.f;
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_vRot.z = D3DXToRadian(m_fRotationZ);
	m_pCtrlTransform->Set_Rot(&m_vRot);
	*pResult = 0;
	UpdateData(FALSE);
}


void CMeshTool::OnDeltaposSpinPositionX(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!m_pCtrlTransform)
		return;
	UpdateData(TRUE);
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)	//->값 증가
	{
		m_fPositionX += m_fDeltaTime;
	}
	else		//감소
	{
		m_fPositionX -= m_fDeltaTime;
	}
	m_vPos.x = m_fPositionX;
	m_pCtrlTransform->Set_Pos(&m_vPos);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(FALSE);
}


void CMeshTool::OnDeltaposSpinPositionY(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!m_pCtrlTransform)
		return;
	UpdateData(TRUE);
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)	//->값 증가
	{
		m_fPositionY += m_fDeltaTime;
	}
	else		//감소
	{
		m_fPositionY -= m_fDeltaTime;
	}
	m_vPos.y = m_fPositionY;
	m_pCtrlTransform->Set_Pos(&m_vPos);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(FALSE);
}


void CMeshTool::OnDeltaposSpinPositionZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!m_pCtrlTransform)
		return;
	UpdateData(TRUE);
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (pNMUpDown->iDelta < 0)	//->값 증가
	{
		m_fPositionZ += m_fDeltaTime;
	}
	else		//감소
	{
		m_fPositionZ -= m_fDeltaTime;
	}
	m_vPos.z = m_fPositionZ;
	m_pCtrlTransform->Set_Pos(&m_vPos);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(FALSE);
}




void CMeshTool::OnBnClickedObject()
{
	UpdateData(TRUE);
	m_pCheck = (CButton*)GetDlgItem(IDC_RADIO5);
	m_pCheck->EnableWindow(true);
	m_pCheck->SetCheck(true);
	m_pCheck = (CButton*)GetDlgItem(IDC_RADIO6);
	m_pCheck->EnableWindow(true);


	m_pCheck = (CButton*)GetDlgItem(IDC_RADIO7);
	m_pCheck->EnableWindow(false);
	m_pCheck->SetCheck(false);
	m_pCheck = (CButton*)GetDlgItem(IDC_RADIO8);
	m_pCheck->EnableWindow(false);
	m_pCheck->SetCheck(false);

	m_pCheck = nullptr;
	UpdateData(FALSE);
}


void CMeshTool::OnBnClickedNaviMesh()
{
	UpdateData(TRUE);
	m_pCheck = (CButton*)GetDlgItem(IDC_RADIO5);
	m_pCheck->EnableWindow(false);
	m_pCheck->SetCheck(false);
	m_pCheck = (CButton*)GetDlgItem(IDC_RADIO6);
	m_pCheck->EnableWindow(false);
	m_pCheck->SetCheck(false);

	m_pCheck = (CButton*)GetDlgItem(IDC_RADIO7);
	m_pCheck->EnableWindow(true);
	m_pCheck->SetCheck(true);
	m_pCheck = (CButton*)GetDlgItem(IDC_RADIO8);
	m_pCheck->EnableWindow(true);


	m_pCheck = nullptr;
	UpdateData(FALSE);
}
