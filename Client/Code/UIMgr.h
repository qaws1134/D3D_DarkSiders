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
	void	InitWeaponElement(LPDIRECT3DDEVICE9 pGraphicDev);
	list<list<CGameObject*>> InitCreateUI(LPDIRECT3DDEVICE9 pGraphicDev);
	UISET Set_UISET(_vec2 vPos, _vec2 vSize, _uint iTextureNum, wstring wstrTexture, wstring wstrObjTag);
public:
	void SetActiveElementUI(_bool bActive);
	_bool GetElemetUIActive();
private:
	list<list<CGameObject*>> m_listHead;
	list<CGameObject*>	m_listWeaponElement;

public:
	virtual void			Free(void);
};

#endif // UI_h__
