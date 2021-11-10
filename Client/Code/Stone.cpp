#include "stdafx.h"
#include "Stone.h"
#include "Player.h"
#include "Export_Function.h"

CStone::CStone(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CStone::CStone(const CStone& rhs)
	: CGameObject(rhs)
{

}

CStone::~CStone(void)
{

}

HRESULT CStone::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	return S_OK;
}

_int CStone::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);

	return iExit;
}

void CStone::Render_Object(void)
{

	LPD3DXEFFECT	 pEffect = m_pShaderCom->Get_EffectHandle();
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iMaxPass = 0;

	pEffect->Begin(&iMaxPass, NULL);


	pEffect->BeginPass(m_iPassIdx);

	m_pBufferCom->Render_Buffer();


	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matOldProj);

	pEffect->EndPass();

	pEffect->End();
	Safe_Release(pEffect);
	
}

CStone* CStone::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStone*	pInstance = new CStone(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

CStone * CStone::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iCreature)
{
	CStone*	pInstance = new CStone(pGraphicDev);
	pInstance->Set_CreatureInfo(iCreature);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}


void CStone::Free(void)
{
	CGameObject::Free();
}

HRESULT CStone::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Prototype(L"Proto_Buffer_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(m_pRendererCom, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);
	
	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Calculator", pComponent);
	
	// Shader
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Prototype(L"Proto_Shader_UI"));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);


	return S_OK;
}

void CStone::Set_CreatureInfo(_uint iCreatureNum)
{
}


HRESULT CStone::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
{
	_matrix		matWorld, matView;


	m_pGraphicDev->GetTransform(D3DTS_VIEW, &m_matOldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &m_matOldProj);

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matWorld._11 = m_tInfo.vSize.x;  
	matWorld._22 = m_tInfo.vSize.y;
	matWorld._33 = 1.f;
	matWorld._41 =  m_tInfo.vPos.x - WINCX * 0.5f;
	matWorld._42 = -m_tInfo.vPos.y + WINCY * 0.5f;
	

	_matrix matTransformWorld = *m_pTransformCom->Get_WorldMatrix();
	matWorld =  matTransformWorld*matWorld;

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);
	//pEffect->SetFloat("g_SizeX", m_tInfo.vSize.x);
	//pEffect->SetFloat("g_SizeY", m_tInfo.vSize.y);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", m_tInfo.iTextureNum);
	return S_OK;
}
