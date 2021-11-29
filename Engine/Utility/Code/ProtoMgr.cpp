#include "ProtoMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CProtoMgr)

Engine::CProtoMgr::CProtoMgr(void)
{

}

Engine::CProtoMgr::~CProtoMgr(void)
{
	Free();
}

HRESULT Engine::CProtoMgr::Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance)
{
	CComponent*		pComponent = Find_Prototype(pProtoTag);

	if (nullptr != pComponent)
	{
		Safe_Release(pComponent);
		m_mapPrototype.erase(pProtoTag);
	}
	m_mapPrototype.emplace(pProtoTag, pInstance);

	return S_OK;
}

Engine::CComponent* Engine::CProtoMgr::Clone_Prototype(const _tchar* pProtoTag)
{
	CComponent*		pComponent = Find_Prototype(pProtoTag);

	if (nullptr == pComponent)
		return nullptr;

	return pComponent->Clone();
}


Engine::CComponent* Engine::CProtoMgr::Find_Prototype(const _tchar* pProtoTag)
{
	auto	iter = find_if(m_mapPrototype.begin(), m_mapPrototype.end(), CTag_Finder(pProtoTag));

	if (iter == m_mapPrototype.end())
		return nullptr;

	return iter->second;
}

void Engine::CProtoMgr::Free(void)
{
	for_each(m_mapPrototype.begin(), m_mapPrototype.end(), CDeleteMap());
	m_mapPrototype.clear();
}

