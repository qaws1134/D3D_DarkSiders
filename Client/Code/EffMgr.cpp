#include "stdafx.h"
#include "EffMgr.h"
#include "GameMgr.h"
#include "ParticleSystem.h"
#include "Export_Function.h"

IMPLEMENT_SINGLETON(CEffMgr)


CEffMgr::CEffMgr(void)
{
	
}
CEffMgr::~CEffMgr(void)
{
	Free();
}


//�ѹ��� ���ϰ� ����
void CEffMgr::SpawnEff(EFFECT::TYPE eEffect)
{

	CGameObject* pObj = nullptr;
	//pObj=  CGameMgr::GetInstance()->GetEffect(eEffect);
	//�ѹ� Ÿ�ӿ� ������ ���� �� 
	switch (eEffect)
	{
	case EFFECT::EFFECT_CALLLIGHTNING_START:

		pObj = CGameMgr::GetInstance()->GetEffect(eEffect);
		m_listEffect.emplace_back(pObj);
		pObj = CGameMgr::GetInstance()->GetParticle(PARTICLEEFF::PARTICLE_LIGHTNING);
		break;
	case EFFECT::EFFECT_CALLLIGHTNING_LOOP:
		break;
	case EFFECT::EFFECT_CALLLIGHTNING_END:
		break;
	case EFFECT::EFFECT_ORBCHAGE_START:
		break;
	case EFFECT::EFFECT_ORBCHAGE_LOOP:
		break;
	case EFFECT::EFFECT_ORBCHAGE_END:
		break;
	case EFFECT::EFFECT_TSUNAMICHARGE_START:
		break;
	case EFFECT::EFFECT_TSUNAMICHARGE_LOOP:
		break;
	case EFFECT::EFFECT_TSUNAMICHARGE_END:
		break;
	case EFFECT::EFFECT_TAIL_START:
		break;
	case EFFECT::EFFECT_TAIL_LOOP:
		break;
	case EFFECT::EFFECT_TAIL_END:
		break;
	case EFFECT::EFFECT_END:
		break;
	default:
		break;
	}

	/*pObj = CGameMgr::GetInstance()->GetEffect(eEffect);
	pObj->SetOption(&eEffect);
	m_listEffect.emplace_back(pObj);*/

}

//������Ʈ 
void CEffMgr::EffectListUpdate()
{
	for (auto iter = m_listEffect.begin(); iter != m_listEffect.end(); )
	{
		//��ȸ�ϸ鼭 Ȱ��ȭ�� �ȵǾ����� �� ����Ʈ�� ���� �Ǿ��ٰ� �Ǵ�
		if (!(*iter)->GetActive())
		{
			//���� ����Ʈ�� ���������ش� 
			SpawnEff(dynamic_cast<CEffect*>(*iter)->GetNextEffect());
			CGameMgr::GetInstance()->RetunEffect(*iter);
			m_listEffect.erase(iter); //����Ʈ���� �ش� ��� ���� 
		}
		else if (dynamic_cast<CEffect*>(*iter)->GetNextStart())
		{
			//�߰��� �ִϸ��̼� ������ ���� ���õ� �ִϸ��̼� ���� �� �ٽ� �ʱⰪ���� ���� 
			SpawnEff(dynamic_cast<CEffect*>(*iter)->GetNextEffect());
			dynamic_cast<CEffect*>(*iter)->ResetNextEffect();
			iter++;
		}
		else
			iter++;
	}
}


void CEffMgr::Free(void)
{

}
