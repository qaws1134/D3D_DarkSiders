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
	void	 SpawnEff(EFFECTSET::TYPE eEffect, _vec3 vPos);
	void	 EffectListUpdate();

	
	_float m_fStartTimeBegin = 0.f; 
	_float m_fStartTimeEnd= 0.f;

	_float m_fStartSpeed = 0.f;
	_bool	m_bRandBegin = false;
	_bool  m_bRandomPos =false;



private:
	list<EFFECT::TYPE2D> m_listReadyEff2D;
	list<EFFECT::TYPE3D> m_listReadyEff3D;

	list<CGameObject*> m_listEffect;
	EFFECTSET::TYPE m_eNextEff;
	EFFECT::TYPE2D m_eEff2D;
	EFFECT::TYPE2D m_eEff3D;

public:
	virtual void			Free(void);
};

#endif // UI_h__
