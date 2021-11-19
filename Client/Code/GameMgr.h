#ifndef GameMgr_h__
#define GameMgr_h__

#include "Base.h"
#include "Define.h"
#include "GameObject.h"
#include "ParticleSystem.h"
#include "Struct.h"
#include "Enum.h"

class CGameMgr : public CBase
{

	DECLARE_SINGLETON(CGameMgr)
private:
	explicit CGameMgr();
	virtual ~CGameMgr();

public:
	void				SetPlayer(CGameObject*	pPlayer) { m_pPlayer = pPlayer; }
	void				SetDevice(LPDIRECT3DDEVICE9 pGraphicDev) { m_pGraphicDev = pGraphicDev; }
	

	CGameObject*		GetPlayer() { return m_pPlayer; }
	vector<STONE>		GetStoneVec() { return m_vecStone; }
	STONE				GetStone(UI::STONE eStone);
	void TakeStone(UI::STONE eStone);
	
	LPDIRECT3DDEVICE9	GetDevice() { return m_pGraphicDev; }

	POPTION				GetParticleInfo(PARTICLEEFF::TYPE eParticle);


	//오브젝트풀
	HRESULT InitObjPool();

	HRESULT InitBullet();
	void RetunBullet(CGameObject* pObj);
	CGameObject* GetBullet(_uint eType);

	HRESULT InitEffect();				//리셋아직 안함
	void RetunEffect(CGameObject* pObj);
	CGameObject* GetEffect(_uint eType);


	HRESULT InitParticle();
	void RetunParticle(CGameObject* pObj);
	CGameObject* GetParticle(_uint eType);
private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	//플레이어 저장 
	CGameObject* m_pPlayer = nullptr;

	_float m_fSoul;

	vector<STONE> m_vecStone;
	vector<ITEM>m_vecItemInfo;

	queue<CGameObject*> m_queBullet;
	queue<CGameObject*> m_queEffect;
	queue<CGameObject*> m_queParticle;

	_uint m_iBulletIdx = 0;
	_uint m_iEffectIdx = 0;
	_uint m_iParticleIdx = 0;
private:
	virtual void Free(void) override;

};


#endif // GameMgr_h__
