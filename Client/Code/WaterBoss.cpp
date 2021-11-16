#include "stdafx.h"
#include "WaterBoss.h"
#include "Enum.h"
#include "Export_Function.h"



CWaterBoss::CWaterBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CWaterBoss::CWaterBoss(const CWaterBoss& rhs)
	: CGameObject(rhs)
{

}

CWaterBoss::~CWaterBoss(void)
{

}

HRESULT CWaterBoss::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	m_pTransformCom->Set_Rot(0.f, D3DXToRadian(-90.f), 0.f);	//파싱하면서 바꿀꺼임 
	m_pTransformCom->Update_Component(0.f);
	m_pMeshCom->Set_AnimationIndex(WaterBoss::Idle);
	m_fPatternTimer = 2.f;
	m_fPatternSpeed = 0.f;
	m_eCurAniState = WaterBoss::Idle;
	m_iSlamPatternNum = 0;
	m_iPatternNum = 0;
	m_bBlendTime = 0.5;


	SetCharInfo(50.f, 4.f);

	return S_OK;
}

void CWaterBoss::Late_Ready_Object()
{
}


_int CWaterBoss::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);
	StateChange();
	StateLinker(fTimeDelta);
	m_pMeshCom->Play_Animation(fTimeDelta);

	return iExit;
}

CWaterBoss* CWaterBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWaterBoss*	pInstance = new CWaterBoss(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

CWaterBoss * CWaterBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode)
{
	CWaterBoss*	pInstance = new CWaterBoss(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CWaterBoss::Free(void)
{
	CGameObject::Free();
}


HRESULT CWaterBoss::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Prototype(L"WaterBoss"));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(m_pRendererCom, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Calculator", pComponent);


	// Shader
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Prototype(L"Proto_Shader_Mesh"));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

void CWaterBoss::Render_Object(void)
{
	LPD3DXEFFECT	 pEffect = m_pShaderCom->Get_EffectHandle();
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iMaxPass = 0;

	pEffect->Begin(&iMaxPass, NULL);		// 1인자 : 현재 쉐이더 파일이 반환하는 pass의 최대 개수
											// 2인자 : 시작하는 방식을 묻는 FLAG
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();
	pEffect->End();


	Safe_Release(pEffect);
}

void CWaterBoss::StateChange()
{
	//플레이어 상태 전환 시 
	if (m_ePreMachineState != m_eMachineState)
	{
		switch (m_eMachineState)
		{
		case WaterBoss::STATE_IDLE:
			m_fPatternSpeed = 0.f;
			m_eCurAniState = WaterBoss::Idle;
			m_bBlend = true;

			break;
		case WaterBoss::STATE_CALL_LIGHTNING:
			m_eCurAniState = WaterBoss::Atk_CallLightning_Start;
			break;
		case WaterBoss::STATE_ORB:
			m_eCurAniState = WaterBoss::Atk_SummonOrb;
			break;
		case WaterBoss::STATE_SLAM:
			SetSlamPattern();
			break;
		case WaterBoss::STATE_IMPACT:
			if (WaterBoss::STATE_WAVE == m_ePreAniState)
				m_eCurAniState = WaterBoss::TidalWave_Impact;
			else
				m_eCurAniState = WaterBoss::Impact_Stun;
			m_fPatternTimer = 5.f;
			break;
		case WaterBoss::STATE_WAVE:
			m_eCurAniState = WaterBoss::TidalWave_Loop;
			m_fPatternTimer = 7.f;
			m_fPatternSpeed = 0.f;
			break;
		case WaterBoss::STATE_END:
			break;
		default:
			break;
		}

		m_ePreMachineState = m_eMachineState;
	}


	if (m_ePreAniState != m_eCurAniState)
	{
		switch (m_eCurAniState)
		{
		case WaterBoss::Atk_CallLightning_Start:
			m_bBlend = true;
			break;
		case WaterBoss::Atk_CallLightning:
			m_bBlend = true;
			break;
		case WaterBoss::Atk_SummonOrb:
			m_bBlend = true;
			break;
		case WaterBoss::Atk_Tentade_Pummel:	
		case WaterBoss::Atk_TentadeSlam_FL:	
		case WaterBoss::Atk_TentadeSlam_FL_02:	
		case WaterBoss::Atk_TentadeSlam_FR:			
		case WaterBoss::Atk_TentadeSlam_L:	
		case WaterBoss::Atk_TentadeSlam_L_02:
		case WaterBoss::Atk_TentadeSlam_R:
		case WaterBoss::Atk_TentadeSlam_R_02:
			m_bBlend = false;
			break;
		case WaterBoss::Atk_WhirlPool:
			m_bBlend = true;
			break;
		case WaterBoss::Impact_Stun:
			m_bBlend = true;
			break;
		case WaterBoss::Impact_Stun_Loop:
			m_bBlend = true;
			break;
		case WaterBoss::TidalWave:
			m_bBlend = true;
			break;
		case WaterBoss::TidalWave_Impact:
			m_bBlend = true;
			break;
		case WaterBoss::TidalWave_Loop:
			m_bBlend = true;
			break;
		case WaterBoss::Idle:

			break;
		case WaterBoss::End:
			break;
		default:
			break;
		}
		m_ePreAniState = m_eCurAniState;
		m_pMeshCom->Set_AnimationIndex(m_eCurAniState,m_bBlend);
	}

}
//다음 동작으로 자동으로 연결 
void CWaterBoss::StateLinker(_float fDeltaTime)
{
	switch (m_eCurAniState)
	{
	case WaterBoss::Atk_CallLightning_Start:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eCurAniState = WaterBoss::Atk_CallLightning;
		}

		break;
	case WaterBoss::Atk_CallLightning:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eMachineState = WaterBoss::STATE_IDLE;
			m_fPatternTimer = 2.f;

		}
		break;
	case WaterBoss::Atk_SummonOrb:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eMachineState = WaterBoss::STATE_IDLE;
			m_fPatternTimer = 2.f;

		}
		break;
	case WaterBoss::Atk_Tentade_Pummel:
	case WaterBoss::Atk_TentadeSlam_FL:
	case WaterBoss::Atk_TentadeSlam_FL_02:
	case WaterBoss::Atk_TentadeSlam_FR:
	case WaterBoss::Atk_TentadeSlam_L:
	case WaterBoss::Atk_TentadeSlam_L_02:
	case WaterBoss::Atk_TentadeSlam_R:
	case WaterBoss::Atk_TentadeSlam_R_02:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eMachineState = WaterBoss::STATE_IDLE;
			m_fPatternTimer = 1.f;
		}
		break;
	case WaterBoss::Atk_WhirlPool:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eMachineState = WaterBoss::STATE_IDLE;
			m_fPatternTimer = 5.f;
		}
		break;
	case WaterBoss::Impact_Stun:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eCurAniState = WaterBoss::Impact_Stun_Loop;
		}
		break;
	case WaterBoss::Impact_Stun_Loop:
		if (Pattern_Timer(fDeltaTime))
		{
			m_eMachineState = WaterBoss::STATE_IDLE;
		}
		break;
	case WaterBoss::TidalWave:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eMachineState = WaterBoss::STATE_IDLE;
		}
		break;
	case WaterBoss::TidalWave_Impact:
		if (Pattern_Timer(fDeltaTime))
		{
			m_eMachineState = WaterBoss::STATE_IDLE;
		}
		break;
	case WaterBoss::TidalWave_Loop:
		if (Pattern_Timer(fDeltaTime))
		{
			m_eCurAniState = WaterBoss::TidalWave;
		}

		break;
	case WaterBoss::Idle:
		if (Pattern_Timer(fDeltaTime))
		{
				SetPattern();
		}
		break;
	case WaterBoss::End:
		break;
	default:
		break;
	}

}



HRESULT CWaterBoss::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
{
	_matrix		matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	D3DMATERIAL9		tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrl.Power = 10.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&tMtrl.Diffuse);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&tMtrl.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&tMtrl.Ambient);

	pEffect->SetFloat("g_fPower", tMtrl.Power);

	const D3DLIGHT9*		pLightInfo = Get_Light();
	NULL_CHECK_RETURN(pLightInfo, E_FAIL);

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));

	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPos", (_vec4*)&matView._41);

	return S_OK;
}

_bool CWaterBoss::Pattern_Timer(_float fDeltaTime)
{
	m_fPatternSpeed += fDeltaTime;
	if (m_fPatternSpeed > m_fPatternTimer)
	{
		m_fPatternSpeed = 0.f;
		return true;
	}
	return false;
}

void CWaterBoss::SetPattern()
{
	switch (m_iPatternNum)
	{
	case 0:
		m_eMachineState = WaterBoss::STATE_CALL_LIGHTNING;
		break;
	case 1:
		m_eMachineState= WaterBoss::STATE_SLAM;
		break;
	case 2:
		m_eMachineState = WaterBoss::STATE_SLAM;
		break;
	case 3:
		m_eMachineState = WaterBoss::STATE_WAVE;
		break;
	case 4:
		m_eMachineState = WaterBoss::STATE_ORB;	
		break;
	case 5:
		m_eMachineState = WaterBoss::STATE_ORB;
		break;
	case 6:
		m_eMachineState = WaterBoss::STATE_CALL_LIGHTNING;
		break;
	case 7:
		m_eMachineState = WaterBoss::STATE_SLAM;
		break;
	case 8:
		m_eMachineState = WaterBoss::STATE_WAVE;
		m_iPatternNum = 0;
		break;
	}	
	m_iPatternNum++;
}

void CWaterBoss::SetSlamPattern()
{
	switch (m_iSlamPatternNum++)
	{
	case 0:
		m_eCurAniState = WaterBoss::Atk_TentadeSlam_FL;
		break;
	case 1:
		m_eCurAniState = WaterBoss::Atk_TentadeSlam_FL_02;
		break;
	case 2:
		m_eCurAniState = WaterBoss::Atk_Tentade_Pummel;
		break;
	case 3:
		m_eCurAniState = WaterBoss::Atk_TentadeSlam_FR;
		break;
	case 4:
		m_eCurAniState = WaterBoss::Atk_TentadeSlam_L;
		break;
	case 5:
		m_eCurAniState = WaterBoss::Atk_TentadeSlam_R;
		break;
	case 6:
		m_eCurAniState = WaterBoss::Atk_Tentade_Pummel;
		break;
	case 7:
		m_eCurAniState = WaterBoss::Atk_TentadeSlam_R_02;
		break;
	case 8:
		m_eCurAniState = WaterBoss::Atk_TentadeSlam_L_02;
		m_iSlamPatternNum = 0;
		break;
	default:
		break;
	}
}

