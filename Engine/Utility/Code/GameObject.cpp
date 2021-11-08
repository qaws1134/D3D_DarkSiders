#include "GameObject.h"

USING(Engine)

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CGameObject::CGameObject(const CGameObject& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CGameObject::~CGameObject(void)
{

}

HRESULT Engine::CGameObject::Ready_Object(void)
{
	return S_OK;
}

void CGameObject::Late_Ready_Object(void)
{


}


Engine::_int Engine::CGameObject::Update_Object(const _float& fTimeDelta)
{
	_int iResult = 0;

	for (auto& iter : m_mapComponent[ID_DYNAMIC])
	{
		iResult = iter.second->Update_Component(fTimeDelta);

		if (iResult & 0x80000000)
			return -1;
	}

	return iResult;
}

Engine::CComponent * Engine::CGameObject::Find_Component(const _tchar * pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_mapComponent[eID].begin(), m_mapComponent[eID].end(), CTag_Finder(pComponentTag));

	if (iter == m_mapComponent[eID].end())
		return nullptr;

	return iter->second;
}

Engine::CComponent* Engine::CGameObject::Get_Component(const _tchar* pComponentTag, COMPONENTID eID)
{
	CComponent*		pComponent = Find_Component(pComponentTag, eID);

	if (nullptr == pComponent)
		return nullptr;

	return pComponent;
}

void Engine::CGameObject::Compute_ViewZ(const _vec3 * pPos)
{
	_matrix		matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

	_vec3	vCamPos;
	memcpy(&vCamPos, &matCamWorld.m[3][0], sizeof(_vec3));

	m_fViewZ = D3DXVec3Length(&(vCamPos - *pPos));
}

void CGameObject::SetPos(_vec3 vPos, COMPONENTID eID)
{
	CComponent*		pComponent = Find_Component(L"Com_Transform", eID);

	if (nullptr == pComponent)
		return;

	dynamic_cast<CTransform*>(pComponent)->Set_Pos(&vPos);

}

void CGameObject::SetZPos(_float fzPos, COMPONENTID eID)
{
	CComponent*		pComponent = Find_Component(L"Com_Transform", eID);

	if (nullptr == pComponent)
		return ;

	_vec3 vPos;
	dynamic_cast<CTransform*>(pComponent)->Get_INFO(INFO_POS,&vPos);
	dynamic_cast<CTransform*>(pComponent)->Set_Pos(vPos.x, vPos.y ,fzPos);

}

_float CGameObject::GetWorldZ(COMPONENTID eID)
{
	CComponent*		pComponent = Find_Component(L"Com_Transform", eID);

	if (nullptr == pComponent)
		return 0.f;

	_vec3 vPos;
	dynamic_cast<CTransform*>(pComponent)->Get_INFO(INFO_POS, &vPos);
	return vPos.z;
}

void CGameObject::Render_Object(void)
{

}


void Engine::CGameObject::Free(void)
{
	for (_uint i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	Safe_Release(m_pGraphicDev);
}

