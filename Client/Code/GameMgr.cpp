#include "stdafx.h"
#include "GameMgr.h"
#include "UIMgr.h"
#include "Bullet.h"
#include "Effect.h"
#include "ParticleSystem.h"
#include "EffMgr.h"
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
	//�ð������� 
	//Ư��ȿ��  ����, �ٴڿ� ���ӵ� ��� ����, ũ��Ƽ��Ȯ��, �̵��ӵ� �߰�  
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
		tStone = { eStone, L"���",L"��ų �� +1 ",UI::STONE_SKILL,false };
		break;
	case UI::STATUE:
		tStone = { eStone,L"����",L"��ų �� +1",UI::STONE_SKILL,true };
		break;
	case UI::KNIGHT:
		tStone = { eStone, L"���",L"��ų �� +1",UI::STONE_SKILL,false };
		break;
	case UI::ICEMAGE:
		tStone = { eStone,L"���������",L"��ų �� +1",UI::STONE_SKILL,false };
		break;
	case UI::SKULL:
		tStone = { eStone, L"����",L"��ų �� +1",UI::STONE_SKILL,false };
		break;
	case UI::CRIBO:
		tStone = { eStone,L"ũ����",L"��ų �� +1",UI::STONE_SKILL,true };
		break;
	case UI::LARVA:
		tStone = { eStone, L"�ֹ���",L"��ų �� +1",UI::STONE_SKILL,false };
		break;
	case UI::ANT:
		tStone = { eStone,L"�Ұ���",L"���� �� +1",UI::STONE_ATK,false };
		break;
	case UI::BLUEANGEL:
		tStone = { eStone,L"õ��",L"���� �� +1",UI::STONE_ATK,true };
		break;
	case UI::GREMLIN1:
		tStone = { eStone,L"�׷���",L"���� �� +1",UI::STONE_ATK,false };
		break;
	case UI::GREMLIN2:
		tStone = { eStone,L"��ȭ �׷���",L"���� �� + 5",UI::STONE_ATK,false };
		break;
	case UI::GREENGOBLE:
		tStone = { eStone,L"�ʷ� ���",L"���� �� + 5",UI::STONE_ATK,false };
		break;
	case UI::SKULLMAGE:
		tStone = { eStone, L"�ذ� ������",L"���� �� + 5",UI::STONE_ATK,false };
		break;
	case UI::GRINNER:
		tStone = { eStone, L"�׸���",L"���� �� + 5",UI::STONE_ATK,true };
		break;
	case UI::BROODI:
		tStone = { eStone,L"����",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	case UI::BAT:
		tStone = { eStone,L"����",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	case UI::LEVIATHAN:
		tStone = { eStone,L"�����ź",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	case UI::NINJA:
		tStone = { eStone,L"����",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	case UI::WATERBOSS:
		tStone = { eStone, L"�ٵ�",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	case UI::GRINNER2:
		tStone = { eStone,L"���� ��",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	case UI::CRIBO2:
		tStone = { eStone, L"���� ũ����",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	case UI::MAE:
		tStone = { eStone,L"���� ����",L"ü�� +1",UI::STONE_HEALTH,false };
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
	//InitParticle();
	//��ƼŬ
	//����Ʈ

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
	//pObj ������ �� �ʱ�ȭ 
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
	//dynamic_cast<CEffect*>(pObj)->SetOption(&eType);

	m_queEffect.emplace(pObj);
}

//1. ����Ʈ �Ŵ��� ȣ��
//2. ���� �޴������� ����Ʈ�� ��ƼŬ�� ������ 
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



void CGameMgr::Free(void)
{

}


