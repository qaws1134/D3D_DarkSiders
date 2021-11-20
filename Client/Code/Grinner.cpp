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
	m_pMeshCom->Play_Animation(fTimeDelta);

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
	//플레이어 상태 전환 시 
	if (m_ePreMachineState != m_eMachineState)
	{
		switch (m_eMachineState)
		{
		case Grinner::STATE_SPAWN_IDLE:
			break;
		case Grinner::STATE_SPAWN:
			break;
		case Grinner::STATE_IDLE:
			break;
		case Grinner::STATE_ATK:
			m_iPatternNum = RandNext(0, 4);
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
			break;
		case Grinner::Grinner_Impact_Flinch_F:
			break;
		case Grinner::Grinner_Impact_Flinch_L:
			break;
		case Grinner::Grinner_Impact_Flinch_R:
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
		if (Pattern_Timer(fDeltaTime))
		{
			SetPattern();
		}
		break;
	case Grinner::Grinner_Impact_F:
		break;
	case Grinner::Grinner_Impact_Flinch_B:
		break;
	case Grinner::Grinner_Impact_Flinch_F:
		break;
	case Grinner::Grinner_Impact_Flinch_L:
		break;
	case Grinner::Grinner_Impact_Flinch_R:
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


