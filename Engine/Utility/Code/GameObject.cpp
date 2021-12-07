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

_bool CGameObject::HitTimer(_float fDeltaTime)
{
	m_fHitSpeed += fDeltaTime;
	if (m_fHitSpeed > m_fHitTime)
	{
		return false;
	}
	return true;
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
	m_bHit= HitTimer(fTimeDelta);
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
	{
		iResult = iter.second->Update_Component(fTimeDelta);

		if (iResult & 0x80000000)
			return -1;
	}

	return iResult;
}

void CGameObject::EmplaceCol(wstring ObjTag, CGameObject * pGameObject)
{
	USES_CONVERSION;
	const _tchar* pConvObjTag = W2BSTR(ObjTag.c_str());
	m_mapColider.emplace(pConvObjTag, pGameObject);
}

void CGameObject::SetCharInfo(_float fHp, _float fAtk)
{
	m_tCharInfo.fAtk = fAtk;
	m_tCharInfo.fInitAtk = fAtk;

	m_tCharInfo.fMaxHp = fHp;
	m_tCharInfo.fHp = fHp;

	m_tCharInfo.fDmg = 0.f;
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
	if (!m_pGraphicDev)
		return;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

	_vec3	vCamPos;
	memcpy(&vCamPos, &matCamWorld.m[3][0], sizeof(_vec3));

	m_fViewZ = D3DXVec3Length(&(vCamPos - *pPos));
}

_vec3 * Engine::CGameObject::GetPos(COMPONENTID eID)
{
	CComponent*		pComponent = Find_Component(L"Com_Transform", eID);

	if (nullptr == pComponent)
		return nullptr;

	dynamic_cast<CTransform*>(pComponent)->Get_INFO(INFO_POS,&m_vPos);
	
	return &m_vPos;
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

void CGameObject::Set_Component(const _tchar * pComTag, CComponent * pComponent, COMPONENTID eID)
{
	auto iter_find = m_mapComponent[eID].find(pComTag);
	if (iter_find != m_mapComponent[eID].end())
		return;

	m_mapComponent[eID].emplace(pComTag, pComponent);
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

