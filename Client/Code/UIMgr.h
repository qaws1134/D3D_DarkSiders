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
	void	InitCoretreeElement(LPDIRECT3DDEVICE9 pGraphicDev);
#pragma region Init

	list<list<CGameObject*>> InitCreateUI(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	UISET Set_UISET(_vec2 vPos, _vec2 vSize, _uint iTextureNum, wstring wstrTexture, wstring wstrObjTag);
	_vec2	GetCoretreePos(_uint iIdx);

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
	list<list<CGameObject*>> m_listHead;
	list<CGameObject*>	m_listWeaponElement;
	list<CGameObject*>	m_listCoreTree;
public:
	virtual void			Free(void);
};

#endif // UI_h__
