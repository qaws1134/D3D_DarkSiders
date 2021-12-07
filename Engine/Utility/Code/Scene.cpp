#include "Scene.h"
#include "ColMgr.h"
USING(Engine)

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CScene::~CScene(void)
{

}

HRESULT Engine::CScene::Ready_Scene(void)
{
	return S_OK;
}

Engine::_int Engine::CScene::Update_Scene(const _float& fTimeDelta)
{
	_int iResult = 0;

	for (auto& iter : m_mapLayer)
	{
		iResult = iter.second->Update_Layer(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}
	if (GetMapObj(L"Player") != nullptr &&GetMapObj(L"Enemy") != nullptr)
	{
		CColMgr::Col_Body(CHECK_BODY_BODY, *GetMapObj(L"Player"), *GetMapObj(L"Enemy"), MESH_DYNAMIC);
		CColMgr::Col_Body(CHECK_WEAPON, *GetMapObj(L"Player"), *GetMapObj(L"Enemy"), MESH_DYNAMIC);
		CColMgr::Col_Body(CHECK_WEAPON, *GetMapObj(L"Enemy"), *GetMapObj(L"Player"), MESH_DYNAMIC);
	}
	if (GetMapObj(L"Player") != nullptr &&GetMapObj(L"Bullet_Enemy") != nullptr)
	{
		CColMgr::Col_Body(CHECK_BULLET, *GetMapObj(L"Bullet_Enemy"), *GetMapObj(L"Player"), MESH_DYNAMIC);
		//CColMgr::Col_Body(CHECK_BULLET, *GetMapObj(L"Bullet_Player"), *GetMapObj(L"Enemy"), MESH_DYNAMIC);
	}
	if (GetMapObj(L"Enemy") != nullptr &&GetMapObj(L"Bullet_Player") != nullptr)
	{
		CColMgr::Col_Body(CHECK_BULLET, *GetMapObj(L"Bullet_Player"), *GetMapObj(L"Enemy"), MESH_DYNAMIC);
		//CColMgr::Col_Body(CHECK_BULLET, *GetMapObj(L"Bullet_Player"), *GetMapObj(L"Enemy"), MESH_DYNAMIC);
	}
	if (GetMapObj(L"Player") != nullptr &&GetMapObj(L"Orb") != nullptr)
	{
		CColMgr::Col_Body(CHECK_ORB, *GetMapObj(L"Orb"), *GetMapObj(L"Player"), MESH_DYNAMIC);
		//CColMgr::Col_Body(CHECK_BULLET, *GetMapObj(L"Bullet_Player"), *GetMapObj(L"Enemy"), MESH_DYNAMIC);
	}
	if (GetMapObj(L"Player") != nullptr &&GetMapObj(L"Item") != nullptr)
	{
		CColMgr::Col_Body(CHECK_ITEM, *GetMapObj(L"Player"), *GetMapObj(L"Item"), MESH_STATIC);
	}
	if (GetMapObj(L"Player") != nullptr &&GetMapObj(L"Chest") != nullptr)
	{
		CColMgr::Col_Body(CHECK_INTERACTION, *GetMapObj(L"Player"), *GetMapObj(L"Chest"), MESH_DYNAMIC);
		CColMgr::Col_Body(CHECK_WEAPON, *GetMapObj(L"Player"), *GetMapObj(L"Chest"), MESH_DYNAMIC);
	}
	if (GetMapObj(L"Player") != nullptr &&GetMapObj(L"NPC") != nullptr)
	{
		CColMgr::Col_Body(CHECK_INTERACTION, *GetMapObj(L"Player"), *GetMapObj(L"NPC"), MESH_DYNAMIC);
	}
	if (GetMapObj(L"Player") != nullptr &&GetMapObj(L"Serpent") != nullptr)
	{
		CColMgr::Col_Body(CHECK_INTERACTION, *GetMapObj(L"Player"), *GetMapObj(L"Serpent"), MESH_DYNAMIC);
	}
	if (GetMapObj(L"Player") != nullptr &&GetMapObj(L"JumpBall") != nullptr)
	{
		CColMgr::Col_Body(CHECK_JUMPBAll, *GetMapObj(L"Player"), *GetMapObj(L"JumpBall"), MESH_DYNAMIC);
	}


	return iResult;
}

void Engine::CScene::Render_Scene(void)
{
	for (auto& iter : m_mapLayer)
		iter.second->Render_Layer();
}

void CScene::Begin_Scene(void)
{
	m_bBegin = true;
}

CLayer* CScene::GetLayer(wstring LayerTag)
{
	if (m_mapLayer.empty())
		return nullptr;
	auto iter_find = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(LayerTag.c_str()));
	if (iter_find == m_mapLayer.end())
		return nullptr;
	return iter_find->second;
}

void CScene::Add_Layer(map<const _tchar*, CLayer*>& mapLayer)
{
	for (auto iter : mapLayer)
	{
		m_mapLayer.emplace(iter.first, iter.second);
	}
}

map<const _tchar*, CGameObject*>* CScene::GetMapObj(wstring LayerTag)
{
	//find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(LayerTag.c_str()));

	auto iter_find = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(LayerTag.c_str()));
	if (iter_find == m_mapLayer.end())
		return nullptr;
	return  &(iter_find->second->GetMapObj());
}

void CScene::EraseMapObj(wstring LayerTag)
{
	if (m_mapLayer.empty())
		return;
	auto iter_find = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(LayerTag.c_str()));
	if (iter_find == m_mapLayer.end())
		return;

	m_mapLayer.erase(iter_find->first);
}
//void CScene::Release_SaveLayer()
//{
//	//for_each(m_mapSaveLayer.begin(), m_mapSaveLayer.end(), CDeleteMap());
//	//m_mapSaveLayer.clear();
//}

void Engine::CScene::Free(void)
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());

	m_mapLayer.clear();
	//m_mapSaveLayer.clear();
	Safe_Release(m_pGraphicDev);
}

Engine::CComponent* Engine::CScene::Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_Component(pObjTag, pComponentTag, eID);
}

CGameObject * CScene::Get_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	auto	iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_GameObject(pObjTag);
}

void CScene::Add_GameObject(const _tchar * pLayerTag, const _tchar* pObjTag, CGameObject* pInstance)
{
	auto	iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_mapLayer.end())
	{
		CLayer* pLayer = CLayer::Create();
		pLayer->Add_GameObject(pObjTag, pInstance);
		m_mapLayer.emplace(pLayerTag, pLayer);
		return;
	}
	iter->second->Add_GameObject(pObjTag, pInstance);
}

