#ifndef EffMgr_h__
#define EffMgr_h__

#include "Base.h"
#include "Define.h"
#include "Enum.h"
#include "Struct.h"
#include "GameObject.h"
#include "Effect.h"

class CEffMgr : public CBase
{
	DECLARE_SINGLETON(CEffMgr)
public:
	explicit CEffMgr();
	virtual ~CEffMgr(void);


public:
	void	 SpawnEff(EFFECT::TYPE eEffect);
	void	 EffectListUpdate();

	
private:
	list<CGameObject*> m_listEffect;

public:
	virtual void			Free(void);
};

#endif // UI_h__
