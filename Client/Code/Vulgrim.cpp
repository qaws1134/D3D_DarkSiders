#include "stdafx.h"
#include "Vulgrim.h"
#include "Enum.h"
#include "Player.h"
#include "Export_Function.h"
#include "GameMgr.h"
#include "UIMgr.h"



CVulgrim::CVulgrim(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CVulgrim::CVulgrim(const CVulgrim& rhs)
	: CGameObject(rhs)
{

}

CVulgrim::~CVulgrim(void)
{

}

HRESULT CVulgrim::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	m_pTransformCom->Update_Component(0.f);
	m_pMeshCom->Set_AnimationIndex(Vulgrim::Vulgrim_Idle);
	m_eMachineState = Vulgrim::STATE_IDLE;
	m_eCurAniState = Vulgrim::Vulgrim_Idle;


	return S_OK;
}

void CVulgrim::Late_Ready_Object()
{
}


_int CVulgrim::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);

	StateChange();
	StateLinker(fTimeDelta);
	Add_RenderGroup(RENDER_NONALPHA, this);
	m_pMeshCom->Play_Animation(fTimeDelta);

	return iExit;
}

CVulgrim* CVulgrim::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CVulgrim*	pInstance = new CVulgrim(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

CVulgrim * CVulgrim::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode)
{
	CVulgrim*	pInstance = new CVulgrim(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CVulgrim::Free(void)
{
	CGameObject::Free();
}


HRESULT CVulgrim::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Prototype(L"Vulgrim"));
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

void CVulgrim::Render_Object(void)
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

void CVulgrim::StateChange()
{
	auto iter_find = find_if(m_mapColider.begin(), m_mapColider.end(), CTag_Finder(L"Col_Body"));
	if (iter_find != m_mapColider.end())
	{
		if (iter_find->second->GetCol())
		{
			if (dynamic_cast<CPlayer*>(CGameMgr::GetInstance()->GetPlayer())->GetInteraction())
			{

				if (!m_bUIOn)
				{
					CUIMgr::GetInstance()->SetActiveStoreStoneUI(true);
					CGameMgr::GetInstance()->CameraEvent(this);
					dynamic_cast<CPlayer*>(CGameMgr::GetInstance()->GetPlayer())->SetOnUI(true);
					m_bUIOn = true;
				}
				else
				{
					if (CUIMgr::GetInstance()->GetStoreUIActive())
					{
						CUIMgr::GetInstance()->SetActiveStoreStoneUI(false);
						CGameMgr::GetInstance()->CameraEvent(this);
						dynamic_cast<CPlayer*>(CGameMgr::GetInstance()->GetPlayer())->SetOnUI(false);
						m_bUIOn = false;
					}
				}

				m_eMachineState = Vulgrim::STATE_BUY;
				dynamic_cast<CPlayer*>(CGameMgr::GetInstance()->GetPlayer())->ResetInteraction();
			}
		}
	}
	//플레이어 상태 전환 시 
	if (m_ePreMachineState != m_eMachineState)
	{
		switch (m_eMachineState)
		{
		case Vulgrim::STATE_IDLE:
			m_eCurAniState = Vulgrim::Vulgrim_Idle;
			break;
		case Vulgrim::STATE_BUY:
			m_eCurAniState = Vulgrim::Vulgrim_Buy_Item_02;
			break;
		case Vulgrim::STATE_END:
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
		case Vulgrim::Vulgrim_Idle:
			break;
		case Vulgrim::Vulgrim_Buy_Item_02:
			break;
		case Vulgrim::End:
			break;
		default:
			break;
		}
		m_ePreAniState = m_eCurAniState;
		m_pMeshCom->Set_AnimationIndex(m_eCurAniState,m_bBlend);
	}

}
//다음 동작으로 자동으로 연결 
void CVulgrim::StateLinker(_float fDeltaTime)
{

	switch (m_eCurAniState)
	{
	case Vulgrim::Vulgrim_Idle:
		break;
	case Vulgrim::Vulgrim_Buy_Item_02:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eMachineState = Vulgrim::STATE_IDLE;
		}
		break;
	case Vulgrim::End:
		break;
	default:
		break;
	}


}



HRESULT CVulgrim::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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
