#include "stdafx.h"
#include "Grinner.h"
#include "Enum.h"
#include "Export_Function.h"



CGrinner::CGrinner(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CGrinner::CGrinner(const CGrinner& rhs)
	: CGameObject(rhs)
{

}

CGrinner::~CGrinner(void)
{

}

HRESULT CGrinner::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	m_pTransformCom->Set_Rot(0.f, D3DXToRadian(-90.f), 0.f);	//파싱하면서 바꿀꺼임 
	m_pTransformCom->Update_Component(0.f);
	m_pMeshCom->Set_AnimationIndex(Grinner::Grinner_Idle);
	m_fPatternTimer = 2.f;
	m_fPatternSpeed = 0.f;
	m_eCurAniState = Grinner::Grinner_Idle;
	m_iPatternNum = 0;


	m_fHitSpeed = 1.f;
	m_fHitTime = 0.3f;

	SetCharInfo(50.f, 4.f);

	return S_OK;
}

void CGrinner::Late_Ready_Object()
{
}


_int CGrinner::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);

	StateChange();
	
	StateLinker(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);
	return iExit;
}

CGrinner* CGrinner::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGrinner*	pInstance = new CGrinner(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

CGrinner * CGrinner::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode)
{
	CGrinner*	pInstance = new CGrinner(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CGrinner::Free(void)
{
	CGameObject::Free();
}


HRESULT CGrinner::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Prototype(L"Grinner"));
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

void CGrinner::Render_Object(void)
{
	_float fTimeDelta = Get_TimeDelta(L"Timer_Immediate");
	StateLinker(fTimeDelta);
	m_pMeshCom->Play_Animation(fTimeDelta);

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



void CGrinner::StateChange()
{
	//기본 상태 시 무기상태 false 
	for (_uint i = 0; i < 3; i++)
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
		case Grinner::STATE_SPAWN_IDLE:
			m_eCurAniState = Grinner::Grinner_Idle;
			break;
		case Grinner::STATE_SPAWN:
			if(Grinner::SPAWN_POTRAL==m_eSpawnType)
				m_eCurAniState = Grinner::Grinner_PotalSpawn;
			else if (Grinner::SPAWN_APEX == m_eSpawnType)
				m_eCurAniState = Grinner::Grinner_Jump_Apex;
			break;
		case Grinner::STATE_IDLE:
			m_eCurAniState = Grinner::Grinner_Idle;
			m_fPatternSpeed = 0.f;

			if(m_ePreMachineState == Grinner::STATE_HIT)
				m_fPatternTimer = 1.f;
			else
				m_fPatternTimer = 2.f;
			break;
		case Grinner::STATE_ATK:
			m_iPatternNum = RandNext(0, 4);
			SetPattern();

			break;
		case Grinner::STATE_MOVE:
			break;
		case Grinner::STATE_CHASE:
			//if()가까우면
			m_eCurAniState = Grinner::Grinner_Walk_F;
			m_eCurAniState = Grinner::Grinner_Run_F;
			break;
		case Grinner::STATE_HIT:
			if (m_bHit)
				break;
			//USES_CONVERSION;
			for (auto iter : m_mapColider)
			{
				//레이어는 뒤에 메쉬 명 붙여서
				
				//const _tchar* pConvLayerTag = W2BSTR(LayerTag.c_str());

				wstring ColKey = iter.first;
				if (iter.second->GetCol())
				{
					if (m_tCharInfo.fDmg < 2.f)
					{
						if (L"Col_Front" == ColKey)
						{
							m_eCurAniState = Grinner::Grinner_Impact_Flinch_F;
						}
						else if (L"Col_Back" == ColKey)
						{
							m_eCurAniState = Grinner::Grinner_Impact_Flinch_B;
						}
						else if (L"Col_Left" == ColKey)
						{
							m_eCurAniState = Grinner::Grinner_Impact_Flinch_L;	
						}
						else if (L"Col_Right" == ColKey)
						{
							m_eCurAniState = Grinner::Grinner_Impact_Flinch_R;
						}
					}
					else if(m_tCharInfo.fDmg<3.f)
					{
						if (L"Col_Front" == iter.first)
						{
							m_eCurAniState = Grinner::Grinner_Impact_F;
						}
						else if (L"Col_Back" == iter.first)
						{
							m_eCurAniState = Grinner::Grinner_Impact_Flinch_B;
						}
						else if (L"Col_Left" == iter.first)
						{
							m_eCurAniState = Grinner::Grinner_Impact_L;
						}
						else if (L"Col_Right" == iter.first)
						{
							m_eCurAniState = Grinner::Grinner_Impact_R;
						}
					}
					else
					{
						m_eCurAniState = Grinner::Grinner_Knock_B_Start;			
					}
					//iter.second->SetCol(false);
					m_fHitSpeed = 0.f;
					m_tCharInfo.fDmg = 0.f;
				}
			}		
			break;
		case Grinner::STATE_DEAD:
			break;
		case Grinner::STATE_END:
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
		case Grinner::Grinner_Atk_Flip:
			break;
		case Grinner::Grinner_Atk_BarfinRainbows:
			break;
		case Grinner::Grinner_Atk_Lunge:
			break;
		case Grinner::Grinner_Atk_Swipe_Combo:
			break;
		case Grinner::Grinner_DeadPose:
			break;
		case Grinner::Grinner_Death:
			break;
		case Grinner::Grinner_Death_War:
			break;
		case Grinner::Grinner_DeathPose_War:
			break;
		case Grinner::Grinner_Idle:
			break;
		case Grinner::Grinner_Impact_F:
			break;
		case Grinner::Grinner_Impact_Flinch_B:
		case Grinner::Grinner_Impact_Flinch_F:
		case Grinner::Grinner_Impact_Flinch_L:
		case Grinner::Grinner_Impact_Flinch_R:
			m_bBlend = false;
			break;
		case Grinner::Grinner_Impact_L:
			break;
		case Grinner::Grinner_Impact_R:
			break;
		case Grinner::Grinner_Jump_Apex:
			break;
		case Grinner::Grinner_Jump_Fall:
			break;
		case Grinner::Grinner_Jump_Land:
			break;
		case Grinner::Grinner_Jump_Launch:
			break;
		case Grinner::Grinner_Knock_B_Start:
			break;
		case Grinner::Grinner_Knock_B_Apex:
			break;
		case Grinner::Grinner_Knock_B_Fall:
			break;
		case Grinner::Grinner_Knock_B_Land:
			break;
		case Grinner::Grinner_Knock_B_Recover:
			break;
		case Grinner::Grinner_Knock_B_Idle:
			break;
		case Grinner::Grinner_PotalSpawn:
			break;
		case Grinner::Grinner_Run_F:
			break;
		case Grinner::Grinner_Run_FL:
			break;
		case Grinner::Grinner_Run_FR:
			break;
		case Grinner::Grinner_Turn_90_L:
			break;
		case Grinner::Grinner_Turn_90_R:
			break;
		case Grinner::Grinner_Turn_180_L:
			break;
		case Grinner::Grinner_Turn_180_R:
			break;
		case Grinner::Grinner_Walk_B:
			break;
		case Grinner::Grinner_Walk_BL:
			break;
		case Grinner::Grinner_Walk_BR:
			break;
		case Grinner::Grinner_Walk_F:
			break;
		case Grinner::Grinner_Walk_L:
			break;
		case Grinner::Grinner_Walk_R:
			break;
		default:
			break;
		}
		m_ePreAniState = m_eCurAniState;
		m_pMeshCom->Set_AnimationIndex(m_eCurAniState,m_bBlend);
	}

}
void CGrinner::StateActor(_float fDeltaTime)
{

	_vec3 vPos;
	m_pTransformCom->Get_INFO(INFO_POS, &vPos);
	m_vDir = *m_pTarget->GetPos(ID_DYNAMIC) - vPos;

	_vec3 vLook;
	m_pTransformCom->Get_INFO(INFO_LOOK, &vLook);
	_vec3 vCross;
	D3DXVec3Cross(&vCross, &vLook, &m_vDir);
	D3DXVec3Normalize(&vLook, &vLook);
	_float fCos = D3DXVec3Dot(&vLook, &m_vDir);
	_float fAngle = D3DXToDegree(acosf(fCos));

	_float fAngleOffset = 3.f;

	if (vCross.y < 0.f)
	{
		fAngle = 360.f - fAngle;
	}

	switch (m_eMachineState)
	{
	case Grinner::STATE_SPAWN_IDLE:
		break;
	case Grinner::STATE_SPAWN:
		break;
	case Grinner::STATE_IDLE:
		break;
	case Grinner::STATE_ATK:
		break;
	case Grinner::STATE_MOVE:
		break;
	case Grinner::STATE_CHASE:
		
		break;
	case Grinner::STATE_HIT:
		break;
	case Grinner::STATE_DEAD:
		break;
	case Grinner::STATE_END:
		break;
	default:
		break;
	}
}
//다음 동작으로 자동으로 연결 
void CGrinner::StateLinker(_float fDeltaTime)
{

	switch (m_eCurAniState)
	{
	case Grinner::Grinner_Atk_Flip:
		break;
	case Grinner::Grinner_Atk_BarfinRainbows:
		break;
	case Grinner::Grinner_Atk_Lunge:
		break;
	case Grinner::Grinner_Atk_Swipe_Combo:
		break;
	case Grinner::Grinner_DeadPose:
		break;
	case Grinner::Grinner_Death:
		break;
	case Grinner::Grinner_Death_War:
		break;
	case Grinner::Grinner_DeathPose_War:
		break;
	case Grinner::Grinner_Idle:
		if (m_eMachineState != Grinner::STATE_CHASE)
		{
			if (Pattern_Timer(fDeltaTime))
			{
				m_eMachineState = Grinner::STATE_ATK;
			}
		}
		break;
	case Grinner::Grinner_Impact_F:
	case Grinner::Grinner_Impact_Flinch_B:
	case Grinner::Grinner_Impact_Flinch_F:
	case Grinner::Grinner_Impact_Flinch_L:
	case Grinner::Grinner_Impact_Flinch_R:
	case Grinner::Grinner_Impact_L:
	case Grinner::Grinner_Impact_R:
	case Grinner::Grinner_Jump_Land:
	case Grinner::Grinner_Knock_B_Recover:
	case Grinner::Grinner_PotalSpawn:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eMachineState = Grinner::STATE_IDLE;
		}
		break;
	case Grinner::Grinner_Jump_Apex:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = Grinner::Grinner_Jump_Fall;
		}
		break;
	case Grinner::Grinner_Jump_Fall:
			m_eCurAniState = Grinner::Grinner_Jump_Land;
		break;
	case Grinner::Grinner_Jump_Launch:
		break;	
	case Grinner::Grinner_Knock_B_Start:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = Grinner::Grinner_Knock_B_Apex;
		}
		break;
	case Grinner::Grinner_Knock_B_Apex:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = Grinner::Grinner_Knock_B_Fall;
		}
		break;
	case Grinner::Grinner_Knock_B_Fall:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = Grinner::Grinner_Knock_B_Land;
		}
		break;
	case Grinner::Grinner_Knock_B_Land:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = Grinner::Grinner_Knock_B_Idle;
		}
		break;
	case Grinner::Grinner_Knock_B_Idle:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = Grinner::Grinner_Knock_B_Recover;
		}
		break;

	case Grinner::Grinner_Run_F:
		break;
	case Grinner::Grinner_Run_FL:
		break;
	case Grinner::Grinner_Run_FR:
		break;
	case Grinner::Grinner_Turn_90_L:
		break;
	case Grinner::Grinner_Turn_90_R:
		break;
	case Grinner::Grinner_Turn_180_L:
		break;
	case Grinner::Grinner_Turn_180_R:
		break;
	case Grinner::Grinner_Walk_B:
		break;
	case Grinner::Grinner_Walk_BL:
		break;
	case Grinner::Grinner_Walk_BR:
		break;
	case Grinner::Grinner_Walk_F:
		break;
	case Grinner::Grinner_Walk_L:
		break;
	case Grinner::Grinner_Walk_R:
		break;
	default:
		break;
	}

}



HRESULT CGrinner::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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

_bool CGrinner::Pattern_Timer(_float fDeltaTime)
{
	m_fPatternSpeed += fDeltaTime;
	if (m_fPatternSpeed > m_fPatternTimer)
	{
		m_fPatternSpeed = 0.f;
		return true;
	}
	return false;
}

void CGrinner::SetPattern()
{
	switch (m_iPatternNum)
	{
	case 0:
		m_eCurAniState = Grinner::Grinner_Atk_Flip;
		break;
	case 1:
		m_eCurAniState = Grinner::Grinner_Atk_BarfinRainbows;
		break;
	case 2:
		m_eCurAniState = Grinner::Grinner_Atk_Lunge;
		break;
	case 3:
		m_eCurAniState = Grinner::Grinner_Atk_Swipe_Combo;
		break;

	}	
}


void CGrinner::TakeDmg(_float fDmg)
{
	m_tCharInfo.fDmg = fDmg;
	m_tCharInfo.fHp -= fDmg;
	m_eMachineState = Grinner::STATE_HIT;
}

