#include "stdafx.h"
#include "SpawnMgr.h"

#include "Export_Function.h"
#include "ColMesh.h"
#include "GameMgr.h"
#include "Player.h"
#include "WaterBoss.h"

IMPLEMENT_SINGLETON(CSpawnMgr)
CSpawnMgr::CSpawnMgr()

{

}

CSpawnMgr::~CSpawnMgr(void)
{

}

CGameObject* CSpawnMgr::Spawn(CGameObject* pTargetObj, LOAD_DATA_COL tLoadData)
{
	CGameObject* pCollider = nullptr;
	pCollider = CColMesh::Create(CGameMgr::GetInstance()->GetDevice(), tLoadData.tCol,tLoadData.wstrBoneName.c_str());
	pCollider->SetTarget(pTargetObj);
	
	return pCollider;
}

void CSpawnMgr::Spawn(MESH tMesh)
{

}

//매시 스폰
CGameObject* CSpawnMgr::Spawn(wstring Objkey, MESH tMesh)
{
	CGameObject* pGameObject = nullptr;
	//player스폰
	if (Objkey == L"War")
	{
		pGameObject = CPlayer::Create(CGameMgr::GetInstance()->GetDevice());
		NULL_CHECK_RETURN(pGameObject, nullptr);
		CGameMgr::GetInstance()->SetPlayer(pGameObject);
	}
	//waterBoss 스폰
	if(Objkey ==L"WaterBoss")
	{
		pGameObject = CWaterBoss::Create(CGameMgr::GetInstance()->GetDevice());
		NULL_CHECK_RETURN(pGameObject, nullptr);
	}

	return pGameObject;

}



void CSpawnMgr::Free(void)
{

}


