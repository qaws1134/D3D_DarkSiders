#include "stdafx.h"
#include "Goblin.h"
#include "Enum.h"
#include "Export_Function.h"



CGoblin::CGoblin(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CGoblin::CGoblin(const CGoblin& rhs)
	: CGameObject(rhs)
{

}

CGoblin::~CGoblin(void)
{

}

HRESULT CGoblin::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	m_pTransformCom->Set_Rot(0.f, D3DXToRadian(-90.f), 0.f);	//파싱하면서 바꿀꺼임 
	m_pTransformCom->Update_Component(0.f);
	m_pMeshCom->Set_AnimationIndex(Goblin::Goblin_Idle);
	m_fPatternTimer = 2.f;
	m_fPatternSpeed = 0.f;
	m_eCurAniState = Goblin::Goblin_Idle;
	m_iPatternNum = 0;


	SetCharInfo(50.f, 4.f);

	return S_OK;
}

void CGoblin::Late_Ready_Object()
{
}


_int CGoblin::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);
	StateChange();
	StateLinker(fTimeDelta);
	m_pMeshCom->Play_Animation(fTimeDelta);

	return iExit;
}

CGoblin* CGoblin::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGoblin*	pInstance = new CGoblin(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

CGoblin * CGoblin::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode)
{
	CGoblin*	pInstance = new CGoblin(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CGoblin::Free(void)
{
	CGameObject::Free();
}


HRESULT CGoblin::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Prototype(L"Goblin"));
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

void CGoblin::Render_Object(void)
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

void CGoblin::StateChange()
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
		case Goblin::Goblin_Attack_01:
			break;
		case Goblin::Goblin_Attack_02:
			break;
		case Goblin::Goblin_Attack_Spear:
			break;
		case Goblin::Goblin_Attack_Dash_Back:
			break;
		case Goblin::Goblin_Idle:
			break;
		case Goblin::Goblin_Impact_B:
			break;
		case Goblin::Goblin_Impact_F:
			break;
		case Goblin::Goblin_Jump_Apex:
			break;
		case Goblin::Goblin_Jump_Fall:
			break;
		case Goblin::Goblin_Jump_Land:
			break;
		case Goblin::Goblin_Jump_Launch:
			break;
		case Goblin::Goblin_Jump_Launch_Pose:
			break;
		case Goblin::Goblin_Sit_End:
			break;
		case Goblin::Goblin_Sit_Idle:
			break;
		case Goblin::Goblin_Spawn:
			break;
		case Goblin::Goblin_Spawn_Climb_Hovel:
			break;
		case Goblin::Goblin_Turn_L:
			break;
		case Goblin::Goblin_Turn_R:
			break;
		case Goblin::Goblin_Run_F:
			break;
		case Goblin::End:
			break;
		default:
			break;
		}
		m_ePreAniState = m_eCurAniState;
		m_pMeshCom->Set_AnimationIndex(m_eCurAniState, m_bBlend);
	}
}
//다음 동작으로 자동으로 연결 
void CGoblin::StateLinker(_float fDeltaTime)
{
	switch (m_eCurAniState)
	{
	case Goblin::Goblin_Attack_01:
		break;
	case Goblin::Goblin_Attack_02:
		break;
	case Goblin::Goblin_Attack_Spear:
		break;
	case Goblin::Goblin_Attack_Dash_Back:
		break;
	case Goblin::Goblin_Idle:
		if (Pattern_Timer(fDeltaTime))
		{
			SetPattern();
		}
		break;
	case Goblin::Goblin_Impact_B:
		break;
	case Goblin::Goblin_Impact_F:
		break;
	case Goblin::Goblin_Jump_Apex:
		break;
	case Goblin::Goblin_Jump_Fall:
		break;
	case Goblin::Goblin_Jump_Land:
		break;
	case Goblin::Goblin_Jump_Launch:
		break;
	case Goblin::Goblin_Jump_Launch_Pose:
		break;
	case Goblin::Goblin_Sit_End:
		break;
	case Goblin::Goblin_Sit_Idle:
		break;
	case Goblin::Goblin_Spawn:
		break;
	case Goblin::Goblin_Spawn_Climb_Hovel:
		break;
	case Goblin::Goblin_Turn_L:
		break;
	case Goblin::Goblin_Turn_R:
		break;
	case Goblin::Goblin_Run_F:
		break;
	case Goblin::End:
		break;
	default:
		break;
	}

}



HRESULT CGoblin::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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

_bool CGoblin::Pattern_Timer(_float fDeltaTime)
{
	m_fPatternSpeed += fDeltaTime;
	if (m_fPatternSpeed > m_fPatternTimer)
	{
		m_fPatternSpeed = 0.f;
		return true;
	}
	return false;
}

void CGoblin::SetPattern()
{
	switch (m_iPatternNum)
	{
	case 0:
		m_eCurAniState = Goblin::Goblin_Attack_01;
		break;
	case 1:
		m_eCurAniState = Goblin::Goblin_Attack_02;
		break;
	case 2:
		m_eCurAniState = Goblin::Goblin_Attack_Spear;
		break;
	}	
}

