#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CColliderTool 대화 상자입니다.

#define COLLIDERTOOL_TIMER 100


class CColliderTool : public CDialog
{
	DECLARE_DYNAMIC(CColliderTool)

public:
	CColliderTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CColliderTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLLIDERTOOL };
#endif

public:
	void Release_Tools();
	void Set_Camera(class CToolCam * pToolcam);
	const CAMERA_DESC& Get_CameraDesc(void);

	void CreateTree(CString strPath, HTREEITEM Parents);
	void Find_FolderList(CString strPath);

	void Set_TreeBoneName(CTreeCtrl* pTreeCtrl, HTREEITEM h_Parants, D3DXFRAME_DERIVED* pFrame);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;
	class CToolCam*			m_pToolCam = nullptr;
	class CMy3DToolView*    m_pToolView = nullptr;
	class CForm*			m_pForm = nullptr;
	CAMERA_DESC				m_tCameraDesc;
	_float m_fDeltaTime = 0.f;

	queue<CString>	 m_queFolder;
	queue<CString>	 m_queFile;

	CGameObject* m_pSelectedGameObject = nullptr;

	map<wstring, map<wstring, CGameObject*>>m_mapReadyMesh;
	//map<const char*, map<_ulong, const char*>>m_mapBoneName;

	//1.해당 매시이름 , 뼈 이름, 컬라이더 정보
	map<wstring, map<_ulong, COLLIDERSPHERE>>m_mapCollider;

	COLLIDERSPHERE m_tColInfo;

	CGameObject* SpawnDynamicMesh(wstring wstrMeshObjKey);
	HRESULT Ready_LightInfo();
	void DeleteMultiMap(map<wstring, map<wstring, CGameObject*>> mapDelete);


	

	void	ExpandTree(CTreeCtrl* pTreeCtrl, HTREEITEM htreeItem);


public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CTreeCtrl m_TreeMeshList;
	CTreeCtrl m_TreeBoneList;
	CTreeCtrl m_TreeAttached;
	CString m_wstrColTag;
	_float m_fPositionX;
	_float m_fPositionY;
	_float m_fPositionZ;
	_float m_fRadius;
	CListBox m_ListAniIdx;
	CListBox m_ListSave;
	afx_msg void OnBnClickedAddCol();
	afx_msg void OnBnClickedDeleteCol();
	afx_msg void OnDeltaposSpinPositionX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinPositionY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinPositionZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAddSaveList();
	afx_msg void OnBnClickedDeleteSaveList();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkMeshList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickBoneList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickAttachedColList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLbnSelchangeAnimationList();
	afx_msg void OnLbnSelchangeSaveList();
};
