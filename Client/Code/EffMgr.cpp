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
void CEffMgr::SpawnEff(EFFECTSET::TYPE eEffect,_vec3 vPos)
{

	//CGameObject* pObj = nullptr;
	////pObj=  CGameMgr::GetInstance()->GetEffect(eEffect);
	////�ѹ� Ÿ�ӿ� ������ ���� �� 

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

////������Ʈ 
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

	//	//��ȸ�ϸ鼭 Ȱ��ȭ�� �ȵǾ����� �� ����Ʈ�� ���� �Ǿ��ٰ� �Ǵ�
	//	if (!(*iter)->GetActive())
	//	{
	//		//���� ����Ʈ�� ���������ش� 
	//		SpawnEff(dynamic_cast<CEffect*>(*iter)->GetNextEffect(),*(*iter)->GetPos(ID_DYNAMIC));
	//		CGameMgr::GetInstance()->RetunEffect(*iter);
	//		m_listEffect.erase(iter); //����Ʈ���� �ش� ��� ���� 
	//	}
	//	else
	//		iter++;
	//}
}


void CEffMgr::Free(void)
{

}
