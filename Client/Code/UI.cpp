#include "stdafx.h"
#include "UI.h"
#include "Player.h"
#include "Export_Function.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CUI::CUI(const CUI& rhs)
	: CGameObject(rhs)
	,m_tInfo(rhs.m_tInfo)
	,m_tRcUI(rhs.m_tRcUI)
{

}

CUI::~CUI(void)
{

}

HRESULT CUI::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_tRcUI.left	= (LONG)(m_tInfo.vPos.x - m_tInfo.vSize.x*0.8f);
	m_tRcUI.top		= (LONG)(m_tInfo.vPos.y - m_tInfo.vSize.y*0.8f);
	m_tRcUI.right	= (LONG)(m_tInfo.vPos.x + m_tInfo.vSize.x*0.8f);
	m_tRcUI.bottom  = (LONG)(m_tInfo.vPos.y + m_tInfo.vSize.y*0.8f);


	m_pTransformCom->Set_Scale(0.1f, 0.1f, 0.1f);
	m_pTransformCom->Update_Component(0.f);
	//m_pTarget = Engine::Get_GameObject(L"GameLogic",L"Player");
	//NULL_CHECK_RETURN(m_pTarget, E_FAIL);
	return S_OK;
}

_int CUI::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);
	if (m_bActive)
	{
		D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

		UI_ElementUpdate(fTimeDelta);

		Add_RenderGroup(RENDER_UI, this);
	}
	return iExit;
}

void CUI::Render_Object(void)
{

	LPD3DXEFFECT	 pEffect = m_pShaderCom->Get_EffectHandle();
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iMaxPass = 0;

	pEffect->Begin(&iMaxPass, NULL);


	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();


	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matOldProj);

	pEffect->EndPass();

	pEffect->End();
	Safe_Release(pEffect);
	
}

CUI* CUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI*	pInstance = new CUI(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

CUI * CUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, UISET tInfo,_bool bActive)
{
	CUI*	pInstance = new CUI(pGraphicDev);
	pInstance->SetUI(tInfo);
	pInstance->SetActive(bActive);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CUI::Free(void)
{
	CGameObject::Free();
}

HRESULT CUI::Add_Component(void)
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


HRESULT CUI::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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
	


	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", m_tInfo.iTextureNum);

	
	return S_OK;
}

void CUI::UI_ElementUpdate(const _float& fTimeDelta)
{
	if (m_tInfo.wstrTexture == L"Proto_Texture_Element")
	{
	
		if (m_pCalculatorCom->Picking_OnUI(g_hWnd, m_tRcUI))
		{
			switch (m_tInfo.iTextureNum)
			{
			case UI::ELEMENT_00:
			{
				if (Key_Down(KEY_LBUTTON))
				{
					//UI창 닫고 속성 적용 
					dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_EARTH);
				}
				dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_EARTH);
				
			}
			break;
			case UI::ELEMENT_01:
			{
				if (Key_Down(KEY_LBUTTON))
				{
					//UI창 닫고 속성 적용 
					dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_FLAME);
				}
				dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_FLAME);
			}

			break;
			case UI::ELEMENT_02:
			{
				if (Key_Down(KEY_LBUTTON))
				{
					//UI창 닫고 속성 적용 
					dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_LIGHTNING);
				}
				dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_LIGHTNING);
			}
			break;
			case UI::ELEMENT_03:
			{
				if (Key_Down(KEY_LBUTTON))
				{
					//UI창 닫고 속성 적용 
					dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_DEATH);
				}
				dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_DEATH);
			}
			break;
			case UI::ELEMENT_04:
			{
				if (Key_Down(KEY_LBUTTON))
				{
					//UI창 닫고 속성 적용 
					dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_WIND);
				}
				dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_WIND);
			}
			break;
			case UI::ELEMENT_05:
			{
				if (Key_Down(KEY_LBUTTON))
				{
					//UI창 닫고 속성 적용 
					dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_VAMP);
				}
				dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_VAMP);
			}
			break;
			case UI::ELEMENT_END:
				break;
			}
		}
	}
}

void CUI::SetUI(UISET tInfo)
{
	m_tInfo.vPos = tInfo.vPos;
	m_tInfo.vSize = tInfo.vSize;
	m_tInfo.iTextureNum = tInfo.iTextureNum;
	m_tInfo.wstrObjTag = tInfo.wstrObjTag.c_str();
	m_tInfo.wstrTexture = tInfo.wstrTexture.c_str();

	CComponent*		pComponent = nullptr;
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Prototype(m_tInfo.wstrTexture.c_str()));
	NULL_CHECK_RETURN(m_pTextureCom, );
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", pComponent);
	

}

