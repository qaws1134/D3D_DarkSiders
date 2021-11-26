#include "stdafx.h"
#include "Serpent.h"
#include "Enum.h"
#include "Player.h"
#include "Stage.h"
#include "Export_Function.h"
#include "GameMgr.h"



CSerpent::CSerpent(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CSerpent::CSerpent(const CSerpent& rhs)
	: CGameObject(rhs)
{

}

CSerpent::~CSerpent(void)
{

}

HRESULT CSerpent::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	m_pTransformCom->Update_Component(0.f);
	m_pMeshCom->Set_AnimationIndex(Serpent::Serpent_Close_Idle);

	m_eCurAniState = Serpent::Serpent_Close_Idle;
	SetCharInfo(10.f, 0.f);

	return S_OK;
}

void CSerpent::Late_Ready_Object()
{
}


_int CSerpent::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);
	StateChange();
	StateLinker(fTimeDelta);
	m_pMeshCom->Play_Animation(fTimeDelta);

	return iExit;
}

CSerpent* CSerpent::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSerpent*	pInstance = new CSerpent(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

CSerpent * CSerpent::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode)
{
	CSerpent*	pInstance = new CSerpent(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CSerpent::Free(void)
{
	CGameObject::Free();
}


HRESULT CSerpent::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Prototype(L"Serpent"));
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

void CSerpent::Render_Object(void)
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

void CSerpent::StateChange()
{
	auto iter_find = find_if(m_mapColider.begin(), m_mapColider.end(), CTag_Finder(L"Col_Body"));
	if (iter_find != m_mapColider.end())
	{
		if (iter_find->second->GetCol())
		{
			m_eMachineState = Serpent::STATE_OPEN;
			if (dynamic_cast<CPlayer*>(CGameMgr::GetInstance()->GetPlayer())->GetInteraction())
			{
				//씬전환
				//씬에 씬전환 메시지 ㄱㄱ
				dynamic_cast<CPlayer*>(CGameMgr::GetInstance()->GetPlayer())->ResetInteraction();
				//m_bActive = false;
				//CScene*		pScene = nullptr;
				//pScene = CStage::Create(m_pGraphicDev);			
				//Set_Scene(pScene);
			}
		}
		else
		{
			m_eMachineState = Serpent::STATE_CLEOSE;
		}
	}

	if (m_ePreMachineState != m_eMachineState)
	{
		switch (m_eMachineState)
		{
		case Serpent::STATE_OPEN:
			m_eCurAniState = Serpent::Serpent_Open;
			m_bSpawn = true;
			break;
		case Serpent::STATE_CLEOSE:
			if (m_bSpawn)
			{
				m_eCurAniState = Serpent::Serpent_Close;
			}
			break;
		case Serpent::STATE_END:
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
		case Serpent::Serpent_Open:
			m_bBlend = false;
			break;
		case Serpent::Serpent_Open_Idle:
			break;
		case Serpent::Serpent_Close:
			m_bBlend = false;
			break;
		case Serpent::Serpent_Close_Idle:
			break;
		case Serpent::End:
			break;
		default:
			break;
		}
		m_ePreAniState = m_eCurAniState;
		m_pMeshCom->Set_AnimationIndex(m_eCurAniState,m_bBlend);
	}

}
//다음 동작으로 자동으로 연결 
void CSerpent::StateLinker(_float fDeltaTime)
{
	switch (m_eCurAniState)
	{
	case Serpent::Serpent_Open:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = Serpent::Serpent_Open_Idle;
		}
		break;
	case Serpent::Serpent_Open_Idle:
		break;
	case Serpent::Serpent_Close:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = Serpent::Serpent_Close_Idle;
		}
		break;
	case Serpent::Serpent_Close_Idle:
		break;
	case Serpent::End:
		break;
	default:
		break;
	}

}
HRESULT CSerpent::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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
