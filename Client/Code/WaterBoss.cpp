#include "stdafx.h"
#include "WaterBoss.h"
#include "Enum.h"
#include "Export_Function.h"
#include "GameMgr.h"
#include "Effect.h"
#include "SoundMgr.h"


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
	m_eMachineState = WaterBoss::STATE_SPAWN;
	m_bActive = false;

	SetCharInfo(50.f, 4.f);

	return S_OK;
}

void CWaterBoss::Late_Ready_Object()
{
}


_int CWaterBoss::Update_Object(const _float& fTimeDelta)
{
	if (!m_bActive)
		return 0;
	_int iExit = CGameObject::Update_Object(fTimeDelta);
	StateChange();
	StateActor(fTimeDelta);
	StateLinker(fTimeDelta);
	m_pMeshCom->Play_Animation(fTimeDelta);

	if(m_bActive)
		Add_RenderGroup(RENDER_NONALPHA, this);

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

void CWaterBoss::SpawnTentaFog(_float fAngle, _vec3 vPos)
{

	CGameObject* pEff = CGameMgr::GetInstance()->GetEffect(EFFECT::EFFECT_FOG1_2x2);
	_float fRadius = 2.f;
	_vec3 vDir = { cosf(D3DXToRadian(fAngle))*fRadius,vPos.y,-sinf(D3DXToRadian(fAngle))*fRadius };	
	pEff->SetPos(vPos, ID_DYNAMIC);
	dynamic_cast<CEffect*>(pEff)->SetDir(-vDir);
}

void CWaterBoss::StateChange()
{
	//기본 상태 시 무기상태 false 
	for (_uint i = 0; i < 4; i++)
	{
		wstring wstrWeaponColKey = L"Col_Weapon" + to_wstring(i);
		auto iter_find = find_if(m_mapColider.begin(), m_mapColider.end(), CTag_Finder(wstrWeaponColKey.c_str()));
		if (iter_find != m_mapColider.end())
			iter_find->second->SetActive(false);
	}
	//플레이어 상태 전환 시 
	if (m_ePreMachineState != m_eMachineState)
	{
		switch (m_eMachineState)
		{
		case WaterBoss::STATE_SPAWN:
			m_eCurAniState = WaterBoss::Atk_Tentade_Pummel;
			break;
		case WaterBoss::STATE_IDLE:
			m_fPatternSpeed = 0.f;
			m_eCurAniState = WaterBoss::Idle;
			m_bSpawnEff = false;
			m_bBlend = true;
			for(_uint i = 0 ; i <4; i++)
				m_bPummel[i] = false;


			break;
		case WaterBoss::STATE_CALL_LIGHTNING:
			m_eCurAniState = WaterBoss::Atk_CallLightning_Start;


			break;
		case WaterBoss::STATE_ORB:
		{
			m_eCurAniState = WaterBoss::Atk_SummonOrb;

		}
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
			for(_uint i = 0 ; i <5; i++)
				m_queReadyEff2D.emplace(EFFECT::EFFECT_MAGIC_CIRCLE_SINGLE);
			m_fEffSpawnSpeed = 0.f;
			m_fEffSpawnTime = GetRandomFloat(0.3f, 1.f);

			m_bBlend = true;
			break;
		case WaterBoss::Atk_CallLightning:
			m_bBlend = true;
			break;
		case WaterBoss::Atk_SummonOrb:
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_MONSTER_ATK_2);
			CSoundMgr::Get_Instance()->PlaySound(L"en_waterboss_summon_orb_start_01.ogg", CSoundMgr::CHANNEL_MONSTER_ATK_2);
			m_bOrb = false;
			m_bBlend = true;
		}
			break;
		case WaterBoss::Atk_Tentade_Pummel:	
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_MONSTER_ATK_1);
			CSoundMgr::Get_Instance()->PlaySound(L"en_waterboss_tentacle_pummel.ogg", CSoundMgr::CHANNEL_MONSTER_ATK_1);
			m_bBlend = false;
			break;
		case WaterBoss::Atk_TentadeSlam_FL:	
		case WaterBoss::Atk_TentadeSlam_FL_02:	
		case WaterBoss::Atk_TentadeSlam_FR:			
		case WaterBoss::Atk_TentadeSlam_L:	
		case WaterBoss::Atk_TentadeSlam_L_02:
		case WaterBoss::Atk_TentadeSlam_R:
		case WaterBoss::Atk_TentadeSlam_R_02:
		{
			USES_CONVERSION;

			_uint iIdx = RandNext(0, 4);
			wstring wstrSound = L"en_waterboss_tentacle_0";
			wstring wstrTag = L".ogg";
			wstrSound += to_wstring(iIdx);
			wstrSound += wstrTag;
			TCHAR* pTag = W2BSTR(wstrSound.c_str());
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_MONSTER_ATK_1);
			CSoundMgr::Get_Instance()->PlaySound(pTag, CSoundMgr::CHANNEL_MONSTER_ATK_1);

			m_bBlend = false;
			break;
		}
		case WaterBoss::Atk_WhirlPool:
			m_bBlend = true;
			break;
		case WaterBoss::Impact_Stun:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_WATERBOSS);
			CSoundMgr::Get_Instance()->PlaySound(L"en_waterboss_stun.ogg", CSoundMgr::CHANNEL_WATERBOSS);
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
void CWaterBoss::StateActor(_float fDeltaTime)
{

	if (!m_queReadyEff2D.empty())
	{
		m_fEffSpawnSpeed += fDeltaTime;
		if (m_fEffSpawnSpeed > m_fEffSpawnTime)
		{
			EFFECT::TYPE2D eEffect2D = m_queReadyEff2D.front();
			CGameObject *pObj = CGameMgr::GetInstance()->GetEffect(eEffect2D);
			
			_vec3 vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			_vec3 vRight;
			m_pTransformCom->Get_INFO(INFO_RIGHT, &vRight);
			D3DXVec3Normalize(&vRight, &vRight);
			switch (eEffect2D)
			{
		
			case EFFECT::EFFECT_MAGIC_CIRCLE_SINGLE:
			{
				dynamic_cast<CEffect*>(pObj)->SetNextEffect(EFFECT::EFFECT_LIGHTNING_CLUSTER);
				dynamic_cast<CEffect*>(pObj)->SetNextEffect2(EFFECT::EFFECT_WATER_CLUSTER);
				//전방 
				_float fDistance = 15.f;
				vPos = vPos + (-vRight* fDistance);

				_float fRadius = GetRandomFloat(0.f,10.f);
				_float fAngle = GetRandomFloat(0.f, 360.f);

				//주변 원 
				_vec2 vDir = { cosf(D3DXToRadian(fAngle))*fRadius,-sinf(D3DXToRadian(fAngle))*fRadius };
				vPos.x += vDir.x;
				vPos.z += vDir.y;

				pObj->SetPos(vPos, ID_DYNAMIC);
			}
				break;
			case EFFECT::EFFECT_END:
				break;
			default:
				break;
			}

			m_queReadyEff2D.pop();

			m_fEffSpawnSpeed = 0.f;
		}
	}




	switch (m_eMachineState)
	{
	case WaterBoss::STATE_SPAWN:
		
		break;
	case WaterBoss::STATE_IDLE:
		break;
	case WaterBoss::STATE_CALL_LIGHTNING:
		
		break;
	case WaterBoss::STATE_ORB:
		break;
	case WaterBoss::STATE_SLAM:
		break;
	case WaterBoss::STATE_IMPACT:
		break;
	case WaterBoss::STATE_WAVE:
		break;
	case WaterBoss::STATE_END:
		break;
	default:
		break;
	}

	_vec3 vPos;
	switch (m_eCurAniState)
	{
	case WaterBoss::Atk_CallLightning_Start:
		break;
	case WaterBoss::Atk_CallLightning:
		break;
	case WaterBoss::Atk_SummonOrb:
		break;
	case WaterBoss::Atk_SummonOrb_L:
		break;
	case WaterBoss::Atk_Tentade_Pummel:
	{
		m_bSpawnEff = true;
		AtkColActive(0.2, 0.3, 0,&vPos);	
		if (m_pMeshCom->Is_Animationset(0.3 - 0.03))
		{
			if (!m_bPummel[0])
			{
				for (_uint i = 0; i < 9; i++)
				{
					_float fAngle = -80.f + (i * 20);
					SpawnTentaFog(fAngle, vPos);
				}
				m_bPummel[0] = true;
			}
		}
		AtkColActive(0.3, 0.4, 2,&vPos);
		if (m_pMeshCom->Is_Animationset(0.4 - 0.03))
		{
			if (!m_bPummel[1])
			{
				for (_uint i = 0; i < 9; i++)
				{
					_float fAngle = -80.f + (i * 20);
					SpawnTentaFog(fAngle, vPos);
				}
				m_bPummel[1] = true;
			}
		}
		AtkColActive(0.4, 0.5, 3,&vPos);
		if (m_pMeshCom->Is_Animationset(0.5 - 0.03))
		{
			if (!m_bPummel[2])
			{
				for (_uint i = 0; i < 9; i++)
				{
					_float fAngle = -80.f + (i * 20);
					SpawnTentaFog(fAngle, vPos);
				}
				m_bPummel[2] = true;
			}
		}
		AtkColActive(0.5, 0.6, 1,&vPos);
		if (m_pMeshCom->Is_Animationset(0.6 - 0.08))
		{
			if (!m_bPummel[3])
			{
				for (_uint i = 0; i < 9; i++)
				{
					_float fAngle = -80.f + (i * 20);
					SpawnTentaFog(fAngle, vPos);
				}
				m_bPummel[3] = true;
			}
		}

	}
		break;
	case WaterBoss::Atk_TentadeSlam_FL:
	{
		AtkColActive(0.3, 0.4, 0, &vPos);
	}
		break;
	case WaterBoss::Atk_TentadeSlam_FL_02:
	{
		AtkColActive(0.3, 0.4 , 0, &vPos);
	}
		break;
	case WaterBoss::Atk_TentadeSlam_FR:
		AtkColActive(0.3, 0.4, 3, &vPos);
		break;
	case WaterBoss::Atk_TentadeSlam_L:
		AtkColActive(0.3, 0.4, 1, &vPos);
		break;
	case WaterBoss::Atk_TentadeSlam_L_02:
		AtkColActive(0.3, 0.4, 1, &vPos);
		break;
	case WaterBoss::Atk_TentadeSlam_R:
		AtkColActive(0.3, 0.4, 2, &vPos);
		break;
	case WaterBoss::Atk_TentadeSlam_R_02:
		AtkColActive(0.3, 0.4, 2, &vPos);
		break;
	case WaterBoss::Atk_WhirlPool:
		break;
	case WaterBoss::Impact_Stun:
		break;
	case WaterBoss::Impact_Stun_Loop:
		break;
	case WaterBoss::TidalWave:
		break;
	case WaterBoss::TidalWave_Impact:
		break;
	case WaterBoss::TidalWave_Loop:
		break;
	case WaterBoss::Idle:
		break;
	case WaterBoss::End:
		break;
	default:
		break;
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
		if (m_pMeshCom->Is_Animationset(0.5))
		{
			if(!m_bOrb)
				SpawnOrb();
			m_bOrb = true;
		}
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eMachineState = WaterBoss::STATE_IDLE;
			m_fPatternTimer = 2.f;
			
		}
		break;
	case WaterBoss::Atk_Tentade_Pummel:
	{
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			if (m_eMachineState == WaterBoss::STATE_SPAWN)
			{
				CSoundMgr::Get_Instance()->PlayBGM(L"mus_level13_combat.ogg");
			}
			if (m_pMeshCom->Is_Animationset(0.9))
			{
				m_eMachineState = WaterBoss::STATE_IDLE;
				m_fPatternTimer = 1.f;
			}
		}
	}
	break;

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
	if (m_bCol)
	{
		m_vColor = { 1.f, 1.f, 1.f, 1.f };
		pEffect->SetVector("g_vColor", (_vec4*)&m_vColor);
		m_bCol = false;
	}
	
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

	//switch (m_iPatternNum)
	//{
	//case 0:
	//	m_eMachineState = WaterBoss::STATE_ORB;
	//	break;
	//case 1:
	//	m_eMachineState = WaterBoss::STATE_ORB;
	//	break;
	//case 2:
	//	m_eMachineState = WaterBoss::STATE_ORB;
	//	break;
	//case 3:
	//	m_eMachineState = WaterBoss::STATE_ORB;
	//	break;
	//case 4:
	//	m_eMachineState = WaterBoss::STATE_ORB;
	//	break;
	//case 5:
	//	m_eMachineState = WaterBoss::STATE_ORB;
	//	break;
	//case 6:
	//	m_eMachineState = WaterBoss::STATE_ORB;
	//	break;
	//case 7:
	//	m_eMachineState = WaterBoss::STATE_ORB;
	//	break;
	//case 8:
	//	m_eMachineState = WaterBoss::STATE_ORB;
	//	m_iPatternNum = 0;
	//	break;
	//}



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
		m_eMachineState = WaterBoss::STATE_ORB;
		break;
	case 4:
		m_eMachineState = WaterBoss::STATE_CALL_LIGHTNING;
		break;
	case 5:
		m_eMachineState = WaterBoss::STATE_SLAM;
		break;
	case 6:
		m_eMachineState = WaterBoss::STATE_CALL_LIGHTNING;
		break;
	case 7:
		m_eMachineState = WaterBoss::STATE_SLAM;
		break;
	case 8:
		m_eMachineState = WaterBoss::STATE_ORB;
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

void CWaterBoss::SetOption(void * pArg)
{
	m_eMachineState = WaterBoss::STATE_SPAWN;
	m_bActive = true;
	CSoundMgr::Get_Instance()->PlayBGM(L"mus_level13_combat_intro.ogg");
}

void CWaterBoss::SpawnOrb()
{
	CGameObject* pObj = CGameMgr::GetInstance()->GetWaterBossOrb();

	_vec3 vPos;
	m_pTransformCom->Get_INFO(INFO_POS, &vPos);
	_vec3 vLook;
	m_pTransformCom->Get_INFO(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);

	if (m_bOrbCross)
	{
		vLook *= -1.f;
	}
	m_bOrbCross = !m_bOrbCross;

	_float fDistance = 9.f;
	vPos = vPos + (vLook* fDistance);

	_float fRadius = GetRandomFloat(0.f, 4.f);
	_float fAngle = GetRandomFloat(0.f, 360.f);
	//주변 원 
	_vec2 vDir = { cosf(D3DXToRadian(fAngle))*fRadius,-sinf(D3DXToRadian(fAngle))*fRadius };
	vPos.x += vDir.x;
	vPos.z += vDir.y;
	if(pObj)
		pObj->SetPos(vPos, ID_DYNAMIC);
}

void CWaterBoss::DeadCheck()
{
	if (m_tCharInfo.fHp <= 0.f)
	{
		m_eMachineState = WaterBoss::STATE_DEAD;
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_EVNET);
		CSoundMgr::Get_Instance()->PlaySound(L"general_death_boss.ogg", CSoundMgr::CHANNEL_EVNET);
	}
}

void CWaterBoss::AtkColActive(double dStart, double dEnd,_uint iWeaponIdx,_vec3* pPos )
{
	auto iter_find = find_if(m_mapColider.begin(), m_mapColider.end(), CTag_Finder((L"Col_Weapon"+to_wstring(iWeaponIdx)).c_str()));
	if (iter_find == m_mapColider.end())
		return;

	_vec3 vPos;
	dynamic_cast<CTransform*>(iter_find->second->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_INFO(INFO_POS,&vPos);

	*pPos = vPos;
	if (m_pMeshCom->Is_Animationset(dEnd - 0.08))
	{
		if (!m_bSpawnEff)
		{
			for (_uint i = 0; i < 9; i++)
			{
				_float fAngle = -80.f + (i * 20);
				SpawnTentaFog(fAngle, vPos);
			}
			m_bSpawnEff = true;
		}
	}

	if (m_pMeshCom->Is_Animationset(dEnd))
	{

		iter_find->second->SetActive(false);
	}
	else if (m_pMeshCom->Is_Animationset(dStart))
	{
		
		iter_find->second->SetActive(true);
	}


}


_float CWaterBoss::GetRandomFloat(_float lowBound, _float highBound)
{
	if (lowBound >= highBound) // bad input
		return lowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}

void CWaterBoss::TakeDmg(_float fDmg)
{
	m_tCharInfo.fDmg = fDmg;
	m_tCharInfo.fHp -= fDmg;

	USES_CONVERSION;

	_uint iIdx = RandNext(0, 4);
	wstring wstrSound = L"en_dagon_vo_effort_impact_0";
	wstring wstrTag = L".ogg";
	wstrSound += to_wstring(iIdx);
	wstrSound += wstrTag;
	TCHAR* pTag = W2BSTR(wstrSound.c_str());
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_WATERBOSS);
	CSoundMgr::Get_Instance()->PlaySound(pTag, CSoundMgr::CHANNEL_WATERBOSS);



	//m_bHit = true;
}