#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CColliderTool ��ȭ �����Դϴ�.

#define COLLIDERTOOL_TIMER 100


class CColliderTool : public CDialog
{
	DECLARE_DYNAMIC(CColliderTool)

public:
	CColliderTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CColliderTool();

// ��ȭ ���� �������Դϴ�.
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
	void Set_TreeCollider(CTreeCtrl* pTreeCtrl, HTREEITEM h_Parants);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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

	
	CGameObject* m_pSelectedGameObject = nullptr;	//���õ� ���̳��� �޽� ������Ʈ 
	CGameObject* m_pColliderObj = nullptr;			//�浹ü ������Ʈ 
	CGameObject* m_pCtrlObj = nullptr;
	CTransform* m_pCtrlTransform = nullptr;


	CString		m_cstrMeshName;						//���õ� �޽� �̸�
	const char*	m_pBoneName = nullptr;				//���õ� �� �̸�
	wstring 	m_wstrBoneName ;

	map<wstring, map<wstring, CGameObject*>>m_mapReadyMesh;

	//1.�±� , ��, obj
	map<wstring, CGameObject*> m_mapCollider;

	
	//�޽ÿ� ���� �浹ümap 
	map<wstring, map<wstring, CGameObject*>> m_mapMeshCollider;

	_bool m_bSelect = false;

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
	afx_msg void OnEnChangePositionX();
	afx_msg void OnEnChangePositionY();
	afx_msg void OnEnChangePositionZ();
	afx_msg void OnEnChangeRadius();
	afx_msg void OnDeltaposSpinRadius(NMHDR *pNMHDR, LRESULT *pResult);
};
