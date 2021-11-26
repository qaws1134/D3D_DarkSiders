#include "stdafx.h"
#include "GameMgr.h"
#include "UIMgr.h"
#include "Bullet.h"
#include "Effect.h"
#include "ParticleSystem.h"
#include "EffMgr.h"
#include "Item.h"
#include "SpawnMgr.h"
#include "Grinner.h"
#include "Goblin.h"
#include "Angel.h"
#include "StaticCamera.h"
#include "WaterBoss.h"
#include "Export_Function.h"


IMPLEMENT_SINGLETON(CGameMgr)

CGameMgr::CGameMgr()
{
	m_vecStone.reserve(22);
	CGameObject* pGameObject = nullptr;

}

CGameMgr::~CGameMgr(void)
{
	Free();
}


void CGameMgr::TakeStone(UI::STONE eStone)
{
	//시간남으면 
	//특수효과  무적, 바닥에 지속딜 용암 생성, 크리티컬확률, 이동속도 추가  
	STONE tStone = GetStone(eStone);

	m_vecStone.emplace_back(tStone);
}

POPTION CGameMgr::GetParticleInfo(PARTICLEEFF::TYPE eParticle)
{
	POPTION tOption;
	switch (eParticle)
	{
	case PARTICLEEFF::PARTICLE_LIGHTNING:
		tOption.bColor = false;
		tOption.bLifeTime = true;
		tOption.bSpeed = true;
		tOption.fStartLifeTime = 0.f;
		tOption.fEndLifeTime = 5.f;
		tOption.bLoop = true;
		tOption.eType = ShapeType::CONE;
		tOption.fAngle = 90.f;
		tOption.fArc = 360.f;
		tOption.fDuration = 0.f;
		tOption.fStartSpeed = 390.f;
		tOption.fEndSpeed = 400.f;
		tOption.fGravity = 1.f;
		tOption.fRadius = 0.3f;
		tOption.fSize = 0.5f;
		tOption.iAmount = 100;
		tOption.iBatchSize = 512;
		tOption.vStartColor = D3DXCOLOR{ 0.2f, 0.2f, 1.f, 1.f };
		tOption.fStartTime = 100.f;
		tOption.fSizeSpeed = 1.f;
		tOption.bStartDraw = true;
		tOption.fEndTimer = 3.f;
		break;
	case PARTICLEEFF::PARTICLE_END:
		break;
	default:
		break;
	}



	return tOption;
}

STONE CGameMgr::GetStone(UI::STONE eStone)
{
	STONE tStone;
	switch (eStone)
	{
	case UI::GOBLERIN:
		tStone = { eStone, L"고블린",L"스킬 딜 +1 ",UI::STONE_SKILL,false };
		break;
	case UI::STATUE:
		tStone = { eStone,L"석상",L"스킬 딜 +1",UI::STONE_SKILL,true };
		break;
	case UI::KNIGHT:
		tStone = { eStone, L"기사",L"스킬 딜 +1",UI::STONE_SKILL,false };
		break;
	case UI::ICEMAGE:
		tStone = { eStone,L"얼음고양이",L"스킬 딜 +1",UI::STONE_SKILL,false };
		break;
	case UI::SKULL:
		tStone = { eStone, L"구울",L"스킬 딜 +1",UI::STONE_SKILL,false };
		break;
	case UI::CRIBO:
		tStone = { eStone,L"크리보",L"스킬 딜 +1",UI::STONE_SKILL,true };
		break;
	case UI::LARVA:
		tStone = { eStone, L"애벌래",L"스킬 딜 +1",UI::STONE_SKILL,false };
		break;
	case UI::ANT:
		tStone = { eStone,L"불개미",L"공격 딜 +1",UI::STONE_ATK,false };
		break;
	case UI::BLUEANGEL:
		tStone = { eStone,L"천사",L"공격 딜 +1",UI::STONE_ATK,true };
		break;
	case UI::GREMLIN1:
		tStone = { eStone,L"그램린",L"공격 딜 +1",UI::STONE_ATK,false };
		break;
	case UI::GREMLIN2:
		tStone = { eStone,L"강화 그램린",L"공격 딜 + 5",UI::STONE_ATK,false };
		break;
	case UI::GREENGOBLE:
		tStone = { eStone,L"초록 고블린",L"공격 딜 + 5",UI::STONE_ATK,false };
		break;
	case UI::SKULLMAGE:
		tStone = { eStone, L"해골 마법사",L"공격 딜 + 5",UI::STONE_ATK,false };
		break;
	case UI::GRINNER:
		tStone = { eStone, L"그리너",L"공격 딜 + 5",UI::STONE_ATK,true };
		break;
	case UI::BROODI:
		tStone = { eStone,L"브루디",L"체력 +1",UI::STONE_HEALTH,false };
		break;
	case UI::BAT:
		tStone = { eStone,L"박쥐",L"체력 +1",UI::STONE_HEALTH,false };
		break;
	case UI::LEVIATHAN:
		tStone = { eStone,L"레비아탄",L"체력 +1",UI::STONE_HEALTH,false };
		break;
	case UI::NINJA:
		tStone = { eStone,L"닌자",L"체력 +1",UI::STONE_HEALTH,false };
		break;
	case UI::WATERBOSS:
		tStone = { eStone, L"바돈",L"체력 +1",UI::STONE_HEALTH,false };
		break;
	case UI::GRINNER2:
		tStone = { eStone,L"빨강 개",L"체력 +1",UI::STONE_HEALTH,false };
		break;
	case UI::CRIBO2:
		tStone = { eStone, L"얼음 크리보",L"체력 +1",UI::STONE_HEALTH,false };
		break;
	case UI::MAE:
		tStone = { eStone,L"마스 터이",L"체력 +1",UI::STONE_HEALTH,false };
		break;
	default:
		break;
	}
	return tStone;
}

void CGameMgr::CameraEvent()
{
	for (auto iter : m_listEvent)
	{
		if (iter == m_iNaviIdx)
			return;
	}

	switch (m_iNaviIdx)
	{
	case 0 :
		m_pCamera->SetOption(&m_iNaviIdx);
		m_listEvent.emplace_back(m_iNaviIdx);
		break;
	case 4:
		SpawnSet((_uint)SpawnSet::GoblinFiledSp1);
		m_pCamera->SetOption(&m_iNaviIdx);
		m_listEvent.emplace_back(m_iNaviIdx);
		break;
	case 14:
	{
		_uint eStateType = (_uint)Player_Barrier::STATE_CLEOSE;
		CGameObject* pObj = Get_GameObject(L"PlayerBarrier", L"PlayerBarrier");
		pObj->SetOption(&eStateType);
		dynamic_cast<CStaticCamera*>(m_pCamera)->SetTarget(pObj);
		m_pCamera->SetOption(&m_iNaviIdx);
		m_listEvent.emplace_back(m_iNaviIdx);
	}
	case 16:
	{
		_uint Spawn0 = Grinner::SPAWN_APEX;
		vector<CGameObject*> m_vecEnemy = CSpawnMgr::GetInstance()->GetGrinnerVec();

		m_listEvent.emplace_back(m_iNaviIdx);
		break;
	}

	case 22:
		m_pCamera->SetOption(&m_iNaviIdx);
		m_listEvent.emplace_back(m_iNaviIdx);
		//상자 정면으로 카메라 회전
		break;
	case 25: 
	{
		CGameObject* pObj = Get_GameObject(L"Enemy", L"Angel");
		dynamic_cast<CAngel*>(pObj)->SetNaviIdx(28);
		pObj->SetOption();
		m_listEvent.emplace_back(m_iNaviIdx);
		break;
	}
	case 27:
		m_pCamera->SetOption(&m_iNaviIdx);
		m_listEvent.emplace_back(m_iNaviIdx);
		break;

	case 31:
		//카메라 보스쪽으로 회전
		m_pCamera->SetOption(&m_iNaviIdx);
		m_listEvent.emplace_back(m_iNaviIdx);
		break;
	case 43:
	{
		//보스 시작씬 이후 보스쪽으로 회전
		CGameObject* pObj = Get_GameObject(L"Enemy", L"WaterBoss");
		_uint idx;
		dynamic_cast<CWaterBoss*>(pObj)->SetOption(&idx);
		dynamic_cast<CStaticCamera*>(m_pCamera)->SetTarget(pObj);
		m_pCamera->SetOption(&m_iNaviIdx);
		m_listEvent.emplace_back(m_iNaviIdx);
	}
		break;
	default:
		break;
	}
}

void CGameMgr::CameraEvent(CGameObject* pTargetObj)
{
	if (pTargetObj == m_pCamera->GetTarget())
	{
		dynamic_cast<CStaticCamera*>(m_pCamera)->ReleaseView();
		dynamic_cast<CStaticCamera*>(m_pCamera)->ReleaseTarget();
	}
	else
	{
		m_pCamera->SetTarget(pTargetObj);
		dynamic_cast<CStaticCamera*>(m_pCamera)->SetEventView();
	}
}

_bool CGameMgr::EventAngel()
{
	CGameObject* pObj = Get_GameObject(L"Enemy", L"Angel");
	return pObj->GetDead();

}

HRESULT CGameMgr::InitObjPool()
{
	InitEnemyBullet();
	InitPlayerBullet();
	InitEffect();
	InitItem();
	//InitParticle();
	//파티클
	//이펙트

	return S_OK;
}

HRESULT CGameMgr::InitEnemyBullet()
{
	CGameObject* pObj = nullptr;
	USES_CONVERSION;
	for (_uint i = 0; i < 20; i++)
	{
		wstring wstrIndxKey = to_wstring(m_iBulletIdx);
		const _tchar* pConvObjTag = W2BSTR(wstrIndxKey.c_str());
		pObj = CBullet::Create(m_pGraphicDev);
		pObj->SetActive(false);
		Add_GameObject(L"Bullet_Enemy", pConvObjTag, pObj);
		m_queEnemyBullet.emplace(pObj);
		m_iBulletIdx++;
	}
	return S_OK;
}

HRESULT CGameMgr::InitPlayerBullet()
{
	CGameObject* pObj = nullptr;
	USES_CONVERSION;
	for (_uint i = 0; i < 20; i++)
	{
		wstring wstrIndxKey = to_wstring(m_iBulletIdx);
		const _tchar* pConvObjTag = W2BSTR(wstrIndxKey.c_str());
		pObj = CBullet::Create(m_pGraphicDev);
		pObj->SetActive(false);
		Add_GameObject(L"Bullet_Player", pConvObjTag, pObj);
		m_quePlayerBullet.emplace(pObj);
		m_iBulletIdx++;
	}
	return S_OK;
}

void CGameMgr::RetunPlayerBullet(CGameObject * pObj)
{
	_uint eType = BULLET::BULLET_END;
	dynamic_cast<CBullet*>(pObj)->SetOption(&eType);
	//	pObj->SetActive(false);

	m_quePlayerBullet.emplace(pObj);
}

CGameObject * CGameMgr::GetPlayerBullet(_uint eType)
{
	if (m_quePlayerBullet.empty())
	{
		InitPlayerBullet();
	}
	CGameObject* pObj = m_quePlayerBullet.front();
	//pObj->SetActive(true);
	dynamic_cast<CBullet*>(pObj)->SetOption(&eType);
	m_quePlayerBullet.pop();
	return pObj;
}

void CGameMgr::RetunEnemyBullet(CGameObject * pObj)
{
	//pObj 조정한 값 초기화 
	_uint eType = BULLET::BULLET_END;
	dynamic_cast<CBullet*>(pObj)->SetOption(&eType);
//	pObj->SetActive(false);
	
	m_queEnemyBullet.emplace(pObj);
}

CGameObject * CGameMgr::GetEnemyBullet(_uint eType)
{
	if (m_queEnemyBullet.empty())
	{
		InitEnemyBullet();
	}
	CGameObject* pObj = m_queEnemyBullet.front();
	//pObj->SetActive(true);
	dynamic_cast<CBullet*>(pObj)->SetOption(&eType);
	m_queEnemyBullet.pop();
	return pObj;
}


HRESULT CGameMgr::InitEffect()
{
	CGameObject* pObj = nullptr;
	USES_CONVERSION;
	for (_uint i = 0; i < 20; i++)
	{
		wstring wstrIndxKey = to_wstring(m_iEffectIdx);
		const _tchar* pConvObjTag = W2BSTR(wstrIndxKey.c_str());
		pObj = CEffect::Create(m_pGraphicDev);
		pObj->SetActive(false);
		Add_GameObject(L"Effect", pConvObjTag, pObj);
		m_queEffect.emplace(pObj);
		m_iEffectIdx++;
	}
	return S_OK;
}


void CGameMgr::RetunEffect(CGameObject * pObj)
{
	_uint eType = EFFECT::EFFECT_END;
	//dynamic_cast<CEffect*>(pObj)->SetOption(&eType);

	m_queEffect.emplace(pObj);
}

//1. 이펙트 매니져 호출
//2. 게임 메니져에서 이펙트랑 파티클을 가져감 
CGameObject * CGameMgr::GetEffect(_uint eType)
{
	if (m_queEffect.empty())
	{
		InitEffect();
	}
	CGameObject* pObj = m_queEffect.front();
	dynamic_cast<CEffect*>(pObj)->SetOption(&eType);
	m_queEffect.pop();

	return pObj;
}

HRESULT CGameMgr::InitParticle()
{
	CGameObject* pObj = nullptr;
	USES_CONVERSION;
	for (_uint i = 0; i < 20; i++)
	{
		wstring wstrIndxKey = to_wstring(m_iParticleIdx);
		const _tchar* pConvObjTag = W2BSTR(wstrIndxKey.c_str());
		pObj = CParticleSystem::Create(m_pGraphicDev);
		pObj->SetActive(false);
		Add_GameObject(L"Particle", pConvObjTag, pObj);
		m_queParticle.emplace(pObj);
		m_iParticleIdx++;
	}
	return S_OK;
}

void CGameMgr::RetunParticle(CGameObject * pObj)
{
	//_uint eType = PARTICLEEFF::PARTICLE_END;
	//dynamic_cast<CParticleSystem*>(pObj)->SetOption(&eType);
	pObj->SetActive(false);
	m_queParticle.emplace(pObj);
}

CGameObject * CGameMgr::GetParticle(_uint eType)
{
	if (m_queParticle.empty())
	{
		InitParticle();
	}
	CGameObject* pObj = m_queParticle.front();

	POPTION tOption;

	switch (eType)
	{
	case PARTICLEEFF::PARTICLE_LIGHTNING:
		tOption = GetParticleInfo((PARTICLEEFF::TYPE)eType);
		dynamic_cast<CParticleSystem*>(pObj)->SetParticle(L"../../Resource/Texture/Effect/Particle.png", tOption);
		pObj->SetActive(true);
		//dynamic_cast<CParticleSystem*>(pObj)->SetOption(&eType);
		m_queParticle.pop();
	}
	return pObj;
}

HRESULT CGameMgr::InitItem()
{
	CGameObject* pObj = nullptr;
	USES_CONVERSION;
	for (_uint i = 0; i < 20; i++)
	{
		wstring wstrIndxKey = to_wstring(m_iItemIdx);
		const _tchar* pConvObjTag = W2BSTR(wstrIndxKey.c_str());
		pObj = CItem::Create(m_pGraphicDev);
		pObj->SetActive(false);
		Add_GameObject(L"Item", pConvObjTag, pObj);
		m_queItem.emplace(pObj);
		m_iItemIdx++;
	}
	return S_OK;
}

void CGameMgr::RetunItem(CGameObject * pObj)
{
	_uint eType = DROPITEM::ITEM_END;
	dynamic_cast<CItem*>(pObj)->SetOption(&eType);

	m_queItem.emplace(pObj);
}

CGameObject * CGameMgr::GetItem(_uint eType)
{
	if (m_queItem.empty())
	{
		InitItem();
	}
	CGameObject* pObj = m_queItem.front();
	dynamic_cast<CItem*>(pObj)->SetOption(&eType);
	m_queItem.pop();
	
	return pObj;
}

void CGameMgr::SpawnSet(_uint idx)
{
	vector<CGameObject*> m_vecEnemy;
	switch (idx)
	{
	case (_uint)SpawnSet::GoblinFiledSp1:
	{
		_uint Spawn0 = Goblin::SPAWN_APEX;
		_uint Spawn2 = Goblin::SPAWN_SIT;
		m_vecEnemy = CSpawnMgr::GetInstance()->GetGoblinVec();
		dynamic_cast<CGoblin*>(m_vecEnemy[0])->SetNaviIdx(5);
		m_vecEnemy[0]->SetOption(&Spawn0);
		dynamic_cast<CGoblin*>(m_vecEnemy[1])->SetNaviIdx(4);
		m_vecEnemy[1]->SetOption(&Spawn0);
		dynamic_cast<CGoblin*>(m_vecEnemy[2])->SetNaviIdx(10);
		m_vecEnemy[2]->SetOption(&Spawn2);
		dynamic_cast<CGoblin*>(m_vecEnemy[3])->SetNaviIdx(10);
		m_vecEnemy[3]->SetOption(&Spawn2);
		dynamic_cast<CGoblin*>(m_vecEnemy[4])->SetNaviIdx(5);
		m_vecEnemy[4]->SetOption(&Spawn0);
		////m_vecEnemy[5]->SetOption(&Spawn0);
		////m_vecEnemy[6]->SetOption(&Spawn0);
		////m_vecEnemy[7]->SetOption(&Spawn0);

		dynamic_cast<CStaticCamera*>(m_pCamera)->SetTarget(m_vecEnemy[0]);
		//스폰 이벤트가 끝나면 
	}
		break;
	case (_uint)SpawnSet::GoblinFiledSp2:
	{
		//_uint Spawn2 = Goblin::SPAWN_SIT;
		//m_vecEnemy = CSpawnMgr::GetInstance()->GetGoblinVec();
		//m_vecEnemy[2]->SetOption(&Spawn2);
		//m_vecEnemy[3]->SetOption(&Spawn2);
		////dynamic_cast<CStaticCamera*>(m_pCamera)->SetTarget(m_vecEnemy[0]);
	}
	break;
	case (_uint)SpawnSet::GrinnerFiledSp:
	{
		_uint Spawn0 = Grinner::SPAWN_APEX;
		//_uint Spawn1 = Grinner::SPAWN_POTRAL;
		m_vecEnemy = CSpawnMgr::GetInstance()->GetGrinnerVec();
		dynamic_cast<CGrinner*>(m_vecEnemy[0])->SetNaviIdx(14);
		m_vecEnemy[0]->SetOption(&Spawn0);
		dynamic_cast<CGrinner*>(m_vecEnemy[1])->SetNaviIdx(15);
		m_vecEnemy[1]->SetOption(&Spawn0);
		dynamic_cast<CGrinner*>(m_vecEnemy[2])->SetNaviIdx(17);
		m_vecEnemy[2]->SetOption(&Spawn0);
		break;
	}
	default:
		break;
	}
}



void CGameMgr::Free(void)
{

}


