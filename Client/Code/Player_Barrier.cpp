#include "stdafx.h"
#include "Player_Barrier.h"
#include "Enum.h"
#include "Export_Function.h"
#include "GameMgr.h"
#include "SoundMgr.h"
#include "SpawnMgr.h"
CPlayer_Barrier::CPlayer_Barrier(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CPlayer_Barrier::CPlayer_Barrier(const CPlayer_Barrier& rhs)
	: CGameObject(rhs)
{

}

CPlayer_Barrier::~CPlayer_Barrier(void)
{

}

HRESULT CPlayer_Barrier::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	//m_pTransformCom->Set_Rot(0.f, D3DXToRadian(-90.f), 0.f);	//파싱하면서 바꿀꺼임 
	m_pTransformCom->Update_Component(0.f);
	m_pMeshCom->Set_AnimationIndex(Player_Barrier::STATE_IDLE_OPEN);

	m_fSpawnTime = 5.f;
	m_bActive = false;
	m_eCurAniState = Player_Barrier::PlayerBarrer_Close; 

	return S_OK;
}

void CPlayer_Barrier::Late_Ready_Object()
{
}


_int CPlayer_Barrier::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);

	StateChange();
	StateActor(fTimeDelta);
	StateLinker(fTimeDelta);


	m_pMeshCom->Play_Animation(m_fFrameSpeed);
	if (m_bActive)
	{
		Add_RenderGroup(RENDER_NONALPHA, this);
	}
	return iExit;
}

CPlayer_Barrier* CPlayer_Barrier::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer_Barrier*	pInstance = new CPlayer_Barrier(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

CPlayer_Barrier * CPlayer_Barrier::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode)
{
	CPlayer_Barrier*	pInstance = new CPlayer_Barrier(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_Barrier::Free(void)
{
	CGameObject::Free();
}


HRESULT CPlayer_Barrier::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Prototype(L"PlayerBarrier"));
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

void CPlayer_Barrier::Render_Object(void)
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

void CPlayer_Barrier::StateChange()
{
	//플레이어 상태 전환 시 
	if (m_ePreMachineState != m_eMachineState)
	{
	
		switch (m_eMachineState)
		{
		case Player_Barrier::STATE_IDLE_OPEN:
			m_eCurAniState = Player_Barrier::PlayerBarrer_Close;

			m_bActive = false;
			break;
		case Player_Barrier::STATE_IDLE_CLOSE:
			m_eCurAniState = Player_Barrier::PlayerBarrer_Open;

			m_fSpawnSpeed = 0.f;
			m_bActive = true;
			break;
		case Player_Barrier::STATE_OPEN:
			m_eCurAniState = Player_Barrier::PlayerBarrer_Open;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_DOOR);
			CSoundMgr::Get_Instance()->PlaySound(L"prop_gondola_start.ogg", CSoundMgr::CHANNEL_DOOR);
			m_bActive = true;
			break;
		case Player_Barrier::STATE_CLEOSE:
			m_eCurAniState = Player_Barrier::PlayerBarrer_Close;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_DOOR);
			CSoundMgr::Get_Instance()->PlaySound(L"prop_gondola_stop.ogg", CSoundMgr::CHANNEL_DOOR);	
			m_bActive = true;
			break;
		case Player_Barrier::STATE_END:
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
		case Player_Barrier::PlayerBarrer_Close:
			m_bBlend = false;
			break;
		case Player_Barrier::PlayerBarrer_Open:
			m_bBlend = false;
			break;
		case Player_Barrier::End:
			break;
		default:
			break;
		}
		m_ePreAniState = m_eCurAniState;
		m_pMeshCom->Set_AnimationIndex(m_eCurAniState,m_bBlend);
	}

}
void CPlayer_Barrier::StateActor(_float fDeltaTime)
{
	switch (m_eMachineState)
	{
	case Player_Barrier::STATE_IDLE_OPEN:
		m_fFrameSpeed = 0.f;
		break;
	case Player_Barrier::STATE_IDLE_CLOSE:
	{
		if (!m_bSpawn)
		{
			m_fSpawnSpeed += fDeltaTime;
			if (m_fSpawnSpeed > m_fSpawnTime)
			{
				CGameMgr::GetInstance()->SpawnSet((_uint)SpawnSet::GrinnerFiledSp);
				m_bSpawn = true;

			}
		}
		else
		{
			vector<CGameObject*> m_vecEnemy= CSpawnMgr::GetInstance()->GetGrinnerVec();
			
			if (!m_vecEnemy.empty())
			{
				for (auto iter : m_vecEnemy)
				{
					if (iter->GetActive())
					{
						m_bClose = false;
						break;
					}
					else
						m_bClose = true;
				}
				if (m_bClose)
				{
					m_eMachineState = Player_Barrier::STATE_OPEN;
				}
			}
		}
		m_fFrameSpeed = 0.f;
	}
		break;
	case Player_Barrier::STATE_OPEN:
		m_fFrameSpeed = fDeltaTime;
		break;
	case Player_Barrier::STATE_CLEOSE:
		m_fFrameSpeed = fDeltaTime;
		break;
	case Player_Barrier::STATE_END:
		break;
	default:
		break;
	}
	switch (m_eCurAniState)
	{
	case Player_Barrier::PlayerBarrer_Close:
		break;
	case Player_Barrier::PlayerBarrer_Open:
		break;
	case Player_Barrier::End:
		break;
	default:
		break;
	}
}
//다음 동작으로 자동으로 연결 
void CPlayer_Barrier::StateLinker(_float fDeltaTime)
{
	switch (m_eCurAniState)
	{
	case Player_Barrier::PlayerBarrer_Close:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eMachineState = Player_Barrier::STATE_IDLE_CLOSE;
		}
		break;
	case Player_Barrier::PlayerBarrer_Open:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eMachineState = Player_Barrier::STATE_IDLE_OPEN;
		}
		break;
	case Player_Barrier::End:
		break;
	default:
		break;
	}

}

void CPlayer_Barrier::SetOption(void * pArg)
{
	if (pArg)
	{
		memcpy(&m_eMachineState, pArg, sizeof(_uint));
	}
	m_bActive = true;

}



HRESULT CPlayer_Barrier::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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

