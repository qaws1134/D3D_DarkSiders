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


//한번만 톡하고 생성
void CEffMgr::SpawnEff(EFFECT::TYPE eEffect)
{

	CGameObject* pObj = nullptr;
	//pObj=  CGameMgr::GetInstance()->GetEffect(eEffect);
	//한번 타임에 여러개 생성 시 
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

//업데이트 
void CEffMgr::EffectListUpdate()
{
	for (auto iter = m_listEffect.begin(); iter != m_listEffect.end(); )
	{
		//순회하면서 활성화가 안되어있을 시 이펙트가 종료 되었다고 판단
		if (!(*iter)->GetActive())
		{
			//다음 이펙트를 생성시켜준다 
			SpawnEff(dynamic_cast<CEffect*>(*iter)->GetNextEffect());
			CGameMgr::GetInstance()->RetunEffect(*iter);
			m_listEffect.erase(iter); //리스트에서 해당 노드 삭제 
		}
		else if (dynamic_cast<CEffect*>(*iter)->GetNextStart())
		{
			//중간에 애니메이션 실행이 오면 선택된 애니메이션 실행 후 다시 초기값으로 리셋 
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
