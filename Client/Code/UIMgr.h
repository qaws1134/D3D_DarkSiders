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
	void	InitStoneList(LPDIRECT3DDEVICE9 pGraphicDev);

#pragma region Init

	map<UI::LISTKEY, list<CGameObject*>> InitCreateUI(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	UISET Set_UISET(_vec2 vPos, _vec2 vSize, _uint iTextureNum, wstring wstrTexture, wstring wstrObjTag);
	_vec2	GetCoretreePos(_uint iIdx);

	void SetStoneInfoUI(LPDIRECT3DDEVICE9 pGraphicDev,STONE tStone);
	void SetStoneListUI(LPDIRECT3DDEVICE9 pGraphicDev, UI::STONE eStone);


public:
#pragma region Set
	void SetActiveElementUI(_bool bActive);
	void SetActiveCoreTreeUI(_bool bActive);
#pragma region Set
public:
#pragma region Get
	_bool GetElemetUIActive();
	_bool GetCoreTreeUIActive();
#pragma endregion Get



private:
	map<UI::LISTKEY,list<CGameObject*>> m_mapHead;

	list<CGameObject*>	m_listWeaponElement;
	list<CGameObject*>	m_listCoreTree;
	list<CGameObject*>	m_listCoreList;
	list<CGameObject*>	m_listPlayerInfo;
	list<CGameObject*>	m_listStoneInfo;
	list<CGameObject*>	m_listStoneList;

	map<_uint, list<CGameObject*>>m_mapStoneList;
	map<_uint, list<CGameObject*>>m_mapStoneInfo;
	_uint m_StoneListIdx = 0;
	
public:
	virtual void			Free(void);
};

#endif // UI_h__
