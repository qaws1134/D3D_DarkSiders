#include "stdafx.h"
#include "LoadMgr.h"
#include "Struct.h"
#include "SpawnMgr.h"
#include "NaviMesh.h"
#include "GameMgr.h"
#include "Player.h"
#include "Export_Function.h"


IMPLEMENT_SINGLETON(CLoadMgr)

CLoadMgr::CLoadMgr()
	
{

}

CLoadMgr::~CLoadMgr(void)
{

}

HRESULT CLoadMgr::LoadData(wstring szFilePath)
{
	LoadMeshTool(szFilePath+L"Map35Obj.dat");
	//LoadColTool(szFilePath+L"Colider28.dat");
	//LoadMeshTool(szFilePath+L"TestNpc.dat");
	//LoadMeshTool(szFilePath+L"Test4.dat");
	//LoadMeshTool(szFilePath + L"MapStart.dat");

	return S_OK;
}

HRESULT CLoadMgr::LoadStartStageData(wstring szFilePath)
{
	LoadColTool(szFilePath + L"Colider28.dat");
	LoadMeshTool(szFilePath + L"MapStart.dat");

	return S_OK;
}

HRESULT CLoadMgr::LoadColTool(wstring szFilePath)
{

	HANDLE hFile = CreateFile(szFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return E_FAIL;
	}

	DWORD dwbyte = 0;
	DWORD dwStringSize = 0;
	DWORD dwMapSize = 0;
	TCHAR* pBuf = nullptr;

	TCHAR* pTagBuf = nullptr;
	TCHAR* pBoneBuf = nullptr;

	while (true)
	{
		//ojb키
		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		if (0 == dwbyte)
			break;
		pBuf = new TCHAR[dwStringSize];
		ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
		//사이즈 개수만큼 반복문 ㄱ
		ReadFile(hFile, &dwMapSize, sizeof(DWORD), &dwbyte, nullptr);
		map<wstring, LOAD_DATA_COL> mapColData;
		for (_uint i = 0; i <dwMapSize; i++)
		{
			LOAD_DATA_COL LoadData;
			//태그
			ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			pTagBuf = new TCHAR[dwStringSize];
			ReadFile(hFile, pTagBuf, dwStringSize, &dwbyte, nullptr);

			//뼈이름
			ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			pBoneBuf = new TCHAR[dwStringSize];
			ReadFile(hFile, pBoneBuf, dwStringSize, &dwbyte, nullptr);
			LoadData.wstrBoneName = pBoneBuf;

			ReadFile(hFile, &LoadData.tCol, sizeof(COLLIDERSPHERE), &dwbyte, nullptr);

			mapColData.emplace(pTagBuf, LoadData);
			Safe_Delete(pTagBuf);
			Safe_Delete(pBoneBuf);
		}

		m_mapMeshCollider.emplace(pBuf, mapColData);
		Safe_Delete(pBuf);
	}
	CloseHandle(hFile);
	return S_OK;
}

HRESULT CLoadMgr::LoadMeshTool(wstring szFilePath)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HANDLE hFile = CreateFile(szFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwbyte = 0;
	DWORD dwStringSize = 0;
	DWORD dwMapSize = 0;
	DWORD dwObjSize = 0;
	TCHAR* pBuf = nullptr;
	MESH tMesh;

	m_mapNaviData.clear();
	m_mapStaticData.clear();
	m_mapDynamicData.clear();


	ReadFile(hFile, &dwMapSize, sizeof(DWORD), &dwbyte, nullptr);
	for (_uint i = 0; i < dwMapSize; i++)
	{
		map<_uint, MESH> mapNavi;
		for (_uint i = 0; i < 3; i++)
		{
			ReadFile(hFile, &tMesh, sizeof(MESH), &dwbyte, nullptr);
			mapNavi.emplace(i, tMesh);
		}
		m_mapNaviData.emplace(i, mapNavi);
	}


	ReadFile(hFile, &dwMapSize, sizeof(DWORD), &dwbyte, nullptr);
	for (_uint i = 0; i < dwMapSize; i++)
	{
		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		pBuf = new TCHAR[dwStringSize];
		ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
		ReadFile(hFile, &dwObjSize, sizeof(DWORD), &dwbyte, nullptr);
		map<wstring, MESH> mapStatic;
		for (_uint i = 0; i < dwObjSize; i++)
		{
			ReadFile(hFile, &tMesh, sizeof(MESH), &dwbyte, nullptr);
			wstring wstrKey = pBuf + to_wstring(i);

			mapStatic.emplace(wstrKey, tMesh);
		}

		m_mapStaticData.emplace(pBuf, mapStatic);
		Safe_Delete(pBuf);
	}

	ReadFile(hFile, &dwMapSize, sizeof(DWORD), &dwbyte, nullptr);
	for (_uint i = 0; i < dwMapSize; i++)
	{
		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
		pBuf = new TCHAR[dwStringSize];
		ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
		ReadFile(hFile, &dwObjSize, sizeof(DWORD), &dwbyte, nullptr);
		map<wstring, MESH> mapDynamic;
		for (_uint i = 0; i < dwObjSize; i++)
		{
			ReadFile(hFile, &tMesh, sizeof(MESH), &dwbyte, nullptr);
			wstring wstrKey = pBuf + to_wstring(i);
			mapDynamic.emplace(wstrKey, tMesh);
		}
		m_mapDynamicData.emplace(pBuf, mapDynamic);
		Safe_Delete(pBuf);
	}

	CloseHandle(hFile);
	return S_OK;
}

map<wstring,CGameObject*> CLoadMgr::SpawnData()
{
	CGameObject* pCol = nullptr;
	CGameObject* pGameObject = nullptr;
	wstring LayerTag;
	_vec3* vNavePos = nullptr; 


	for (auto& iter : m_mapStaticData)
	{
		for (auto& iter_Second : iter.second)
		{
			//생성
			pGameObject =  CSpawnMgr::GetInstance()->Spawn(iter.first.c_str(), iter_Second.second, &LayerTag);
			NULL_CHECK_MSG(pGameObject, L"로드 스태틱 데이터 실패");
			SetTransform(dynamic_cast<CTransform*>(pGameObject->Get_Component(L"Com_Transform", ID_STATIC)), iter_Second.second);
 
			auto& iter_find = m_mapMeshCollider.find(iter.first);
			if (iter_find != m_mapMeshCollider.end())
			{
				//컬라이더 부착
				for (auto& iter_find_second : iter_find->second)
				{
					USES_CONVERSION;
					//레이어는 뒤에 메쉬 명 붙여서
					const _tchar* pConvLayerTag = W2BSTR(LayerTag.c_str());
					const _tchar* pConvObjTag = W2BSTR((iter_find_second.first + iter_find->first).c_str());
					pCol = CSpawnMgr::GetInstance()->Spawn(pGameObject, iter_find_second.second);
					//오브젝트 맵에 따로 넣고 
					pGameObject->EmplaceCol(iter_find_second.first, pCol);
					Add_GameObject(pConvLayerTag, pConvObjTag, pCol);
					//m_mapHead.emplace((iter_find->first +iter_find_second.first), pCol);	//레이어에 추가 
				}
			}
			
			//레이어에 추가할 데이터 저장 
			//m_mapHead.emplace(iter_Second.first,pGameObject);
		}
	}

	_uint idx = 0;
	for (auto& iter : m_mapDynamicData)
	{
		for (auto& iter_Second : iter.second)
		{
			pGameObject = CSpawnMgr::GetInstance()->Spawn(iter.first, iter_Second.second, &LayerTag);
			NULL_CHECK_MSG(pGameObject, L"로드 다이나믹 데이터 실패");
			SetTransform(dynamic_cast<CTransform*>( pGameObject->Get_Component(L"Com_Transform", ID_DYNAMIC)),iter_Second.second);

			//해당 매시의 컬라이더가 있으면 
			auto& iter_find = m_mapMeshCollider.find(iter.first);
			if (iter_find != m_mapMeshCollider.end())
			{
				//컬라이더 부착
				for (auto& iter_find_second : iter_find->second)
				{
					USES_CONVERSION;
					const _tchar* pConvLayerTag = W2BSTR(LayerTag.c_str());
					const _tchar* pConvObjTag = W2BSTR((iter_find_second.first + iter_find->first+ to_wstring(idx)).c_str());
					pCol = CSpawnMgr::GetInstance()->Spawn(pGameObject, iter_find_second.second);

					//오브젝트 맵에 따로 넣고 
					pGameObject->EmplaceCol(iter_find_second.first, pCol);
					//오브젝트 레이어에 맞춰서 생성 
					Add_GameObject(pConvLayerTag, pConvObjTag, pCol);
					//m_mapHead.emplace((iter_find->first+iter_find_second.first),pCol);	//레이어에 추가 
				}
			}

			//레이어에 추가할 데이터 저장 
			//m_mapHead.emplace(iter_Second.first, pGameObject);
			idx++;
		}
	}
	
	//네비매시 추가 
	//CComponent* pComponent = CNaviMesh::Create(CGameMgr::GetInstance()->GetDevice(), m_mapNaviData);

	Ready_Prototype(L"Proto_Navi", CNaviMesh::Create(CGameMgr::GetInstance()->GetDevice(), m_mapNaviData));

	CComponent* pComponent = dynamic_cast<CNaviMesh*>(Clone_Prototype(L"Proto_Navi"));

	//CGameMgr::GetInstance()->SetNavi(dynamic_cast<CNaviMesh*>(pComponent));
	dynamic_cast<CPlayer*> (CGameMgr::GetInstance()->GetPlayer())->Set_NaviMesh(dynamic_cast<CNaviMesh*>(pComponent));
	//CGameMgr::GetInstance()->GetPlayer()->Set_Component(L"Com_Navi", pComponent,ID_STATIC);

	return m_mapHead;
}

void CLoadMgr::SetTransform(CTransform * pTransform, MESH tMesh)
{
	pTransform->Set_Scale(&tMesh.vScale);
	pTransform->Set_Rot(&tMesh.vRot);
	pTransform->Set_Pos(&tMesh.vPos);
	pTransform->Update_Component(0.f);
}




void CLoadMgr::Free(void)
{

}


