#include "stdafx.h"
#include "Effect.h"
#include "GameMgr.h"
#include "Export_Function.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CEffect::CEffect(const CEffect& rhs)
	: CGameObject(rhs)
{

}

CEffect::~CEffect(void)
{
}

HRESULT CEffect::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_pTransformCom->Set_Pos(10.f, 0.f, 5.f);
	m_pTransformCom->Update_Component(0.f);

	return S_OK;
}

_int CEffect::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);
	if (m_bActive)
	{
		FrameChange(fTimeDelta);
		BillBord();

		UpdateEffect(fTimeDelta);

		Add_RenderGroup(RENDER_ALPHA, this);
	}
	return iExit;
}

void CEffect::Render_Object(void)
{

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

void CEffect::SetOption(void * pArg)
{
	CComponent*		pComponent = nullptr;
	if (pArg)
	{
		memcpy(&m_eEffect, pArg, sizeof(EFFECT::TYPE));
	}	


	Safe_Delete_Array(m_vUV);

	m_bActive = true;
	m_bLoop = false;
	switch (m_eEffect)
	{
	case EFFECT::EFFECT_LIGHTNING:
		m_vUV = new _vec2[4];
		m_wstrTexture = L"Proto_Texture_Effect_Lightning";
		m_eNextEffect;
		m_bLoop = true;
		SetVertiQuarterUV();
		SetBufferUV(m_vUV);
		m_pTransformCom->Set_Scale(1.f, 10.f, 1.f);
		//m_fFrameSpeed = 4.f;
		break;
	case EFFECT::EFFECT_BOLT:
		break;
	case EFFECT::EFFECT_ELECTRIC1:
		break;
	case EFFECT::EFFECT_ELECTRIC2:
		break;
	case EFFECT::EFFECT_FOG:
		break;
	case EFFECT::EFFECT_FOGGROUP:
		break;
	case EFFECT::EFFECT_AURA:
		break;
	case EFFECT::EFFECT_WATERBOOM:
		break;
	case EFFECT::EFFECT_CHARGE:
		break;

	case EFFECT::EFFECT_END:
		m_bActive = false;
		m_wstrTexture = L"";
		m_eNextEffect = EFFECT::EFFECT_END;
		m_fFrameSpeed = 0.f;
		m_fFrame = 0.f;
		return;
	default:
		break;
	}


	////초기값 저장 
	m_eInitNextEffect = m_eNextEffect;

	const _tchar* pConvComponentTag = W2BSTR((L"Com" + m_wstrTexture).c_str());

	// Collider
	auto& iter_find = find_if(m_mapComponent[ID_STATIC].begin(), m_mapComponent[ID_STATIC].end(), CTag_Finder(pConvComponentTag));

	if (iter_find == m_mapComponent[ID_STATIC].end())
	{
		//미리 등록해둔 텍스쳐 크론따서 컴포넌트로 저장
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Prototype(m_wstrTexture.c_str()));
		NULL_CHECK_RETURN(m_pTextureCom, );
		m_mapComponent[ID_STATIC].emplace(pConvComponentTag, pComponent);
	}
	else
	{
		m_pTextureCom = dynamic_cast<CTexture*>(iter_find->second);
	}



}

CEffect* CEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect*	pInstance = new CEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CEffect::Free(void)
{
	Safe_Delete_Array(m_vUV);
	CGameObject::Free();
}

void CEffect::FrameChange(_float fDeltaTime)
{
	m_fFrame += fDeltaTime;

	//0~1~2~3~4

		
	//0.25초 간격으로 uv 변경 실행 
	if (m_fFrame > 0.25f)
	{
		for (_uint i = 0; i < 4; i++)
		{
			m_vUV[i].x += m_vMoveUV.x;
		}
		if (m_iUVIdx == 4)
		{
			for (_uint i = 0; i < 4; i++)
			{
				m_vUV[i].y += m_vMoveUV.y;
			}
			m_iUVIdx = 0;
		}
		m_iUVIdx++;

		m_fFrame = 0.f;
		//루프가 아니면 빠져나감
	}
	m_pBufferCom->SetUV(m_vUV);
		//m_vUV[i].y += m_vMoveUV.x;
	

	////루프 
	//if (m_bLoop)
	//{
	//	if (m_fFrame > m_fFrameSpeed)
	//		m_fFrame = 0.f;
	//}
	//else
	//{
	//	if (m_fFrame > m_fFrameSpeed)
	//		m_bActive = false;
	//}

	//중간시작 프레임
	//if (m_fNextStartFrame != 1.f)
	//{
	//	if (m_fFrame > m_fFrameSpeed*m_fNextStartFrame)
	//	{
	//		m_bNextStart = true;
	//	}
	//}

}

void CEffect::BillBord()
{

	CTransform* pCamTrans = dynamic_cast<CTransform*>(CGameMgr::GetInstance()->GetCamera()->Get_Component(L"Com_Transform", ID_DYNAMIC));

	_matrix *pWorld = pCamTrans->Get_WorldMatrix();

	_vec3 vPos;
	m_pTransformCom->Get_INFO(INFO_POS, &vPos);

	pWorld->_41 = vPos.x;
	pWorld->_42 = vPos.y;
	pWorld->_43 = vPos.z;


	m_pTransformCom->Set_WorldMatrix(pWorld);




	//_matrix		matWorld, matView, matBill;

	//m_pTransformCom->Get_WorldMatrix(&matWorld);
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	//D3DXMatrixIdentity(&matBill);

	//matBill._11 = matView._11;
	//matBill._13 = matView._13;
	//matBill._31 = matView._31;
	//matBill._33 = matView._33;

	//D3DXMatrixInverse(&matBill, NULL, &matBill);

	//// 이 코드는 문제의 소지가 있음
	////빌(자,역) * (스 * 자 * 이)
	//m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));

	//_vec3 vPos;
	//m_pTransformCom->Get_INFO(INFO_POS, &vPos);

	//Compute_ViewZ(&vPos);
}

void CEffect::UpdateEffect(_float fTimeDelta)
{

}

void CEffect::SetVertiQuarterUV()
{
	m_vMoveUV.x = 0.25;
	m_vMoveUV.y = 1.f;
	m_vUV[0] = _vec2{ 0.f,0.f };
	m_vUV[1] = _vec2{ m_vMoveUV.x,0.f };
	m_vUV[2] = _vec2{ m_vMoveUV.x,m_vMoveUV.y };
	m_vUV[3] = _vec2{ 0.f,m_vMoveUV.y };
}

void CEffect::SetCenterQuarterUV()
{
	m_vMoveUV.x = 0.5f;
	m_vMoveUV.y = 0.5f;
	m_vUV[0] = _vec2{ 0.f,0.f };
	m_vUV[1] = _vec2{ m_vMoveUV.x,0.f };
	m_vUV[2] = _vec2{ m_vMoveUV.x,m_vMoveUV.y };
	m_vUV[3] = _vec2{ 0.f,m_vMoveUV.y };
}

void CEffect::SetHoriQuarterUV()
{
	m_vMoveUV.x = 1.f;
	m_vMoveUV.y = 0.25f;
	m_vUV[0] = _vec2{ 0.f,0.f };
	m_vUV[1] = _vec2{ m_vMoveUV.x,0.f };
	m_vUV[2] = _vec2{ m_vMoveUV.x,m_vMoveUV.y };
	m_vUV[3] = _vec2{ 0.f,m_vMoveUV.y };
}

void CEffect::Set66UV()
{
	m_vMoveUV.x = 0.166f;
	m_vMoveUV.y = 0.166f;
	m_vUV[0] = _vec2{ 0.f,0.f };
	m_vUV[1] = _vec2{ m_vMoveUV.x,0.f };
	m_vUV[2] = _vec2{ m_vMoveUV.x,m_vMoveUV.y };
	m_vUV[3] = _vec2{ 0.f,m_vMoveUV.y };

}

void CEffect::Set44UV()
{
	m_vMoveUV.x = 0.25f;
	m_vMoveUV.y = 0.25f;
	m_vUV[0] = _vec2{ 0.f,0.f };
	m_vUV[1] = _vec2{ m_vMoveUV.x,0.f };
	m_vUV[2] = _vec2{ m_vMoveUV.x,m_vMoveUV.y };
	m_vUV[3] = _vec2{ 0.f,m_vMoveUV.y };
}



HRESULT CEffect::Add_Component(void)
{
	CComponent*		pComponent = nullptr;
	
	// buffer
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Prototype(L"Proto_Buffer_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Prototype(L"Proto_Shader_Sample"));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);


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


void CEffect::SetBufferUV(_vec2* vUV)
{
	m_pBufferCom->SetUV(vUV);
}



HRESULT CEffect::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
{

	_matrix		matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", (_uint)m_fFrame);

	return S_OK;
}

