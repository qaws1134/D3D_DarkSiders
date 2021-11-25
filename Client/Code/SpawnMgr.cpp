#include "stdafx.h"
#include "SpawnMgr.h"

#include "Export_Function.h"
#include "ColMesh.h"
#include "GameMgr.h"
#include "Player.h"
#include "WaterBoss.h"
#include "Goblin.h"
#include "Chest.h"
#include "Grinner.h"
#include "Player_Barrier.h"
#include "WaterFall.h"
#include "Cell.h"
#include "Building.h"
IMPLEMENT_SINGLETON(CSpawnMgr)
CSpawnMgr::CSpawnMgr()

{

}

CSpawnMgr::~CSpawnMgr(void)
{

}

CGameObject* CSpawnMgr::Spawn(CGameObject* pTargetObj, LOAD_DATA_COL tLoadData)
{
	
	CGameObject* pCollider = CColMesh::Create(CGameMgr::GetInstance()->GetDevice(), tLoadData.tCol,tLoadData.wstrBoneName.c_str());
	pCollider->SetTarget(pTargetObj);
	
	return pCollider;
}


//USES_CONVERSION;

//for (auto& iter : CLoadMgr::GetInstance()->SpawnData())
//{
//	 const _tchar* pObjKEy = W2BSTR(iter.first.c_str());

//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjKEy,iter.second), E_FAIL);
//	
//}

//매시 스폰
CGameObject* CSpawnMgr::Spawn(wstring Objkey, MESH tMesh, wstring* pLayerTag)
{
	USES_CONVERSION;
	

	CGameObject* pGameObject = nullptr;
	//player스폰
	if (Objkey == L"War")
	{
		*pLayerTag = L"Player";
		const _tchar* pConvLayerTag = W2BSTR((*pLayerTag).c_str());
		pGameObject = CPlayer::Create(CGameMgr::GetInstance()->GetDevice());
		NULL_CHECK_RETURN(pGameObject, nullptr);
		CGameMgr::GetInstance()->SetPlayer(pGameObject);

		Add_GameObject(pConvLayerTag, L"Player", pGameObject);
	}
	//waterBoss 스폰
	else if(Objkey ==L"WaterBoss")
	{
		*pLayerTag = L"Enemy";
		const _tchar* pConvLayerTag = W2BSTR((*pLayerTag).c_str());
		pGameObject = CWaterBoss::Create(CGameMgr::GetInstance()->GetDevice());
		NULL_CHECK_RETURN(pGameObject, nullptr);

		Add_GameObject(pConvLayerTag, L"WaterBoss", pGameObject);
	}
	else if (Objkey == L"Grinner")
	{
		*pLayerTag = L"Enemy";

		const _tchar* pConvProtoTag = W2BSTR((Objkey+to_wstring(m_iGrinnerIdx)).c_str());
		const _tchar* pConvLayerTag = W2BSTR((*pLayerTag).c_str());
		const _tchar* pConvObjTag = W2BSTR((*pLayerTag + to_wstring(m_iEnemyIdx)).c_str());
		pGameObject = CGrinner::Create(CGameMgr::GetInstance()->GetDevice(),pConvProtoTag);
		NULL_CHECK_RETURN(pGameObject, nullptr);
		m_vecGrinner.emplace_back(pGameObject);
		Add_GameObject(pConvLayerTag, pConvObjTag, pGameObject);
		m_iGrinnerIdx++;
		m_iEnemyIdx++;
	}
	else if (Objkey == L"Goblin")
	{
		*pLayerTag = L"Enemy";
		const _tchar* pConvProtoTag = W2BSTR((Objkey + to_wstring(m_iGoblinIdx)).c_str());
		const _tchar* pConvLayerTag = W2BSTR((*pLayerTag).c_str());
		const _tchar* pConvObjTag = W2BSTR((*pLayerTag + to_wstring(m_iEnemyIdx)).c_str());
		pGameObject = CGoblin::Create(CGameMgr::GetInstance()->GetDevice(), pConvProtoTag);
		NULL_CHECK_RETURN(pGameObject, nullptr);
		m_vecGoblin.emplace_back(pGameObject);
		Add_GameObject(pConvLayerTag, pConvObjTag, pGameObject);
		m_iGoblinIdx++;
		m_iEnemyIdx++;
	}
	else if (Objkey == L"Chest")
	{
		*pLayerTag = L"Interaction";		//NPC, 상자 
		const _tchar* pConvLayerTag = W2BSTR((*pLayerTag).c_str());
		pGameObject = CChest::Create(CGameMgr::GetInstance()->GetDevice());
		NULL_CHECK_RETURN(pGameObject, nullptr);
		Add_GameObject(pConvLayerTag, L"Chest", pGameObject);
	}
	else if (Objkey == L"PlayerBarrier")
	{
		*pLayerTag = L"PlayerBarrier";		//조작을 위한 레이어 분리 
		//플레이어의 네비매시 인덱스로 조작 
		const _tchar* pConvLayerTag = W2BSTR((*pLayerTag).c_str());
		pGameObject = CPlayer_Barrier::Create(CGameMgr::GetInstance()->GetDevice());
		//const _tchar* pConvObjTag = W2BSTR(L"PlayerBarrier").c_str());
		NULL_CHECK_RETURN(pGameObject, nullptr);
		Add_GameObject(pConvLayerTag, L"PlayerBarrier", pGameObject);
	}
	else if (Objkey == L"Eden_WaterFall0"|| Objkey == L"Eden_WaterFall1")
	{
		//스테틱 매시 
		//스크립트 조작만 필요
		*pLayerTag = L"Building";
		const _tchar* pConvLayerTag = W2BSTR((*pLayerTag).c_str());
		const _tchar* pConvObjTag = W2BSTR((*pLayerTag + to_wstring(m_iBuidingIdx)).c_str());
		pGameObject = CWaterFall::Create(CGameMgr::GetInstance()->GetDevice(), Objkey.c_str());
		NULL_CHECK_RETURN(pGameObject, nullptr);
		Add_GameObject(pConvLayerTag, pConvObjTag, pGameObject);
		m_iBuidingIdx++;
	}
	else
	{
		//맵 스폰 
		*pLayerTag = L"Building"; 
		const _tchar* pConvLayerTag = W2BSTR((*pLayerTag).c_str());
		const _tchar* pConvObjTag= W2BSTR((*pLayerTag +to_wstring(m_iBuidingIdx)).c_str());
		pGameObject = CBuilding::Create(CGameMgr::GetInstance()->GetDevice(), Objkey.c_str());
		NULL_CHECK_RETURN(pGameObject, nullptr);
		Add_GameObject(pConvLayerTag, pConvObjTag, pGameObject);
		m_iBuidingIdx++;
		
	}

	return pGameObject;
}



void CSpawnMgr::Free(void)
{

}


