#include "stdafx.h"
#include "Effect.h"
#include "GameMgr.h"
#include "Export_Function.h"
#include "Player.h"
#include "SoundMgr.h"
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
	
	//m_pTransformCom->Set_Pos(10.f, 0.f, 5.f);
	m_pTransformCom->Update_Component(0.f);

	return S_OK;
}

_int CEffect::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);
	
	if (m_bBegin)
	{
		m_fBeginSpeed += fTimeDelta;
		if (m_fBeginSpeed > m_fBeginTime)
		{
			m_bBegin = false;
		}
	}
	if (m_bActive&&!m_bBegin)
	{
		FrameChange(fTimeDelta);
		if(m_bBillbord)
			BillBord();

		UpdateEffect(fTimeDelta);
		m_pTransformCom->Update_Component(fTimeDelta);
		if (!m_bCluster)
			Add_RenderGroup(RENDER_EFFECT, this);
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
		memcpy(&m_eEffect, pArg, sizeof(EFFECT::TYPE2D));
	}	


	Safe_Delete_Array(m_vUV);
	m_iUVIdx = 0;
	m_bActive = true;
	m_bLoop = false;
	m_fFrameSpeed = 2.f;
	if (!m_vUV)
		m_vUV = new _vec2[4];
	m_fLifeSpeed = 0.f;
	m_bCluster = false;

	switch (m_eEffect)
	{
	case EFFECT::EFFECT_LIGHTNING_CLUSTER:
		m_bCluster = true;
		return;
	case EFFECT::EFFECT_WATER_CLUSTER:
		m_bCluster = true;
		return;
	case EFFECT::EFFECT_LIGHTNING_WEAPON_CLUSTER:
		m_bCluster = true;
		return;
	case EFFECT::EFFECT_LIGHTNING1_1x4:
	{
		m_fLifeSpeed = 0.f;
		m_bLoop = true;
		m_bLife = true;
		m_bCluster = false;

		m_fLifeTime = GetRandomFloat(0.5f, 0.8f);

		m_wstrTexture = L"Proto_Texture_Effect1x4";
		m_wstrBuffer = L"Buffer_RcTex1x4";

		SetVertiQuarterUV();
		_float fScaleY = 30.f;
		m_vColor = { GetRandomFloat(0.3f,0.6f),GetRandomFloat(0.7f,1.f),GetRandomFloat(0.7f,1.f),GetRandomFloat(0.7f,1.f) };
		m_pTransformCom->Set_Scale(2.f, 30.f, 2.f);

		_vec3 vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_PosY(vPos.y + fScaleY * 2);

		m_iTextIdx = 0;
		m_eShader = EFFECT::SHADER_BLACKOUT;
	}
		break;
	case EFFECT::EFFECT_LIGHTNING2_1x4:
	{
		m_bLoop = true;
		m_bLife = true;
		m_bCluster = false;

		m_fLifeTime = GetRandomFloat(0.5f, 0.8f);

		m_wstrTexture = L"Proto_Texture_Effect1x4";
		m_wstrBuffer = L"Buffer_RcTex1x4";
		_float fScaleY = 30.f;
		SetVertiQuarterUV();

		m_vColor = { GetRandomFloat(0.3f,0.6f),GetRandomFloat(0.7f,1.f),GetRandomFloat(0.7f,1.f),GetRandomFloat(0.7f,1.f) };
		m_pTransformCom->Set_Scale(1.f, fScaleY, 1.f);

		_vec3 vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_PosY(vPos.y + fScaleY * 2);

		m_iTextIdx = 1;
		m_eShader = EFFECT::SHADER_BLACKOUT;
	}
		break;
	case EFFECT::EFFECT_LIGHTNING3_1x4:
	{
		m_bLoop = true;
		m_bLife = true;
		m_bCluster = false;

		m_fLifeTime = GetRandomFloat(0.5f, 0.8f);

		m_wstrTexture = L"Proto_Texture_Effect1x4";
		m_wstrBuffer = L"Buffer_RcTex1x4";

		SetVertiQuarterUV();
		_float fScaleY = 30.f;

		_vec3 vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_PosY(vPos.y + fScaleY);

		m_vColor = { GetRandomFloat(0.3f,0.6f),GetRandomFloat(0.7f,1.f),GetRandomFloat(0.7f,1.f),GetRandomFloat(0.7f,1.f) };
		m_pTransformCom->Set_Scale(2.f, fScaleY * 2, 2.f);
		m_iTextIdx = 2;
		m_eShader = EFFECT::SHADER_BLACKOUT;
	}
		break;
	case EFFECT::EFFECT_LIGHTNING4_1x4:
	{
		m_bLoop = true;
		m_bLife = true;
		m_bCluster = false;

		m_fLifeTime = GetRandomFloat(0.5f, 0.8f);

		m_wstrTexture = L"Proto_Texture_Effect1x4";
		m_wstrBuffer = L"Buffer_RcTex1x4";

		SetVertiQuarterUV();
		_float fScaleY = 30.f;
		m_vColor = { GetRandomFloat(0.3f,0.6f),GetRandomFloat(0.7f,1.f),GetRandomFloat(0.7f,1.f),1.f };
		m_pTransformCom->Set_Scale(1.f, fScaleY, 1.f);
		_vec3 vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_PosY(vPos.y + fScaleY*2);

		m_eShader = EFFECT::SHADER_BLACKOUT;
		m_iTextIdx = 3;
		break;

	}
	case EFFECT::EFFECT_LIGHTNING1_PLAYER_1x4:
	{
		m_bLoop = true;
		m_bLife = true;
		m_bCluster = false;

		m_fLifeTime = GetRandomFloat(0.8f, 1.2f);

		m_wstrTexture = L"Proto_Texture_Effect1x4";
		m_wstrBuffer = L"Buffer_RcTex1x4";
		m_bBillbord = false;
		SetVertiQuarterUV();
		_float fScaleY = 15.f;
		m_vColor = { 1.f,1.f,GetRandomFloat(0.5f,1.f),1.f };
		m_pTransformCom->Set_Scale(1.f, 10.f, 1.f);
		m_fMaxSize = 15.f;

		CTransform* pTrans = dynamic_cast<CTransform*>(CGameMgr::GetInstance()->GetPlayer()->Get_Component(L"Com_Transform", ID_DYNAMIC));
		_vec3 vPlayerRot = pTrans->Get_Rot();
		m_pTransformCom->Set_Rot(D3DXToRadian(-90.f), vPlayerRot.y, 0.f);

		_vec3 vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_PosY(vPos.y + 5.f);

		

		m_iTextIdx = 0;
		m_eShader = EFFECT::SHADER_BLACKOUT_LIGHTNING;
		break;
	}
	case EFFECT::EFFECT_LIGHTNING2_PLAYER_1x4:
	{
		m_bLoop = true;
		m_bLife = true;
		m_bCluster = false;

		m_fLifeTime = GetRandomFloat(0.8f, 1.2f);

		m_wstrTexture = L"Proto_Texture_Effect1x4";
		m_wstrBuffer = L"Buffer_RcTex1x4";

		SetVertiQuarterUV();
		m_vColor = { GetRandomFloat(0.5f,0.8f),GetRandomFloat(0.5f,0.8f),GetRandomFloat(0.0f,0.2f),GetRandomFloat(0.6f,0.9f) };
		m_pTransformCom->Set_Scale(1.f, 10.f, 1.f);
		m_fMaxSize = 15.f;
		m_bBillbord = false;
		CTransform* pTrans = dynamic_cast<CTransform*>(CGameMgr::GetInstance()->GetPlayer()->Get_Component(L"Com_Transform", ID_DYNAMIC));
		_vec3 vPlayerRot = pTrans->Get_Rot();
		m_pTransformCom->Set_Rot(D3DXToRadian(-90.f), vPlayerRot.y, 0.f);

		_vec3 vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_PosY(vPos.y + 5.f);

		m_iTextIdx = 1;
		m_eShader = EFFECT::SHADER_BLACKOUT_LIGHTNING;
		break;
	}
	case EFFECT::EFFECT_LIGHTNING3_PLAYER_1x4:
	{
		m_bLoop = true;
		m_bLife = true;
		m_bCluster = false;

		m_fLifeTime = GetRandomFloat(0.8f, 1.2f);

		m_wstrTexture = L"Proto_Texture_Effect1x4";
		m_wstrBuffer = L"Buffer_RcTex1x4";

		SetVertiQuarterUV();
		_float fScaleY = 30.f;
		m_vColor = { GetRandomFloat(1.f,1.f),GetRandomFloat(1.f,1.f),GetRandomFloat(0.8f,1.f),1.f };
		m_pTransformCom->Set_Scale(1.f, 10.f, 1.f);
		m_fMaxSize = 15.f;


		CTransform* pTrans = dynamic_cast<CTransform*>(CGameMgr::GetInstance()->GetPlayer()->Get_Component(L"Com_Transform", ID_DYNAMIC));
		_vec3 vPlayerRot = pTrans->Get_Rot();
		m_pTransformCom->Set_Rot(D3DXToRadian(90.f), vPlayerRot.y, 0.f);
		m_bBillbord = false;
		_vec3 vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_PosY(vPos.y + 5.f);

		m_iTextIdx = 2;
		m_eShader = EFFECT::SHADER_BLACKOUT_LIGHTNING;
		break;
	}
	case EFFECT::EFFECT_LIGHTNING4_PLAYER_1x4:
	{
		m_bLoop = true;
		m_bLife = true;
		m_bCluster = false;

		m_fLifeTime = GetRandomFloat(0.8f, 1.2f);

		m_wstrTexture = L"Proto_Texture_Effect1x4";
		m_wstrBuffer = L"Buffer_RcTex1x4";


		CTransform* pTrans = dynamic_cast<CTransform*>(CGameMgr::GetInstance()->GetPlayer()->Get_Component(L"Com_Transform", ID_DYNAMIC));
		_vec3 vPlayerRot = pTrans->Get_Rot();
		m_pTransformCom->Set_Rot(D3DXToRadian(90.f), vPlayerRot.y, 0.f);

		m_bBillbord = false;

		SetVertiQuarterUV();
		_float fScaleY = 30.f;
		m_vColor = { GetRandomFloat(0.5f,0.8f),GetRandomFloat(0.5f,0.8f),GetRandomFloat(0.0f,0.2f),1.f };
		m_pTransformCom->Set_Scale(2.f, 10.f, 2.f);
		m_fMaxSize = 15.f;
		_vec3 vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_PosY(vPos.y +5.f);

		m_iTextIdx = 3;
		m_eShader = EFFECT::SHADER_BLACKOUT_LIGHTNING;
		break;
	}
	case EFFECT::EFFECT_WATERBOOM1_2x2:
	{
		m_bLoop = true;
		m_bLife = true;
		m_bCluster = false;

		m_fLifeTime = GetRandomFloat(0.2f, 0.4f);
		m_fFrameSpeed = 1.f;
		m_wstrTexture = L"Proto_Texture_Effect2x2";
		m_wstrBuffer = L"Buffer_RcTex2x2";
		SetCenterQuarterUV();

		_float fSize = GetRandomFloat(4.f, 7.f);
		m_pTransformCom->Set_Scale(fSize, fSize, fSize);
		m_vColor = { 1.f,1.f,1.f,0.4f };


		m_eShader = EFFECT::SHADER_BLACKOUT;
		m_iTextIdx = 0;
		break;
	}
	case EFFECT::EFFECT_WATERBOOM2_2x2:
	{
		m_bLoop = true;
		m_bLife = true;
		m_bCluster = false;

		m_fLifeTime = GetRandomFloat(0.2f, 0.4f);
		m_fFrameSpeed = 1.f;
		m_wstrTexture = L"Proto_Texture_Effect2x2";
		m_wstrBuffer = L"Buffer_RcTex2x2";
		SetCenterQuarterUV();
		_float fSize = GetRandomFloat(4.f, 7.f);
		m_pTransformCom->Set_Scale(fSize, fSize, fSize);
		m_vColor = { 1.f,1.f,1.f,0.4f };

		m_eShader = EFFECT::SHADER_BLACKOUT;
		m_iTextIdx = 1;
		break;
	}
	case EFFECT::EFFECT_WATERBOOM3_2x2:
	{
		m_bLoop = true;
		m_bLife = true;
		m_bCluster = false;

		m_fLifeTime = GetRandomFloat(0.2f, 0.4f);
		m_fFrameSpeed = 1.f;
		m_wstrTexture = L"Proto_Texture_Effect2x2";
		m_wstrBuffer = L"Buffer_RcTex2x2";
		SetCenterQuarterUV();
		_float fSize = GetRandomFloat(4.f, 7.f);
		m_pTransformCom->Set_Scale(fSize, fSize, fSize);
		m_vColor = { 1.f,1.f,1.f,0.4f };

		m_eShader = EFFECT::SHADER_BLACKOUT;
		m_iTextIdx = 2;
		break;
	}
	case EFFECT::EFFECT_WATERBOOM4_2x2:
	{
		m_bLoop = true;
		m_bLife = true;
		m_bCluster = false;
		m_fLifeTime = GetRandomFloat(0.2f, 0.4f);
		m_fFrameSpeed = 1.f;


		m_wstrTexture = L"Proto_Texture_Effect2x2";
		m_wstrBuffer = L"Buffer_RcTex2x2";
		SetCenterQuarterUV();

		_float fSize = GetRandomFloat(4.f, 7.f);
		m_pTransformCom->Set_Scale(fSize, fSize, fSize);
		
		m_vColor = { 1.f,1.f,1.f,0.4f };

		m_iTextIdx = 3;
		m_eShader = EFFECT::SHADER_BLACKOUT;
		break;
	}
	case EFFECT::EFFECT_WATERBOOM5_2x2:
	{
		m_bLoop = true;
		m_bLife = true;
		m_bCluster = false;

		m_fLifeTime = GetRandomFloat(0.2f, 0.4f);
		m_fFrameSpeed = 1.f;

		m_wstrTexture = L"Proto_Texture_Effect2x2";
		m_wstrBuffer = L"Buffer_RcTex2x2";
		SetCenterQuarterUV();

		_float fSize = GetRandomFloat(4.f, 7.f);
		m_pTransformCom->Set_Scale(fSize, fSize, fSize);
		m_vColor = { 1.f,1.f,1.f,0.4f };

		m_eShader = EFFECT::SHADER_BLACKOUT;
		m_iTextIdx = 4;
		break;
	}
	case EFFECT::EFFECT_WATERBOOM6_2x2:
	{
		m_bLoop = true;
		m_bLife = true;
		m_bCluster = false;

		m_fLifeTime = GetRandomFloat(0.2f, 0.4f);
		m_fFrameSpeed = 1.f;

		m_wstrTexture = L"Proto_Texture_Effect2x2";
		m_wstrBuffer = L"Buffer_RcTex2x2";
		SetCenterQuarterUV();

		_float fSize = GetRandomFloat(4.f, 7.f);
		m_pTransformCom->Set_Scale(fSize, fSize, fSize);
		m_vColor = { 1.f,1.f,1.f,0.4f };

		m_eShader = EFFECT::SHADER_BLACKOUT;
		m_iTextIdx = 5;
		break;
	}
	case EFFECT::EFFECT_FOG1_2x2:
	{
		m_bLoop = false;
		m_bLife = false;
		m_bCluster = false;
		m_bStay = true;
		//m_fLifeTime = GetRandomFloat(0.2f, 0.4f);
		m_fFrameSpeed = 2.f;
		m_wstrTexture = L"Proto_Texture_Effect2x2";
		m_wstrBuffer = L"Buffer_RcTex2x2";
		SetCenterQuarterUV();

		_float fSize = GetRandomFloat(3.f, 7.f);
		m_pTransformCom->Set_Scale(fSize, fSize, fSize);
		m_vColor = { 0.5f,0.5f,0.5f,0.1f };
		m_bBillbord = true;
		m_fMoveSpeed = 20.f;
		m_eShader = EFFECT::SHADER_BLACKOUT;
		m_iTextIdx = 6;
		break;
	}
	case EFFECT::EFFECT_FOG2_2x2:
	{
		m_bLoop = false;
		m_bLife = false;
		m_bCluster = false;

		//m_fLifeTime = GetRandomFloat(0.2f, 0.4f);
		m_fFrameSpeed =2.f;
		m_wstrTexture = L"Proto_Texture_Effect2x2";
		m_wstrBuffer = L"Buffer_RcTex2x2";
		SetCenterQuarterUV();

		_float fSize = GetRandomFloat(4.f, 7.f);
		m_pTransformCom->Set_Scale(fSize, fSize, fSize);
		m_vColor = { 1.f,1.f,1.f,0.2f };


		m_eShader = EFFECT::SHADER_BLACKOUT;
		m_iTextIdx = 7;
		break;
	}
	case EFFECT::EFFECT_FOG3_2x2:
		m_wstrTexture = L"Proto_Texture_Effect2x2";
		m_wstrBuffer = L"Buffer_RcTex2x2";
		SetCenterQuarterUV();
		m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
		m_iTextIdx = 8;
		break;
	case EFFECT::EFFECT_ACT1_2x2:
		m_wstrTexture = L"Proto_Texture_Effect2x2";
		m_wstrBuffer = L"Buffer_RcTex2x2";
		SetCenterQuarterUV();
		m_pTransformCom->Set_Scale(5.f, 5.f, 5.f);
		m_vColor = { 0.1f,9.f,0.1f,0.8f };
		m_iTextIdx = 9;
		m_fFrameSpeed = 2.f;
		m_eShader = EFFECT::SHADER_TEST;

		//m_bLoop = true;
		break;
	case EFFECT::EFFECT_ACT2_2x2:
		m_wstrTexture = L"Proto_Texture_Effect2x2";
		m_wstrBuffer = L"Buffer_RcTex2x2";
		SetCenterQuarterUV();
		m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
		m_iTextIdx = 10;
		break;

	case EFFECT::EFFECT_ACT3_2x2:
		m_wstrTexture = L"Proto_Texture_Effect2x2";
		m_wstrBuffer = L"Buffer_RcTex2x2";
		SetCenterQuarterUV();
		m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
		m_iTextIdx = 11;
		break;

	case EFFECT::EFFECT_STONE_4x4:
		m_wstrTexture = L"Proto_Texture_Effect4x4";
		m_wstrBuffer = L"Buffer_RcTex4x4";
		Set44UV();
		m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
		m_iTextIdx = 2;
		break;
	case EFFECT::EFFECT_FLAME1_6x6:
		m_wstrTexture = L"Proto_Texture_Effect6x6";
		m_wstrBuffer = L"Buffer_RcTex6x6";
		Set66UV();
		m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
		m_iTextIdx = 0;
		break;
	case EFFECT::EFFECT_FLAME2_6x6:
		m_wstrTexture = L"Proto_Texture_Effect6x6";
		m_wstrBuffer = L"Buffer_RcTex6x6";
		Set66UV();
		m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
		m_iTextIdx = 1;
		break;
	case EFFECT::EFFECT_FLAME3_6x6:
		m_wstrTexture = L"Proto_Texture_Effect6x6";
		m_wstrBuffer = L"Buffer_RcTex6x6";

		Set66UV();
		m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
		m_iTextIdx = 2;
		break;
	case EFFECT::EFFECT_MAGIC_CIRCLE_SINGLE:
		m_wstrTexture = L"Proto_Texture_EffectSingle";
		m_wstrBuffer = L"Proto_Buffer_RcTex";
		m_pTransformCom->Set_Scale(6.f, 6.f, 6.f);
		m_iTextIdx = 0;
		m_eShader = EFFECT::SHADER_BLACKOUT;
		m_bBillbord = false;
		SetProtoUV();
		m_vColor = { 0.5f,0.5f,0.5f,0.7f };
		m_bAction = true;
		m_fActionSpeed = 0.f;
		m_fActionTime = 2.f;
		m_fFrameSpeed = 10.f;
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_MONSTER_ATK_1);
		CSoundMgr::Get_Instance()->PlaySound(L"en_waterboss_calllightning_01.ogg", CSoundMgr::CHANNEL_MONSTER_ATK_1);

		//m_bLife = true;
		//m_fLifeSpeed = 0.f;
		//m_fLifeSpeed = 2.f;
		break;
	case EFFECT::EFFECT_EXPLOSION:
		m_wstrTexture = L"Proto_Texture_EffectExplosion";
		m_wstrBuffer = L"Proto_Buffer_RcTex";
		m_pTransformCom->Set_Scale(6.f, 6.f, 6.f);
		m_iTextIdx = 0;
		m_bBillbord = true;
		SetProtoUV();
		m_vColor = { 0.8f,0.8f,0.8f,0.85f };
		m_bAction = true;
		m_fActionSpeed = 0.f;
		m_fActionTime = 1.f;
		m_fFrameSpeed = 15.f;
		m_bBegin = true;
		m_fBeginSpeed = 0.f;
		m_fBeginTime = 0.5f;

		m_eShader = EFFECT::SHADER_BLACKOUT;
		break;
	case EFFECT::EFFECT_END:
		m_bActive = false;
		m_wstrTexture = L"";
		m_wstrBuffer = L"";
		m_eShader = EFFECT::SHADER_END;

		m_eNextEffect = EFFECT::EFFECT_END;
		m_eNextEffect2 = EFFECT::EFFECT_END;
		m_fFrameSpeed = 0.f;
		m_fFrame = 0.f;
		m_bCluster = true;
		m_fLifeSpeed = 0.f;
		m_fLifeTime = 0.f;
		m_bLife = false;
		m_bBillbord = true;
		//m_bLife = false;
		m_iTextIdx = 0;
		m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
		m_pTransformCom->Set_Rot(0.f, 0.f, 0.f);
		m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
		m_vColor = { 1.f, 1.f, 1.f, 1.f };
		m_vMaxUv.x = 0.f;
		m_vMaxUv.y = 0.f;

		m_vDir = { 0.f,0.f,0.f };
		m_bAction = false;
		m_fActionSpeed = 0.f;
		m_fActionTime= 0.f;
		m_fBeginSpeed = 0.f;
		m_fBeginTime= 0.f;
		m_bBegin = false;
		m_bMove = false;
		m_bStay = false;
		m_fMoveSpeed = 0.f;
		return;
	default:
		break;
	}
	
	switch (m_eShader)
	{
	case EFFECT::SHADER_BLEND:
		m_iPassIdx = 0;
		break;
	case EFFECT::SHADER_TEST:
		m_iPassIdx = 1;
		break;
	case EFFECT::SHADER_BLACKOUT:
		m_iPassIdx = 2;
		break;
	case EFFECT::SHADER_BLACKOUT_LIGHTNING:
		m_iPassIdx = 3;
		break;
	case EFFECT::SHADER_END:
		break;
	default:
		break;
	}

	////초기값 저장 
	
	if (m_wstrTexture == L"")
		return;

	USES_CONVERSION;
	const _tchar* pConvComponentTag = W2BSTR((m_wstrTexture).c_str());

	auto& iter_find = find_if(m_mapComponent[ID_STATIC].begin(), m_mapComponent[ID_STATIC].end(), CTag_Finder(pConvComponentTag));

	if (iter_find == m_mapComponent[ID_STATIC].end())
	{
		//미리 등록해둔 텍스쳐 크론따서 컴포넌트로 저장
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Prototype(pConvComponentTag));
		NULL_CHECK_RETURN(m_pTextureCom, );
		m_mapComponent[ID_STATIC].emplace(pConvComponentTag, pComponent);
	}
	else
	{
		m_pTextureCom = dynamic_cast<CTexture*>(iter_find->second);
	}


	// buffer
	pConvComponentTag = W2BSTR((m_wstrBuffer).c_str());

	iter_find = find_if(m_mapComponent[ID_STATIC].begin(), m_mapComponent[ID_STATIC].end(), CTag_Finder(pConvComponentTag));

	if (iter_find == m_mapComponent[ID_STATIC].end())
	{
		//미리 등록해둔 텍스쳐 크론따서 컴포넌트로 저장
		pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Prototype(pConvComponentTag));
		NULL_CHECK_RETURN(m_pBufferCom, );
		m_mapComponent[ID_STATIC].emplace(pConvComponentTag, pComponent);
	}
	else
	{
		m_pBufferCom = dynamic_cast<CRcTex*>(iter_find->second);
	}
	SetBufferUV(m_vUV);


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
	m_fFrame += fDeltaTime*m_fFrameSpeed;

	//0~1~2~3~4

	if (m_vMaxUv.x != 0)
	{
		if (m_fFrame > 0.1f)
		{
			for (_uint i = 0; i < 4; i++)
			{
				m_vUV[i].x += m_vMoveUV.x;
			}
			if (m_iUVIdx % (_uint)m_vMaxUv.x == 0)
			{
				for (_uint i = 0; i < 4; i++)
				{
					m_vUV[i].y += m_vMoveUV.y;
				}

			}
			m_iUVIdx++;
			m_fFrame = 0.f;
			if (m_iUVIdx >= m_vMaxUv.x* m_vMaxUv.y)
			{
				if (m_bLoop)
				{
					m_iUVIdx = 0;
				}
				else
				{
					if(!m_bStay)
						CGameMgr::GetInstance()->RetunEffect(this);
					else
					{
						m_vMaxUv.x = 0.f;
					}
				}
			}
		}

	}
	if(!m_bCluster)
		m_pBufferCom->SetUV(m_vUV);

}

void CEffect::BillBord()
{

	CTransform* pCamTrans = dynamic_cast<CTransform*>(CGameMgr::GetInstance()->GetCamera()->Get_Component(L"Com_Transform", ID_DYNAMIC));

	_matrix *pWorld = pCamTrans->Get_WorldMatrix();
	D3DXMatrixInverse(pWorld, NULL,pWorld);
	_vec3 vPos;
	m_pTransformCom->Get_INFO(INFO_POS, &vPos);

	pWorld->_41 = vPos.x;
	pWorld->_42 = vPos.y;
	pWorld->_43 = vPos.z;


	m_pTransformCom->Set_WorldMatrix(pWorld);

}

void CEffect::UpdateEffect(_float fTimeDelta)
{

	if (m_bLife)
	{		
		m_fLifeSpeed += fTimeDelta;
		if (m_fLifeSpeed > m_fLifeTime)
		{
			EffectEnd();
		}
	}
	if (m_bAction)
	{
		m_fActionSpeed += fTimeDelta;
		if (m_fActionSpeed > m_fActionTime)
		{
			m_bAction = false;
		}
	}

	switch (m_eEffect)
	{
	case EFFECT::EFFECT_LIGHTNING_CLUSTER:
	{
		CGameObject* pObj;
		_vec3 vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		pObj = CGameMgr::GetInstance()->GetEffect(EFFECT::EFFECT_LIGHTNING1_1x4);
		pObj->SetPos(vPos, ID_DYNAMIC);
		pObj = CGameMgr::GetInstance()->GetEffect(EFFECT::EFFECT_LIGHTNING2_1x4);
		pObj->SetPos(vPos, ID_DYNAMIC);
		pObj = CGameMgr::GetInstance()->GetEffect(EFFECT::EFFECT_LIGHTNING3_1x4);
		pObj->SetPos(vPos, ID_DYNAMIC);
		pObj = CGameMgr::GetInstance()->GetEffect(EFFECT::EFFECT_LIGHTNING4_1x4);
		pObj->SetPos(vPos, ID_DYNAMIC);
		pObj =CGameMgr::GetInstance()->GetParticle(PARTICLEEFF::PARTICLE_LIGHTNING);
		pObj->SetPos(vPos, ID_STATIC);
		pObj = CGameMgr::GetInstance()->GetEnemyBullet(BULLET::BULLET_CALLLIGHTNING);
		pObj->SetPos(vPos, ID_DYNAMIC);

		EffectEnd();
		break;
	}
	case EFFECT::EFFECT_WATER_CLUSTER:
	{
		CGameObject* pObj;
		_vec3 vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		pObj = CGameMgr::GetInstance()->GetEffect(EFFECT::EFFECT_WATERBOOM1_2x2);
		pObj->SetPos(vPos,ID_DYNAMIC);
		//pObj = CGameMgr::GetInstance()->GetEffect(EFFECT::EFFECT_WATERBOOM2_2x2);
		//pObj->SetPos(vPos, ID_DYNAMIC);
		pObj = CGameMgr::GetInstance()->GetEffect(EFFECT::EFFECT_WATERBOOM3_2x2);
		pObj->SetPos(vPos, ID_DYNAMIC);
		pObj = CGameMgr::GetInstance()->GetEffect(EFFECT::EFFECT_WATERBOOM5_2x2);
		pObj->SetPos(vPos, ID_DYNAMIC);
		EffectEnd();
		break;
	}
	case EFFECT::EFFECT_LIGHTNING_WEAPON_CLUSTER:
	{
		CGameObject* pObj;
		_vec3 vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		pObj = CGameMgr::GetInstance()->GetEffect(EFFECT::EFFECT_LIGHTNING1_PLAYER_1x4);
		pObj->SetPos(vPos, ID_DYNAMIC);
		pObj = CGameMgr::GetInstance()->GetEffect(EFFECT::EFFECT_LIGHTNING2_PLAYER_1x4);
		pObj->SetPos(vPos, ID_DYNAMIC);
		pObj = CGameMgr::GetInstance()->GetEffect(EFFECT::EFFECT_LIGHTNING3_PLAYER_1x4);
		pObj->SetPos(vPos, ID_DYNAMIC);
		pObj = CGameMgr::GetInstance()->GetEffect(EFFECT::EFFECT_LIGHTNING4_PLAYER_1x4);
		pObj->SetPos(vPos, ID_DYNAMIC);
		EffectEnd();
		break;
	}
	case EFFECT::EFFECT_LIGHTNING1_1x4:
		m_pTransformCom->Set_Rot(0.f, D3DXToRadian(0.f), 0.f);
		break;
	case EFFECT::EFFECT_LIGHTNING2_1x4:
		m_pTransformCom->Set_Rot(0.f, D3DXToRadian(45.f), 0.f);
		break;
	case EFFECT::EFFECT_LIGHTNING3_1x4:
		m_pTransformCom->Set_Rot(0.f, D3DXToRadian(90.f), 0.f);

		break;
	case EFFECT::EFFECT_LIGHTNING4_1x4:
		m_pTransformCom->Set_Rot(0.f, D3DXToRadian(135.f), 0.f);
		break;

	case EFFECT::EFFECT_LIGHTNING1_PLAYER_1x4:
	{

		break;
	}
	case EFFECT::EFFECT_LIGHTNING2_PLAYER_1x4:
	{
		break;
	}
	case EFFECT::EFFECT_LIGHTNING3_PLAYER_1x4:
	{
		break;
	}
	case EFFECT::EFFECT_LIGHTNING4_PLAYER_1x4:
	{
		break;
	}

	case EFFECT::EFFECT_WATERBOOM1_2x2:
		m_pTransformCom->Set_Rot(0.f, D3DXToRadian(30.f), 0.f);
		break;
	case EFFECT::EFFECT_WATERBOOM2_2x2:
		m_pTransformCom->Set_Rot(0.f, D3DXToRadian(60.f), 0.f);
		break;
	case EFFECT::EFFECT_WATERBOOM3_2x2:
		m_pTransformCom->Set_Rot(0.f, D3DXToRadian(90.f), 0.f);
		break;
	case EFFECT::EFFECT_WATERBOOM4_2x2:
		m_pTransformCom->Set_Rot(0.f, D3DXToRadian(120.f), 0.f);
		break;
	case EFFECT::EFFECT_WATERBOOM5_2x2:
		m_pTransformCom->Set_Rot(0.f, D3DXToRadian(150.f), 0.f);
		break;
	case EFFECT::EFFECT_WATERBOOM6_2x2:
		m_pTransformCom->Set_Rot(0.f, D3DXToRadian(180.f), 0.f);
		break;
	case EFFECT::EFFECT_FOG1_2x2:
	{
		if (m_bMove)
		{
			D3DXVec3Normalize(&m_vDir, &m_vDir);
			if (m_pTransformCom->MoveStep(MOVETYPE_BREAK, &m_fMoveSpeed, 100.f, 0.f, &m_vDir, fTimeDelta))
				EffectEnd();
		}
		_vec3 vScale = m_pTransformCom->Get_Scale();

		vScale.x -= fTimeDelta*m_fFrameSpeed;
		vScale.y -= fTimeDelta*m_fFrameSpeed;
		vScale.z -= fTimeDelta*m_fFrameSpeed;
		m_pTransformCom->Set_Scale(&vScale);
	}
		break;
	case EFFECT::EFFECT_FOG2_2x2:
		break;
	case EFFECT::EFFECT_FOG3_2x2:
		break;
	case EFFECT::EFFECT_ACT1_2x2:
	{
		m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(360.f));
		_vec3 vScale = m_pTransformCom->Get_Scale();

		vScale.x -= fTimeDelta*m_fFrameSpeed;
		vScale.y -= fTimeDelta*m_fFrameSpeed;
		vScale.z -= fTimeDelta*m_fFrameSpeed;
		m_pTransformCom->Set_Scale(&vScale);
		if (vScale.x < 0.f)
			EffectEnd();
	}
		break;
	case EFFECT::EFFECT_ACT2_2x2:
		break;
	case EFFECT::EFFECT_ACT3_2x2:
		break;
	case EFFECT::EFFECT_STONE_4x4:
		break;
	case EFFECT::EFFECT_FLAME1_6x6:
		break;
	case EFFECT::EFFECT_FLAME2_6x6:
		break;
	case EFFECT::EFFECT_FLAME3_6x6:
		break;
	case EFFECT::EFFECT_MAGIC_CIRCLE_SINGLE:
	{
		//m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(360.f));
		//m_pTransformCom->Update_Component(fTimeDelta);
		m_pTransformCom->Set_Rot(D3DXToRadian(90.f), 0.f,0.f);
		m_pTransformCom->Update_Component(fTimeDelta);
		if (!m_bAction)
		{
			_vec3 vScale = m_pTransformCom->Get_Scale();
			vScale.x -= fTimeDelta*m_fFrameSpeed;
			vScale.y -= fTimeDelta*m_fFrameSpeed;
			//vScale.z -= fTimeDelta*m_fFrameSpeed;
			m_pTransformCom->Set_Scale(&vScale);
			if(vScale.x <0.1)
			CSoundMgr::Get_Instance()->PlaySound(L"en_waterboss_calllightning_01.ogg", CSoundMgr::CHANNEL_EFFECT1);
			CSoundMgr::Get_Instance()->PlaySound(L"en_waterboss_calllightning_02.ogg", CSoundMgr::CHANNEL_EFFECT2);
			if (vScale.x < 0.f)
				EffectEnd();
		}
		m_vColor = { 1.f,0.1f,0.1f,0.6f };
	}
		break;
	case EFFECT::EFFECT_EXPLOSION:
	{

		m_iTextIdx = (_uint)m_fFrame;
		if(m_iTextIdx>3)
			EffectEnd();

		
		break;
	}
	case EFFECT::EFFECT_END:
		break;
	default:
		break;
	}
}

void CEffect::EffectEnd()
{
	if (m_eNextEffect != EFFECT::EFFECT_END)
	{
		CGameObject* pObj;
		pObj = CGameMgr::GetInstance()->GetEffect(m_eNextEffect);
		_vec3 vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		pObj->SetPos(vPos,ID_DYNAMIC);
	}
	if (m_eNextEffect2 != EFFECT::EFFECT_END)
	{
		CGameObject* pObj;
		pObj = CGameMgr::GetInstance()->GetEffect(m_eNextEffect2);
		_vec3 vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		pObj->SetPos(vPos, ID_DYNAMIC);
	}
	CGameMgr::GetInstance()->RetunEffect(this);
}

void CEffect::SetVertiQuarterUV()
{
	m_vMoveUV.x = 0.25;
	m_vMoveUV.y = 1.f;
	m_vUV[0] = _vec2{ 0.f,0.f };
	m_vUV[1] = _vec2{ m_vMoveUV.x,0.f };
	m_vUV[2] = _vec2{ m_vMoveUV.x,m_vMoveUV.y };
	m_vUV[3] = _vec2{ 0.f,m_vMoveUV.y };
	m_vMaxUv.x = 4.f;
	m_vMaxUv.y = 1.f;


}

void CEffect::SetCenterQuarterUV()
{
	m_vMoveUV.x = 0.5f;
	m_vMoveUV.y = 0.5f;
	m_vUV[0] = _vec2{ 0.f,0.f };
	m_vUV[1] = _vec2{ m_vMoveUV.x,0.f };
	m_vUV[2] = _vec2{ m_vMoveUV.x,m_vMoveUV.y };
	m_vUV[3] = _vec2{ 0.f,m_vMoveUV.y };
	m_vMaxUv.x = 2.f;
	m_vMaxUv.y = 2.f;
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
	m_vMaxUv.x = 6.f;
	m_vMaxUv.y = 6.f;
}

void CEffect::Set44UV()
{
	m_vMoveUV.x = 0.25f;
	m_vMoveUV.y = 0.25f;
	m_vUV[0] = _vec2{ 0.f,0.f };
	m_vUV[1] = _vec2{ m_vMoveUV.x,0.f };
	m_vUV[2] = _vec2{ m_vMoveUV.x,m_vMoveUV.y };
	m_vUV[3] = _vec2{ 0.f,m_vMoveUV.y };
	m_vMaxUv.x = 4.f;
	m_vMaxUv.y = 4.f;
}

void CEffect::SetProtoUV()
{
	m_vMoveUV.x = 1.f;
	m_vMoveUV.y = 1.f;
	m_vUV[0] = _vec2{ 0.f,0.f };
	m_vUV[1] = _vec2{ m_vMoveUV.x,0.f };
	m_vUV[2] = _vec2{ m_vMoveUV.x,m_vMoveUV.y };
	m_vUV[3] = _vec2{ 0.f,m_vMoveUV.y };
}



HRESULT CEffect::Add_Component(void)
{
	CComponent*		pComponent = nullptr;
	

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Prototype(L"Proto_Shader_Effect"));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);


	// Renderer
	//pComponent = m_pRendererCom = Engine::Get_Renderer();
	//NULL_CHECK_RETURN(m_pRendererCom, E_FAIL);
	//pComponent->AddRef();
	//m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);
	//
	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
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


	pEffect->SetVector("g_vColor", &m_vColor);

	if (m_iPassIdx == 3)
	{
		pEffect->SetFloat("g_fLifeSpeed", m_fLifeSpeed*m_fFrameSpeed);
		pEffect->SetFloat("g_fLifeTime", m_fLifeTime);

	}

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", m_iTextIdx);

	return S_OK;
}

_float CEffect::GetRandomFloat(_float lowBound, _float highBound)
{
	if (lowBound >= highBound) // bad input
		return lowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}