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
	tagPlayerStat() :m_iPlayerAttack(10), m_iPlayerHealth(10), m_iPlayerSkill(10) {}

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


public:
	void				SetPlayer(CGameObject*	pPlayer) { m_pPlayer = pPlayer;  }

	void				SetPlayerNaviIdx(_uint iIdx) { m_iNaviIdx = iIdx; }
	_uint				GetPlayerNaviIdx() { return m_iNaviIdx; }

	void				SetDevice(LPDIRECT3DDEVICE9 pGraphicDev) { m_pGraphicDev = pGraphicDev; }

	CGameObject*		GetPlayer() { return m_pPlayer; }
	vector<STONE>		GetStoneVec() { return m_vecStone; }
	STONE				GetStone(UI::STONE eStone);
	void TakeStone(UI::STONE eStone);
	
	LPDIRECT3DDEVICE9	GetDevice() { return m_pGraphicDev; }

	POPTION				GetParticleInfo(PARTICLEEFF::TYPE eParticle);

	//ī�޶�
	void CameraEvent();
	void CameraEvent( CGameObject* pTargetObj);
	void SetCamera(CGameObject* pObj) { m_pCamera = pObj; }
	CGameObject* GetCamera() { return m_pCamera; }

	//��
	void TakeSoul(_int iSoul) {m_iSoul += iSoul; if (m_iSoul <= 0)m_iSoul = 0;}
	void SetSoul(_uint iSoul) { m_iSoul = iSoul; }
	_uint GetSoul() { return m_iSoul; }
	CGameObject* GetFontObj() { return m_pSoulFont; }
	void SetFontObj(CGameObject* pObj) { m_pSoulFont = pObj; }


	_bool	EventAngel();


	//������ƮǮ
	HRESULT InitObjPool();
	HRESULT InitStageObjPool();

	HRESULT InitEnemyBullet();
	void RetunEnemyBullet(CGameObject* pObj);
	CGameObject* GetEnemyBullet(_uint eType);


	HRESULT InitPlayerBullet();
	void RetunPlayerBullet(CGameObject* pObj);
	CGameObject* GetPlayerBullet(_uint eType);



	HRESULT InitEffect();				//���¾��� ����
	void RetunEffect(CGameObject* pObj);
	CGameObject* GetEffect(_uint eType);


	HRESULT InitParticle();
	void RetunParticle(CGameObject* pObj);
	CGameObject* GetParticle(_uint eType);

	HRESULT InitItem();
	void RetunItem(CGameObject* pObj);
	CGameObject* GetItem(_uint eType);


	void SpawnSet(_uint idx);


	PLAYERSTAT GetPlayerStat() { return m_tPlayerStat; }
	void SetPlayerStat(_uint iAtk, _uint iHp, _uint iSkill) {m_tPlayerStat.m_iPlayerAttack = iAtk; m_tPlayerStat.m_iPlayerHealth = iHp;m_tPlayerStat.m_iPlayerSkill= iSkill;}


private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	//�÷��̾� ���� 
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

	_uint m_iBulletIdx = 0;
	_uint m_iEffectIdx = 0;
	_uint m_iParticleIdx = 0;
	_uint m_iNaviIdx = 0;
	_uint m_iItemIdx = 0;


	PLAYERSTAT m_tPlayerStat;

	list<_uint>m_listEvent;

	_int m_iSoul = 0;


	//CNaviMesh* m_pNaviMesh = nullptr;

private:
	virtual void Free(void) override;

};


#endif // GameMgr_h__
