#include "stdafx.h"
#include "UI.h"
#include "Player.h"
#include "GameMgr.h"
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

	

	if (m_tInfo.wstrTexture == L"Proto_Texture_CoreTree_StoneList_Sel")
	{
		m_tInfo.vPos.x += 300.f;
		m_tInfo.vSize.x += 600.f;
		m_tRcUI.left = (LONG)(m_tInfo.vPos.x - m_tInfo.vSize.x*0.5f);
		m_tRcUI.top = (LONG)(m_tInfo.vPos.y - m_tInfo.vSize.y*0.5f);
		m_tRcUI.right = (LONG)(m_tInfo.vPos.x + m_tInfo.vSize.x*0.5f);
		m_tRcUI.bottom = (LONG)(m_tInfo.vPos.y + m_tInfo.vSize.y*0.5f);
	
	}
	else
	{
		m_tRcUI.left	= (LONG)(m_tInfo.vPos.x - m_tInfo.vSize.x*0.5f);
		m_tRcUI.top		= (LONG)(m_tInfo.vPos.y - m_tInfo.vSize.y*0.5f);
		m_tRcUI.right	= (LONG)(m_tInfo.vPos.x + m_tInfo.vSize.x*0.5f);
		m_tRcUI.bottom  = (LONG)(m_tInfo.vPos.y + m_tInfo.vSize.y*0.5f);
	}
	m_iPassIdx = 0;		//���� �ؽ��İ� �þ �� ���� �н� ���� 

	m_pTransformCom->Set_Scale(&_vec3{ 1.f,1.f,1.f });
	m_pTransformCom->Update_Component(0.f);
	
	if (m_tInfo.wstrTexture == L"Proto_Texture_Element")
		m_pTarget = CGameMgr::GetInstance()->GetPlayer();
	
	m_fScaleSize = 1.f;
	return S_OK;
}

_int CUI::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);
	if (m_bActive)
	{
		D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

		UI_ElementUpdate(fTimeDelta);
		UI_CoreTreeUpdate(fTimeDelta);
		UI_StoneListUpdate(fTimeDelta);
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


	pEffect->BeginPass(m_iPassIdx);

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

CUI * CUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, UISET tInfo,_bool bActive, UI::TYPE eType)
{
	CUI*	pInstance = new CUI(pGraphicDev);
	pInstance->SetUI(tInfo);
	pInstance->SetActive(bActive);
	pInstance->SetType(eType);
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
	

	
	//m_pTransformCom->Set_Pos(&_vec3{ m_tInfo.vPos.x,m_tInfo.vPos.y,m_fViewZ });
	_matrix matTransformWorld = *m_pTransformCom->Get_WorldMatrix();
	matWorld =  matTransformWorld*matWorld;

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);
	//pEffect->SetFloat("g_SizeX", m_tInfo.vSize.x);
	//pEffect->SetFloat("g_SizeY", m_tInfo.vSize.y);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", m_tInfo.iTextureNum);

	if(m_bSubTex1)
		m_pSubTextureCom1->Set_Texture(pEffect, "g_SubTexture1", m_iSubTexNum1);
	if(m_bSubTex2)
		m_pSubTextureCom2->Set_Texture(pEffect, "g_SubTexture2", m_iSubTexNum2);

	return S_OK;
}

void CUI::UI_ElementUpdate(const _float& fTimeDelta)
{
	if (m_tInfo.wstrTexture == L"Proto_Texture_Element_Arrow"|| m_tInfo.wstrTexture == L"Proto_Texture_Element_Arrow_Bg")
	{
		POINT		ptMouse{};

		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		_vec3 vMousPos = _vec3{ (_float)ptMouse.x, (_float)ptMouse.y ,0.f};
		_vec3 vCenterPos = _vec3{ WINCX*0.5f,WINCY*0.5f,0.f };


		_vec3 vDir = vMousPos - vCenterPos;
		_vec3 vRight = _vec3{ 0.f,-1.f,0.f };
		D3DXVec3Normalize(&vDir, &vDir);

		_float fCos = D3DXVec3Dot(&vDir, &vRight);
		_float fAngle = D3DXToDegree(acosf(fCos));	
	
		if (vMousPos.x < WINCX*0.5f)			
			fAngle = 360.f-fAngle;

		if (m_tInfo.wstrTexture == L"Proto_Texture_Element_Arrow_Bg")
		{
			for (_uint i = 0; i < 6; i++)
			{
				if (fAngle < _float(i + 1) * 60.f)
				{
					fAngle = (_float)i*60.f;
					break;
				}
			}
			fAngle += 30.f;
		}
		fAngle *= -1.f;
		m_pTransformCom->Set_Rot(&_vec3{ 0.f,0.f,D3DXToRadian(fAngle) });

	}

	if (m_tInfo.wstrTexture == L"Proto_Texture_Element")
	{
		////////////////////////////////////////////////
		//���õǸ� ���̴��� ����ȿ�� �ְ�ʹ� 

		if (m_pCalculatorCom->Picking_OnUI(g_hWnd, m_tRcUI))
		{
			switch (m_tInfo.iTextureNum)
			{
			case UI::ELEMENT_00:
			{
				if (Key_Down(KEY_LBUTTON))
				{
					//UIâ �ݰ� �Ӽ� ���� 
					dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_EARTH);
					break;
				}
				dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_EARTH);
				
			}
			break;
			case UI::ELEMENT_01:
			{
				if (Key_Down(KEY_LBUTTON))
				{
					//UIâ �ݰ� �Ӽ� ���� 
					dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_FLAME);
					break;
				}
				dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_FLAME);
			}

			break;
			case UI::ELEMENT_02:
			{
				if (Key_Down(KEY_LBUTTON))
				{
					//UIâ �ݰ� �Ӽ� ���� 
					dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_LIGHTNING);
					break;
				}
				dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_LIGHTNING);
			}
			break;
			case UI::ELEMENT_03:
			{
				if (Key_Down(KEY_LBUTTON))
				{
					//UIâ �ݰ� �Ӽ� ���� 
					dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_DEATH);
					break;
				}
				dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_DEATH);
			}
			break;
			case UI::ELEMENT_04:
			{
				if (Key_Down(KEY_LBUTTON))
				{
					//UIâ �ݰ� �Ӽ� ���� 
					dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_WIND);
					break;
				}
				dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_WIND);
			}
			break;
			case UI::ELEMENT_05:
			{
				if (Key_Down(KEY_LBUTTON))
				{
					//UIâ �ݰ� �Ӽ� ���� 
					dynamic_cast<CPlayer*> (m_pTarget)->Set_Element(War::ELEMENT_VAMP);
					break;
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

void CUI::UI_CoreTreeUpdate(const _float & fTimeDelta)
{
	if (m_tInfo.wstrTexture == L"Proto_Texture_CoreTree_Core")
	{
		if (m_pCalculatorCom->Picking_OnUI(g_hWnd, m_tRcUI))
		{
			dynamic_cast<CUI*>(Get_GameObject(L"UI", L"UI_CoreTree_Sel"))->SetUIPos(m_tInfo.vPos);

			//�� UI �� ������ �ִ� ���� �����ؾߵ� - > ������ ��UI ���� ���� ���̽� ���� �ø���
			//�� UI�� ���� ������Ʈ ���� 

			if (Key_Down(KEY_LBUTTON))
			{


			}
			if (Key_Down(KEY_RBUTTON))
			{
				//�� ���� 

			}

		}
	}

	if (m_tInfo.wstrTexture == L"Proto_Texture_CoreTree_Sel")
	{
		//1.3�� �Ǹ� ����
		//0.9�� �Ǹ� ����

		if (m_fScaleSize < 0.9f)
			m_bScale = true;
		else if(m_fScaleSize > 1.1f)
			m_bScale = false;

		if (m_bScale)
			m_fScaleSize += fTimeDelta;
		else
			m_fScaleSize -= fTimeDelta;

		m_pTransformCom->Set_Scale(m_fScaleSize, m_fScaleSize, m_fScaleSize);

	}


}

void CUI::UI_StoneListUpdate(const _float & fTimeDelta)
{
	if (m_tInfo.wstrTexture == L"Proto_Texture_CoreTree_StoneList_Sel")
	{
		if (m_pCalculatorCom->Picking_OnUI(g_hWnd, m_tRcUI))
		{
			m_tInfo.iTextureNum = 1;
		}
		else
			m_tInfo.iTextureNum = 0;

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

void CUI::SetSubTex1(wstring wstrProtoTag, _uint iTextureNum)
{
	m_bSubTex1 = true;
	m_iSubTexNum1 = iTextureNum;

	CComponent*		pComponent = nullptr;
	pComponent = m_pSubTextureCom1 = dynamic_cast<CTexture*>(Clone_Prototype(wstrProtoTag.c_str()));
	NULL_CHECK_RETURN(m_pTextureCom, );
	m_mapComponent[ID_STATIC].emplace(L"Com_SubTexture1", pComponent);
	m_iPassIdx++;
}

void CUI::SetSubTex2(wstring wstrProtoTag, _uint iTextureNum)
{
	m_bSubTex2 = true;
	m_iSubTexNum2 = iTextureNum;

	CComponent*		pComponent = nullptr;
	pComponent = m_pSubTextureCom2 = dynamic_cast<CTexture*>(Clone_Prototype(wstrProtoTag.c_str()));
	NULL_CHECK_RETURN(m_pTextureCom, );
	m_mapComponent[ID_STATIC].emplace(L"Com_SubTexture2", pComponent);
	m_iPassIdx++;
}

