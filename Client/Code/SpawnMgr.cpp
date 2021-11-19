#include "stdafx.h"
#include "SpawnMgr.h"

#include "Export_Function.h"
#include "ColMesh.h"
#include "GameMgr.h"
#include "Player.h"
#include "WaterBoss.h"
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


