#include "stdafx.h"
#include "UI_WeaponElement.h"

#include "Export_Function.h"

CUI_WeaponElement::CUI_WeaponElement(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CUI_WeaponElement::CUI_WeaponElement(const CUI_WeaponElement& rhs)
	: CGameObject(rhs)
{

}

CUI_WeaponElement::~CUI_WeaponElement(void)
{

}

HRESULT CUI_WeaponElement::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fX = 400.f;
	m_fY = 550.f;

	m_fSizeX = 800.f;
	m_fSizeY = 150.f;

	return S_OK;
}

_int CUI_WeaponElement::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CUI_WeaponElement::Render_Object(void)
{
	//_matrix		matWorld, matView, matOldView, matOldProj;

	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	//D3DXMatrixIdentity(&matWorld);
	//D3DXMatrixIdentity(&matView);

	//matWorld._11 = m_fSizeX;  
	//matWorld._22 = m_fSizeY;
	//matWorld._33 = 1.f;
	//matWorld._41 = m_fX - WINCX * 0.5f;
	//matWorld._42 = -m_fY + WINCY * 0.5f;
	//
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	//m_pTextureCom->Set_Texture();
	//m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());



	m_pTextureCom->Set_Texture();


	m_pBufferCom->Render_Buffer();
}

CUI_WeaponElement* CUI_WeaponElement::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_WeaponElement*	pInstance = new CUI_WeaponElement(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CUI_WeaponElement::Free(void)
{
	CGameObject::Free();
}

HRESULT CUI_WeaponElement::Add_Component(void)
{
	CComponent*		pComponent = nullptr;
	
	// buffer
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Prototype(L"Proto_Buffer_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Prototype(L"Proto_Texture_UI"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(m_pRendererCom, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);
	
	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

