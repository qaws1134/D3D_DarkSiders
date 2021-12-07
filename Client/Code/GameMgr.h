#ifndef GameMgr_h__
#define GameMgr_h__

#include "Base.h"
#include "Define.h"
#include "GameObject.h"
#include "ParticleSystem.h"
#include "Struct.h"
#include "Enum.h"
//#include "NaviMesh.h"

typedef struct tagPlayerStat
{
	tagPlayerStat() :m_iPlayerAttack(100), m_iPlayerHealth(100), m_iPlayerSkill(100) {}

	_uint m_iPlayerSkill;
	_uint m_iPlayerHealth ;
	_uint m_iPlayerAttack ;

}PLAYERSTAT;

enum class SpawnSet
{
	GoblinFiledSp1,
	GoblinFiledSp2,

	GrinnerFiledSp,
	GoblinSp,
	GrinnerSp,
	End
};
enum class CamEvent
{
	Open_Dis,
	Open_Vulgrim,
	End
};

class CGameMgr : public CBase
{

	DECLARE_SINGLETON(CGameMgr)
private:
	explicit CGameMgr();
	virtual ~CGameMgr();


public:
	void				SetPlayer(CGameObject*	pPlayer) { m_pPlayer = pPlayer; }
	void				SetStatFont( CGameObject* pObj);
	void				SetPlayerNaviIdx(_uint iIdx) {m_iNaviIdx = iIdx;}
	_uint				GetPlayerNaviIdx() { return m_iNaviIdx; }

	void				SetDevice(LPDIRECT3DDEVICE9 pGraphicDev) {m_pGraphicDev = pGraphicDev;}

	CGameObject*		GetPlayer() { return m_pPlayer; }
	vector<STONE>		GetStoneVec() { return m_vecStone; }
	STONE				GetStone(UI::STONE eStone);

	void TakeStone(UI::STONE eStone);
	
	LPDIRECT3DDEVICE9	GetDevice() { return m_pGraphicDev; }

	POPTION				GetParticleInfo(PARTICLEEFF::TYPE eParticle);

	//카메라
	void CameraEvent();
	void CameraEvent( CGameObject* pTargetObj);
	void SetCamera(CGameObject* pObj) { m_pCamera = pObj; }
	CGameObject* GetCamera() { return m_pCamera; }

	//돈
	void TakeSoul(_int iSoul) {m_iSoul += iSoul; if (m_iSoul <= 0)m_iSoul = 0;}
	void SetSoul(_uint iSoul) { m_iSoul = iSoul; }
	_uint GetSoul() { return m_iSoul; }
	CGameObject* GetFontObj() { return m_pSoulFont; }
	void SetFontObj(CGameObject* pObj) { m_pSoulFont = pObj; }


	_bool	EventAngel();


	//오브젝트풀
	HRESULT InitObjPool();
	HRESULT InitStageObjPool();

	HRESULT InitEnemyBullet();
	void RetunEnemyBullet(CGameObject* pObj);
	CGameObject* GetEnemyBullet(_uint eType);


	HRESULT InitPlayerBullet();
	void RetunPlayerBullet(CGameObject* pObj);
	CGameObject* GetPlayerBullet(_uint eType);



	HRESULT InitEffect();				//리셋아직 안함
	void RetunEffect(CGameObject* pObj);
	CGameObject* GetEffect(_uint eType);


	HRESULT InitParticle();
	void RetunParticle(CGameObject* pObj);
	CGameObject* GetParticle(_uint eType);

	HRESULT InitItem();
	void RetunItem(CGameObject* pObj);
	CGameObject* GetItem(_uint eType);

	HRESULT InitWaterBossOrb();
	void RetunWaterBossOrb(CGameObject* pObj);
	CGameObject* GetWaterBossOrb();


	HRESULT InitEffect3D();
	void RetunEffect3D(CGameObject* pObj);
	CGameObject* GetEffect3D(_uint eType);

	void SpawnSet(_uint idx);


	PLAYERSTAT GetPlayerStat() { return m_tPlayerStat; }
	void AddPlayerStat(UI::STONE eStone);
	void SetTrail(CGameObject* pTrail) { m_pTrail = pTrail; }
	CGameObject* GetTrail() { return m_pTrail; }

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	//플레이어 저장 
	CGameObject* m_pPlayer = nullptr;
	CGameObject* m_pCamera = nullptr;
	CGameObject* m_pSoulFont = nullptr;
	_float m_fSoul;

	vector<STONE> m_vecStone;
	vector<ITEM>m_vecItemInfo;

	queue<CGameObject*> m_queEnemyBullet;
	queue<CGameObject*> m_quePlayerBullet;

	queue<CGameObject*> m_queEffect;
	queue<CGameObject*> m_queParticle;
	queue<CGameObject*> m_queItem;

	queue<CGameObject*> m_queWaterOrb;
	queue<CGameObject*> m_queEffect3D;

	_uint m_iBulletIdx = 0;
	_uint m_iEffectIdx = 0;
	_uint m_iParticleIdx = 0;
	_uint m_iNaviIdx = 0;
	_uint m_iItemIdx = 0;
	_uint m_iWaterBossOrb = 0;
	_uint m_iEffectIdx3D = 0;

	PLAYERSTAT m_tPlayerStat;
	vector<CGameObject*> m_vecStatFont;
	list<_uint>m_listEvent;

	_int m_iSoul = 0;

	CGameObject* m_pTrail=  nullptr;



	//CNaviMesh* m_pNaviMesh = nullptr;

private:
	virtual void Free(void) override;

};


#endif // GameMgr_h__
