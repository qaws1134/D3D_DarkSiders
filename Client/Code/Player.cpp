#include "stdafx.h"
#include "Player.h"

#include "Export_Function.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CPlayer::CPlayer(const CPlayer& rhs)
	: CGameObject(rhs)
{

}

CPlayer::~CPlayer(void)
{

}

HRESULT CPlayer::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);

	_int iExit = CGameObject::Update_Object(fTimeDelta);

	return iExit;
}

void CPlayer::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture();

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer*	pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Free(void)
{
	CGameObject::Free();
}

HRESULT CPlayer::Add_Component(void)
{
	CComponent*		pComponent = nullptr;
	
	// buffer
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Prototype(L"Proto_Buffer_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Prototype(L"Proto_Texture_Player"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
	m_pTransformCom->Get_INFO(INFO_UP, &m_vDir);

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTransformCom->Move_Pos(&m_vDir, 5.f, fTimeDelta);
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTransformCom->Move_Pos(&m_vDir, -5.f, fTimeDelta);
	}

	if (GetAsyncKeyState('Q') & 0x8000)
		m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f * fTimeDelta));
	
	if (GetAsyncKeyState('A') & 0x8000)
		m_pTransformCom->Rotation(ROT_X, D3DXToRadian(-90.f * fTimeDelta));

	if (GetAsyncKeyState('W') & 0x8000)
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(90.f * fTimeDelta));
	
	if (GetAsyncKeyState('S') & 0x8000)
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-90.f * fTimeDelta));

	if (GetAsyncKeyState('E') & 0x8000)
		m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(90.f * fTimeDelta));
	
	if (GetAsyncKeyState('D') & 0x8000)
		m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(-90.f * fTimeDelta));

}

