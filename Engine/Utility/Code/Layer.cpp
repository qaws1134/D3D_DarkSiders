#include "Layer.h"

USING(Engine)

Engine::CLayer::CLayer(void)
{

}

Engine::CLayer::~CLayer(void)
{

}

HRESULT Engine::CLayer::Add_GameObject(const _tchar* pObjTag, CGameObject* pInstance)
{
	if (nullptr == pInstance)
		return E_FAIL;

	m_mapObject.emplace(pObjTag, pInstance);

	return S_OK;
}


HRESULT Engine::CLayer::Ready_Layer(void)
{
	return S_OK;
}

int Engine::CLayer::Update_Layer(const _float& fTimeDelta)
{
	_int	iResult = 0;

	for (auto& iter : m_mapObject)
	{
		iResult = iter.second->Update_Object(fTimeDelta);

		if (iResult & 0x8000000)
			return iResult;
	}

	return iResult;
}

void Engine::CLayer::Render_Layer(void)
{
	for (auto& iter : m_mapObject)
	{
		
		iter.second->Render_Object();
	}
}

CLayer* Engine::CLayer::Create(void)
{
	CLayer*		pInstance = new CLayer;

	if (FAILED(pInstance->Ready_Layer()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CLayer::Free(void)
{
	for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteMap());
	m_mapObject.clear();
}

Engine::CComponent* Engine::CLayer::Get_Component(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second->Get_Component(pComponentTag, eID);
}

CGameObject * CLayer::Get_GameObject(const _tchar * pObjTag)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second;
}

