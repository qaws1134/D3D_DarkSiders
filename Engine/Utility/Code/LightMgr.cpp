#include "LightMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CLightMgr)

Engine::CLightMgr::CLightMgr(void)
{

}

Engine::CLightMgr::~CLightMgr(void)
{
	Free();
}

HRESULT Engine::CLightMgr::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLightInfo,
	const _uint& iIndex)
{
	CLight*		pLight = CLight::Create(pGraphicDev, pLightInfo, iIndex);
	NULL_CHECK_RETURN(pLight, E_FAIL);

	m_LightList.push_back(pLight);

	return S_OK;
}

void Engine::CLightMgr::Free(void)
{
	for_each(m_LightList.begin(), m_LightList.end(), CDeleteObj());
	m_LightList.clear();
}

const D3DLIGHT9* Engine::CLightMgr::Get_Light(const _uint& iIndex /*= 0*/)
{
	auto	iter = m_LightList.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Light();
}

void Engine::CLightMgr::Render_Light(LPD3DXEFFECT& pEffect)
{
	for (auto& iter : m_LightList)
		iter->Render_Light(pEffect);
}

