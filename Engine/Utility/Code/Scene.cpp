#include "Scene.h"

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

void Engine::CScene::Free(void)
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();

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
		return;
	iter->second->Add_GameObject(pObjTag, pInstance);
}

