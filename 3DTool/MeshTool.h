#pragma once
#include "afxwin.h"



// CMeshTool 대화 상자입니다.
#define MESHTOOL_TIMER 101
#define MAX_NAVIVERTEX 3

class CMeshTool : public CDialog
{
	DECLARE_DYNAMIC(CMeshTool)
public:

public:
	CMeshTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMeshTool();

	const CAMERA_DESC& Get_CameraDesc(void);
	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESHTOOL };
#endif
public:
	void Release_Tools();
	void Set_Camera(class CToolCam * pToolcam);

	void Find_FolderList(CString strPath);
	void Set_CreateTree(CString strPath, HTREEITEM Parents);
	void Set_Tree(CTreeCtrl* pTreeCtrl, wstring wstrType, map<wstring, map<wstring, CGameObject*>> mapMesh);
	void Set_TreeNavi(CTreeCtrl* pTreeCtrl, wstring wstrType);
	
	void Make_NaviTri(_uint iIdx);


	//void CheckNaviMod();
	
	//피킹
	void PickNavi(RAY tRayMouse);
	void PickMove(RAY tRayMouse);

	//컬라이더
	_bool NaviCol(RAY tRay,_vec3* pOutPos, _uint* pIndex);
	_bool NaviSel(RAY tRay);// 수정할 네비매시 선택
	
	_bool MeshCol(RAY tRay, map<wstring, map<wstring, CGameObject*>> mapMesh,COMPONENTID eID);

private:
	HRESULT Ready_MeshComponent();
	HRESULT	Ready_LightInfo(void);
	void	ExpandTree(CTreeCtrl* pTreeCtrl, HTREEITEM htreeItem);
	void	DeleteMultiMap(map<wstring, map<wstring, CGameObject*>> mapDelete);
	void	DeleteMultiMap(map<_uint, map<_uint, CGameObject*>> mapDelete);
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;
	class CToolCam*			m_pToolCam = nullptr;
	class CMy3DToolView*    m_pToolView = nullptr;
	class CForm*			m_pForm = nullptr;
	CAMERA_DESC				m_tCameraDesc;

	class CGameObject*		m_pCtrlObject = nullptr;
	class CTransform*		m_pCtrlTransform = nullptr;

	CGameObject* SpawnStaticMesh(wstring wstrMeshObjKey);
	CGameObject* SpawnDynamicMesh(wstring wstrMeshObjKey);



	CButton* m_pCheck;
	_float m_fDeltaTime = 0.f;

	//MeshType, MeshKey, Mesh
	map<wstring, map<wstring, CGameObject*>>m_mapReadyMesh;

	map<wstring, map<wstring, CGameObject*>>m_mapStaticMesh;

	map<wstring, map<wstring, CGameObject*>>m_mapDynamicMesh;

	list<CGameObject*>m_listCtrlNavi;

	map<_uint,CGameObject*>m_mapCtrlNavi;

	queue<CString>	 m_queFolder;
	queue<CString>	 m_queFile;


	//네비매시
	_uint m_iNaviCount = 0;

	_vec3 m_pNaviPos[MAX_NAVIVERTEX];
	
	map<_uint, CGameObject*> m_mapNaviTri;
	map<_uint, map<_uint, CGameObject*>>m_mapNaviMesh;


	_bool	m_bCol = false;

	HTREEITEM m_hParentItem;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	CTreeCtrl m_TreeCreate;
	CTreeCtrl m_TreeStatic;
	CTreeCtrl m_TreeDynamic;
	CTreeCtrl m_TreeNavi;


	afx_msg void OnBnClickedObjStaticListDelete();
	afx_msg void OnBnClickedObjDynamicAddSection();
	afx_msg void OnBnClickedObjDynamicListDelete();
	afx_msg void OnBnClickedNaviDelete();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnNMDblclkCreate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeScaleX();
	afx_msg void OnEnChangeScaleY();
	afx_msg void OnEnChangeScaleZ();
	afx_msg void OnEnChangeRotationX();
	afx_msg void OnEnChangeRotationY();
	afx_msg void OnEnChangeRotationZ();
	afx_msg void OnEnChangePositionX();
	afx_msg void OnEnChangePositionY();
	afx_msg void OnEnChangePositionZ();
	afx_msg void OnNMClickStaticList(NMHDR *pNMHDR, LRESULT *pResult);
	_float m_fScaleX;
	_float m_fScaleY;
	_float m_fScaleZ;
	_float m_fRotationX;
	_float m_fRotationY;
	_float m_fRotationZ;
	_float m_fPositionX;
	_float m_fPositionY;
	_float m_fPositionZ;


	_vec3	m_vScale = { 0.f,0.f,0.f };
	_vec3	m_vRot = { 0.f,0.f,0.f };
	_vec3	m_vPos = { 0.f,0.f,0.f };

	afx_msg void OnDeltaposSpinScaleX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinScaleY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinScaleZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRotationX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRotationY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRotationZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinPositionX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinPositionY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinPositionZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickDynamicList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedObject();
	afx_msg void OnBnClickedNaviMesh();

	afx_msg void OnNMClickNaviList(NMHDR *pNMHDR, LRESULT *pResult);
};
