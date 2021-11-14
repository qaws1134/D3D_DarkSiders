#ifndef UIMgr_h__
#define UIMgr_h__

#include "Base.h"
#include "Define.h"
#include "Enum.h"
#include "Struct.h"
#include "GameObject.h"
#include "UI.h"

class CUIMgr : public CBase
{
	DECLARE_SINGLETON(CUIMgr)
public:
	explicit CUIMgr();
	virtual ~CUIMgr(void);

public:
#pragma region Init
	void	InitWeaponElement(LPDIRECT3DDEVICE9 pGraphicDev);
	void	InitCoretree(LPDIRECT3DDEVICE9 pGraphicDev);
	void	InitCoreList(LPDIRECT3DDEVICE9 pGraphicDev);
	void	InitPlayerInfo(LPDIRECT3DDEVICE9 pGraphicDev);
	void	InitToast(LPDIRECT3DDEVICE9 pGraphicDev);
	void	InitToastInfo(LPDIRECT3DDEVICE9 pGraphicDev);
	void	InitStore(LPDIRECT3DDEVICE9 pGraphicDev);
	void	InitStoreList(LPDIRECT3DDEVICE9 pGraphicDev,list<CGameObject*> listStore, UI::ITEM eItemIdx);


#pragma region Init

	map<UI::LISTKEY, list<CGameObject*>> InitCreateUI(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	UISET Set_UISET(_vec2 vPos, _vec2 vSize, _uint iTextureNum, wstring wstrTexture, wstring wstrObjTag);
	_vec2	GetCoretreePos(_uint iIdx);


public :
	void MoveStoneList(_float fTimeDelta, _float fSpeed);
	void MoveStoreActiveList(_float fTimeDelta, _float fSpeed);
	void MoveStoreStoneList(_float fTimeDelta, _float fSpeed);

public:


	//void EquipStone(_uint iStone);
public:
#pragma region Set Active
	void SetActiveElementUI(_bool bActive);
	void SetActiveCoreTreeUI(_bool bActive);
	void SetActivePlayerInfo(_bool bActive);
	void SetActiveStoneListUI(_bool bActive);
	void SetActiveStoneInfoUI(_bool bActive, _uint iStoneIdx);
	void SetActiveStoreStoneUI(_bool bActive);
	void SetActiveStoreActiveUI(_bool bActive);
	void SetActiveToastBoxUI(_bool bActive);
	void SetActiveToastInfoUI(_bool bActive,_uint iStoneIdx);
	void SetActiveToastMsgItemInfo(_uint iSelIdx);


#pragma region Set Active

#pragma region Set
	void SetCoreSelIdx(_uint iCoreIdx) { m_iCoreIdx = iCoreIdx; }
	void SetPreStoneIdx(_uint iPreIdx) { m_iPreStoneIdx = iPreIdx; }
	void SetStoreGetIdx(_uint iStoreGetIdx) { m_iStoreGetIdx = iStoreGetIdx; }
#pragma region Set

	//아이템 정보 셋팅
	void SetItemInfoList(LPDIRECT3DDEVICE9 pGraphicDev, UI::ITEM eItemIdx, list<CGameObject*>& listItemInsert);

	void SetStoneInfoUI(LPDIRECT3DDEVICE9 pGraphicDev,STONE tStone);
	void SetStoneListUI(LPDIRECT3DDEVICE9 pGraphicDev, STONE tStone);
	
	//void Set_SelIndex(_uint iIdx) { m_iSelIdx = iIdx; }
public:
#pragma region Get
	_bool GetElemetUIActive();
	_bool GetCoreTreeUIActive();
	
	_bool GetStoneInfoUIActive(_uint iStoneIdx);
	_bool GetStoreUIActive();
	_bool GetToastUIActive();

	list<CGameObject*> GetStoneSelIdxList(wstring wstrObjtag,_uint* iStoneIdx);
	list<CGameObject*> GetItemActiveSelIdxList(wstring wstrObjtag, _uint* iStoneIdx);
	list<CGameObject*> GetItemStoneSelIdxList(wstring wstrObjtag, _uint* iStoneIdx);


	_uint GetWheelMove();
	_uint GetPreStoneIdx() { return m_iPreStoneIdx; }



#pragma endregion Get




private:
	map<UI::LISTKEY,list<CGameObject*>> m_mapHead;

	//무기 속성
	list<CGameObject*>	m_listWeaponElement;
	//플레이어 인포
	list<CGameObject*>	m_listPlayerInfo;
	//코어트리
	list<CGameObject*>	m_listCoreTree;
	list<CGameObject*>	m_listCoreList;
	//돌
	list<CGameObject*>	m_listStoneInfo;
	list<CGameObject*>	m_listStoneList;
	
	map<_uint, list<CGameObject*>>m_mapStoneList;
	map<_uint, list<CGameObject*>>m_mapStoneInfo;


	//토스메시지
	list<CGameObject*>	m_listToastMsg;
	list<CGameObject*>	m_listToastInfo;
	map <_uint, list<CGameObject*>>m_mapToastInfo;

	//상점
	list<CGameObject*>	m_listStoreBase;
	list<CGameObject*>	m_listStoreActiveList;
	list<CGameObject*>	m_listStoreStoneList;




	map<_uint, list<CGameObject*>> m_mapStoreActiveList;
	map<_uint, list<CGameObject*>> m_mapStoreStoneList;
	



	UI::STONE	m_eStone;

	_uint m_StoneListIdx = 0;
	_uint m_iStoneSelIdx = 0;
	
	//선택한 코어값 저장 
	_uint m_iCoreIdx = 0;
	_uint m_iStoreGetIdx = 0;

	_float	m_fPreWheel = 0.f;
	_uint	m_iPreStoneIdx = 0;

	//LPDIRECT3DDEVICE9 m_pGraphicDev;



	_uint m_iStoreIdx = 0;


public:
	virtual void			Free(void);
};

#endif // UI_h__
