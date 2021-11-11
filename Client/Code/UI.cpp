#include "stdafx.h"
#include "UI.h"
#include "Player.h"
#include "UIMgr.h"
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
	
	m_pTransformCom->Set_Scale(&_vec3{ 1.f,1.f,1.f });
	m_pTransformCom->Update_Component(0.f);
	m_vInitPos = m_tInfo.vPos;
	if (m_eType == UI::TYPE_FONT)
		return S_OK;
	if (m_tInfo.wstrTexture == L"Proto_Texture_CoreTree_StoneList_Sel")
	{
		fOffset = 300.f;
		m_tRcUI.left = (LONG)((m_tInfo.vPos.x+ fOffset) -( m_tInfo.vSize.x+ fOffset*2.f)*0.5f);
		m_tRcUI.top = (LONG)(m_tInfo.vPos.y - m_tInfo.vSize.y*0.5f);
		m_tRcUI.right = (LONG)((m_tInfo.vPos.x+ fOffset) + (m_tInfo.vSize.x+ fOffset*2.f)*0.5f);
		m_tRcUI.bottom = (LONG)(m_tInfo.vPos.y + m_tInfo.vSize.y*0.5f);
	}
	else 
	{
		m_tRcUI.left = (LONG)(m_tInfo.vPos.x - m_tInfo.vSize.x*0.5f);
		m_tRcUI.top = (LONG)(m_tInfo.vPos.y - m_tInfo.vSize.y*0.5f);
		m_tRcUI.right = (LONG)(m_tInfo.vPos.x + m_tInfo.vSize.x*0.5f);
		m_tRcUI.bottom = (LONG)(m_tInfo.vPos.y + m_tInfo.vSize.y*0.5f);
	}
	m_iPassIdx = 0;		//서브 텍스쳐가 늘어날 시 랜더 패스 변경 

	
	if (m_tInfo.wstrTexture == L"Proto_Texture_Element")
		m_pTarget = CGameMgr::GetInstance()->GetPlayer();
	
	m_fScaleSize = 1.f;
	return S_OK;
}

_int CUI::Update_Object(const _float& fTimeDelta)
{

	_float Timer_UI = Get_TimeDelta(L"Timer_UI");

	_int iExit = CGameObject::Update_Object(Timer_UI);
	//타임을 다른 타임으로 바꿔주자
	if (m_bActive)
	{
		D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
		UI_ElementUpdate(Timer_UI);
		UI_CoreTreeUpdate(Timer_UI);
		UI_StoneListUpdate(Timer_UI);
		UpdateColRect();
		Add_RenderGroup(RENDER_UI, this);
	}
	return iExit;
}

void CUI::Render_Object(void)
{
	if (m_eType == UI::TYPE_FONT)
	{

		Render_Font(m_tFont.wstrFont.c_str(), m_tFont.wstrText.c_str(), &_vec2(m_tFont.vPos.x+ m_tInfo.vPos.x, m_tFont.vPos.y + m_tInfo.vPos.y), D3DXCOLOR(m_tFont.vColor.x, m_tFont.vColor.y, m_tFont.vColor.z, m_tFont.vColor.w));
		// 여기서 폰트 위치를 따로 잡아서 넘기는데
		return;
	}

	LPD3DXEFFECT	 pEffect = m_pShaderCom->Get_EffectHandle();
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iMaxPass = 0;

	pEffect->Begin(&iMaxPass, NULL);


	pEffect->BeginPass(m_iPassIdx);

	m_pBufferCom->Render_Buffer();

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

CUI * CUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, UIFONT tFont, _bool bActive, UI::TYPE eType)
{
	CUI*	pInstance = new CUI(pGraphicDev);
	pInstance->SetFont(tFont);
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


	D3DXMatrixIdentity(&matWorld);

	matWorld._11 = m_tInfo.vSize.x;  
	matWorld._22 = m_tInfo.vSize.y;
	matWorld._33 = 1.f;
	matWorld._41 =  m_tInfo.vPos.x - WINCX * 0.5f;
	matWorld._42 = -m_tInfo.vPos.y + WINCY * 0.5f;
	

	matWorld = *m_pTransformCom->Get_WorldMatrix()*matWorld;

	pEffect->SetMatrix("g_matWorld", &matWorld);

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
		//선택되면 쉐이더로 강조효과 주고싶다 

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
					break;
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
					break;
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
					break;
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
					break;
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
					break;
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

			//이 UI 가 가지고 있는 돌에 접근해야됨 - > 장착된 돌UI 따로 만들어서 베이스 위에 올리자
			//돌 UI는 따로 업데이트 ㄱㄱ 

			if (Key_Down(KEY_LBUTTON))
			{
				CUIMgr::GetInstance()->SetCoreSelIdx(_wtoi(&m_tInfo.wstrObjTag.back()));
				CUIMgr::GetInstance()->SetActiveStoneListUI(true);
				CUIMgr::GetInstance()->SetActiveCoreTreeUI(false);

			}
			if (Key_Down(KEY_RBUTTON))
			{
				//돌 삭제 

			}

		}
	}

	if (m_tInfo.wstrTexture == L"Proto_Texture_CoreTree_Sel")
	{
		//1.3이 되면 감소
		//0.9가 되면 증가

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
	if (m_tInfo.wstrTexture == L"Proto_Texture_CoreTree_StoneList_Base")
	{
		//리스트 목록 배경일때 
		if (m_tInfo.iTextureNum == 2)
		{
			if (m_pCalculatorCom->Picking_OnUI(g_hWnd, m_tRcUI))
			{
				_uint iWheelState = CUIMgr::GetInstance()->GetWheelMove();
				if (WHEEL::MOVE_UP== (WHEEL::MOVE) iWheelState)
				{
					
					CUIMgr::GetInstance()->MoveStoneList(fTimeDelta, 600.f);
				}
				else if(WHEEL::MOVE_DOWN == (WHEEL::MOVE) iWheelState)
				{
					CUIMgr::GetInstance()->MoveStoneList(fTimeDelta, -600.f);
				}
			}
		}
	}


	//_float f
	if (m_tInfo.wstrTexture == L"Proto_Texture_CoreTree_StoneList_Sel")
	{
		if (m_pCalculatorCom->Picking_OnUI(g_hWnd, m_tRcUI))
		{
			m_tInfo.iTextureNum = 1;

			if (!m_bStart)
			{
				m_tInfo.vSize.x += 600.f;
				m_tInfo.vPos.x += 300.f;
				m_bStart = true;
			}
			//선택되었을떄
			//쉐이더로 왼쪽 위 아래 짤라 
			_uint iStoneIdx;
			list<CGameObject*> listSelStoneList = CUIMgr::GetInstance()->GetStoneSelIdxList(m_tInfo.wstrObjTag.c_str(), &iStoneIdx);

			for (auto iter : listSelStoneList)
			{
				CTransform *pTransIter = dynamic_cast<CTransform*>(iter->Get_Component(L"Com_Transform", ID_DYNAMIC));
				_vec3 vScale = pTransIter->Get_Scale();
				 if (vScale.x < 1.2f)
				 {
					 m_bScale = true;
				 }
				 else
				 {
					 m_bScale = false;
				 }

				 if (m_bScale)
				 {
					 vScale.x += fTimeDelta;
					 vScale.y += fTimeDelta;
				 }
				 pTransIter->Set_Scale(&vScale);
			}
			_uint iPreIdx = CUIMgr::GetInstance()->GetPreStoneIdx();
			if (CUIMgr::GetInstance()->GetStoneInfoUIActive(iPreIdx))
				CUIMgr::GetInstance()->SetActiveStoneInfoUI(false, iPreIdx);
				
			CUIMgr::GetInstance()->SetActiveStoneInfoUI(true, iStoneIdx);
			CUIMgr::GetInstance()->SetPreStoneIdx(iStoneIdx);
			if (Key_Down(KEY_LBUTTON))
			{
				CUIMgr::GetInstance()->SetActiveStoneListUI(false);
				CUIMgr::GetInstance()->SetActiveStoneInfoUI(false, iStoneIdx);
				CUIMgr::GetInstance()->SetActiveCoreTreeUI(true);
			}
		}
		else
		{
			if (m_bStart)
			{
				m_tInfo.vSize.x -= 600.f;
				m_tInfo.vPos.x -= 300.f;
				m_bStart = false;
			}

			_uint iStoneIdx;
			list<CGameObject*> listSelStoneList = CUIMgr::GetInstance()->GetStoneSelIdxList(m_tInfo.wstrObjTag.c_str(),&iStoneIdx);

			for (auto iter : listSelStoneList)
			{
				dynamic_cast<CTransform*>(iter->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Scale(1.f,1.f,1.f);		
			}
			
			m_tInfo.iTextureNum = 0;
		}

	}
}

void CUI::UpdateColRect()
{
	_vec3 vPos;
	m_pTransformCom->Get_INFO(INFO_POS, &vPos);

	if (m_tInfo.wstrTexture == L"Proto_Texture_CoreTree_StoneList_Sel")
	{
		fOffset = 300.f;
		m_tRcUI.left = (LONG)((m_tInfo.vPos.x + fOffset) + vPos.x - (m_tInfo.vSize.x + fOffset*2.f)*0.5f);
		m_tRcUI.top = (LONG)(m_tInfo.vPos.y + vPos.y - m_tInfo.vSize.y*0.5f);
		m_tRcUI.right = (LONG)((m_tInfo.vPos.x + fOffset) + vPos.x + (m_tInfo.vSize.x + fOffset*2.f)*0.5f);
		m_tRcUI.bottom = (LONG)(m_tInfo.vPos.y + vPos.y + m_tInfo.vSize.y*0.5f);
	}
	else {
		m_tRcUI.left = (LONG)(m_tInfo.vPos.x   + vPos.x - m_tInfo.vSize.x*0.5f);
		m_tRcUI.top = (LONG)(m_tInfo.vPos.y    + vPos.y - m_tInfo.vSize.y*0.5f);
		m_tRcUI.right = (LONG)(m_tInfo.vPos.x  + vPos.x + m_tInfo.vSize.x*0.5f);
		m_tRcUI.bottom = (LONG)(m_tInfo.vPos.y + vPos.y + m_tInfo.vSize.y*0.5f);
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

