#include "stdafx.h"
#include "Chest.h"
#include "Enum.h"
#include "Player.h"
#include "Export_Function.h"
#include "GameMgr.h"
#include "SoundMgr.h"



CChest::CChest(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CChest::CChest(const CChest& rhs)
	: CGameObject(rhs)
{

}

CChest::~CChest(void)
{

}

HRESULT CChest::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	m_pTransformCom->Update_Component(0.f);
	m_pMeshCom->Set_AnimationIndex(Chest::Chest_Closed);

	m_eCurAniState = Chest::Chest_Closed;

	SetCharInfo(10.f, 0.f);

	return S_OK;
}

void CChest::Late_Ready_Object()
{
}


_int CChest::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return 0;
	_int iExit = CGameObject::Update_Object(fTimeDelta);



	if (CGameMgr::GetInstance()->EventAngel())
	{
		StateChange();
		StateLinker(fTimeDelta);
	}
	Add_RenderGroup(RENDER_NONALPHA, this);
	m_pMeshCom->Play_Animation(fTimeDelta);

	return iExit;
}

CChest* CChest::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CChest*	pInstance = new CChest(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

CChest * CChest::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode)
{
	CChest*	pInstance = new CChest(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChest::Free(void)
{
	CGameObject::Free();
}


HRESULT CChest::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Prototype(L"Chest"));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Renderer
	//pComponent = m_pRendererCom = Engine::Get_Renderer();
	//NULL_CHECK_RETURN(m_pRendererCom, E_FAIL);
	//pComponent->AddRef();
	//m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

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

void CChest::Render_Object(void)
{

	_float fTimeDelta = Get_TimeDelta(L"Timer_Immediate");
	LPD3DXEFFECT	 pEffect = m_pShaderCom->Get_EffectHandle();
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iMaxPass = 0;

	pEffect->Begin(&iMaxPass, NULL);		// 1인자 : 현재 쉐이더 파일이 반환하는 pass의 최대 개수
							
	if(m_eCurAniState != Chest::Chest_Opened)
		pEffect->BeginPass(0);
	else
	{
		m_fDissolveAmount += fTimeDelta*0.4f;
		pEffect->SetFloat("g_DissolveAmount", m_fDissolveAmount);

		pEffect->BeginPass(6);
	}
	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();
	pEffect->End();


	Safe_Release(pEffect);
}

void CChest::StateChange()
{

	auto iter_find = find_if(m_mapColider.begin(), m_mapColider.end(), CTag_Finder(L"Col_Body"));
	if (iter_find != m_mapColider.end())
	{
		if (iter_find->second->GetCol())
		{
			//ui띄우기 

			if (dynamic_cast<CPlayer*>(CGameMgr::GetInstance()->GetPlayer())->GetInteraction())
			{
				m_eMachineState = Chest::STATE_OPEN;
				dynamic_cast<CPlayer*>(CGameMgr::GetInstance()->GetPlayer())->SetOnUI(true);
				dynamic_cast<CPlayer*>(CGameMgr::GetInstance()->GetPlayer())->Set_PlayerState(War::CHEST_OPNE);
				dynamic_cast<CPlayer*>(CGameMgr::GetInstance()->GetPlayer())->ResetInteraction();
			}
		}
	}
	//플레이어 상태 전환 시 
	if (m_ePreMachineState != m_eMachineState)
	{
		switch (m_eMachineState)
		{
		case Chest::STATE_IDLE:
			break;
		case Chest::STATE_OPEN:
			m_eCurAniState = Chest::Chest_Start;
			break;
		case Chest::STATE_OPEND:
			m_eCurAniState = Chest::Chest_Opened;
			
			break;
		case Chest::STATE_HIT:
			m_eCurAniState = Chest::Chest_Impact;
			break;
		case Chest::STATE_DEAD:
			m_eMachineState = Chest::STATE_OPEND;
			break;
		case Chest::STATE_END:
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
		case Chest::Chest_Idle:
			break;
		case Chest::Chest_Start:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_CHEST);
			CSoundMgr::Get_Instance()->PlaySound(L"prop_chest_limbo_idle_start", CSoundMgr::CHANNEL_CHEST);
			break;
		case Chest::Chest_Opened:
			break;
		case Chest::Chest_Open:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_CHEST);
			CSoundMgr::Get_Instance()->PlaySound(L"prop_chest_limbo_open.ogg", CSoundMgr::CHANNEL_CHEST);
			m_bBlend = false;
			break;
		case Chest::Chest_Impact:
		{
			USES_CONVERSION;

			_uint iIdx = RandNext(0, 3);
			wstring wstrSound = L"prop_chest_limbo_impact_0";
			wstring wstrTag = L".ogg";
			wstrSound += to_wstring(iIdx);
			wstrSound += wstrTag;
			TCHAR* pTag = W2BSTR(wstrSound.c_str());
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_CHEST);
			CSoundMgr::Get_Instance()->PlaySound(pTag, CSoundMgr::CHANNEL_CHEST);
		}
			break;
		case Chest::Chest_Closed:
			break;
		case Chest::End:
			break;
		default:
			break;
		}
		m_ePreAniState = m_eCurAniState;
	}
		m_pMeshCom->Set_AnimationIndex(m_eCurAniState,m_bBlend);

}
//다음 동작으로 자동으로 연결 
void CChest::StateLinker(_float fDeltaTime)
{
	switch (m_eMachineState)
	{
	case Chest::STATE_HIT:
		m_eMachineState = Chest::STATE_IDLE;
		break;
	case Chest::STATE_END:
		break;
	default:
		break;
	}



	switch (m_eCurAniState)
	{
	case Chest::Chest_Idle:
		if (m_pMeshCom->Is_Animationset(0.3))
		{
			m_eCurAniState = Chest::Chest_Open;
			CGameObject* pObj;
			_vec3 vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			for (_uint i = 0; i < 20; i++)
			{
				pObj = CGameMgr::GetInstance()->GetItem(DROPITEM::ITEM_SOUL);
				pObj->SetPos(vPos, ID_DYNAMIC);
				dynamic_cast<CNaviMesh*>(pObj->Get_Component(L"Com_Navi", ID_STATIC))->Set_CellIndex(29);

			}
			pObj = CGameMgr::GetInstance()->GetItem(DROPITEM::ITEM_STONE);
			dynamic_cast<CNaviMesh*>(pObj->Get_Component(L"Com_Navi", ID_STATIC))->Set_CellIndex(29);
			pObj->SetPos(vPos, ID_DYNAMIC);
		}
		break;
	case Chest::Chest_Start:
		if (m_pMeshCom->Is_Animationset(0.5))
		{
			m_eCurAniState = Chest::Chest_Idle;
		}
		break;
	case Chest::Chest_Opened:
		if (m_fDissolveAmount > 1.f)
			m_bActive = false;
		for (auto& iter : m_mapColider)
		{
			iter.second->SetDead(true);
		}
		m_bDead = true;
		break;
	case Chest::Chest_Open:
		if(m_pMeshCom->Is_Animationset(0.6))
			dynamic_cast<CPlayer*>(CGameMgr::GetInstance()->GetPlayer())->SetOnUI(false);
		


		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = Chest::Chest_Opened;
		}
		break;
	case Chest::Chest_Impact:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = Chest::Chest_Closed;
		}
		break;
	case Chest::Chest_Closed:
		break;
	case Chest::End:
		break;
	default:
		break;
	}

}

void CChest::TakeDmg(_float Atk)
{
	m_tCharInfo.fDmg = Atk;
	m_tCharInfo.fHp -= 1.f;
	m_eMachineState = Chest::STATE_HIT;
}


HRESULT CChest::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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
