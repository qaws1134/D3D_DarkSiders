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
void CEffMgr::SpawnEff(EFFECTSET::TYPE eEffect,_vec3 vPos)
{

	//CGameObject* pObj = nullptr;
	////pObj=  CGameMgr::GetInstance()->GetEffect(eEffect);
	////한번 타임에 여러개 생성 시 

	//switch (eEffect)
	//{
	//case EFFECTSET::BOSS_CALL_LIGHTNING_START:
	//	for (_uint i = 0; i < 4; i++)
	//	{
	//		m_listReadyEff2D.emplace_back(EFFECT::EFFECT_MAGIC_CIRCLE_SINGLE);
	//		pObj = CGameMgr::GetInstance()->GetEffect(EFFECT::EFFECT_MAGIC_CIRCLE_SINGLE);
	//		dynamic_cast<CEffect*>(pObj)->SetNextEffect(EFFECTSET::BOSS_CALL_LIGHTNING_MID);
	//		m_listEffect.emplace_back(pObj);
	//	}	

	//	m_bRandBegin = true;
	//	m_fStartTimeBegin = 0.5f;
	//	m_fStartTimeEnd= 1.f;

	//	m_bRandomPos = true;
	//	m_fStartSpeed = 0.f;
	//	break;
	//case EFFECTSET::BOSS_CALL_LIGHTNING_MID:
	//	pObj = CGameMgr::GetInstance()->GetEffect(EFFECT::EFFECT_LIGHTNING1_1x4);
	//	pObj->SetPos(vPos,ID_DYNAMIC);
	//	m_listEffect.emplace_back(pObj);
	//	pObj = CGameMgr::GetInstance()->GetEffect(EFFECT::EFFECT_WATERBOOM1_2x2);
	//	pObj->SetPos(vPos, ID_DYNAMIC);
	//	m_listEffect.emplace_back(pObj);
	//	m_fStartTime = 0.;

	//	m_bRandomPos = false;
	//	
	//	break;

	//case EFFECTSET::EFFECTSET_END:
	//	break;
	//default:
	//	break;
	//}
	///*pObj = CGameMgr::GetInstance()->GetEffect(eEffect);
	//pObj->SetOption(&eEffect);
	//m_listEffect.emplace_back(pObj);*/

}

////업데이트 
void CEffMgr::EffectListUpdate()
{

	//for (auto iter = m_listEffect.begin(); iter != m_listEffect.end(); )
	//{
	//	if (m_bRandBegin)
	//	{
	//		
	//	}
	//	if (m_bRandomPos)
	//	{

	//	}

	//	//순회하면서 활성화가 안되어있을 시 이펙트가 종료 되었다고 판단
	//	if (!(*iter)->GetActive())
	//	{
	//		//다음 이펙트를 생성시켜준다 
	//		SpawnEff(dynamic_cast<CEffect*>(*iter)->GetNextEffect(),*(*iter)->GetPos(ID_DYNAMIC));
	//		CGameMgr::GetInstance()->RetunEffect(*iter);
	//		m_listEffect.erase(iter); //리스트에서 해당 노드 삭제 
	//	}
	//	else
	//		iter++;
	//}
}


void CEffMgr::Free(void)
{

}
