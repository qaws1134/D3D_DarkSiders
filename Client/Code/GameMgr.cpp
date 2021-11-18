#include "stdafx.h"
#include "GameMgr.h"
#include "UIMgr.h"
#include "Bullet.h"
#include "Effect.h"
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

HRESULT CGameMgr::InitObjPool()
{
	InitBullet();
	InitEffect();
	//파티클
	//이펙트

	return S_OK;
}

HRESULT CGameMgr::InitBullet()
{
	CGameObject* pObj = nullptr;
	USES_CONVERSION;
	for (_uint i = 0; i < 20; i++)
	{
		wstring wstrIndxKey = to_wstring(m_iBulletIdx);
		const _tchar* pConvObjTag = W2BSTR(wstrIndxKey.c_str());
		pObj = CBullet::Create(m_pGraphicDev);
		pObj->SetActive(false);
		Add_GameObject(L"Bullet", pConvObjTag, pObj);
		m_queBullet.emplace(pObj);
		m_iBulletIdx++;
	}
	return S_OK;
}

void CGameMgr::RetunBullet(CGameObject * pObj)
{
	//pObj 조정한 값 초기화 
	_uint eType = BULLET::BULLET_END;
	dynamic_cast<CBullet*>(pObj)->SetOption(&eType);
//	pObj->SetActive(false);
	
	m_queBullet.emplace(pObj);
}

CGameObject * CGameMgr::GetBullet(_uint eType)
{
	if (m_queBullet.empty())
	{
		InitBullet();
	}
	CGameObject* pObj = m_queBullet.front();
	//pObj->SetActive(true);
	dynamic_cast<CBullet*>(pObj)->SetOption(&eType);
	m_queBullet.pop();
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
	dynamic_cast<CEffect*>(pObj)->SetOption(&eType);

	m_queEffect.emplace(pObj);
}

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



void CGameMgr::Free(void)
{

}


