#include "stdafx.h"
#include "JumpBall.h"
#include "GameMgr.h"
#include "Player.h"
#include "Export_Function.h"

CJumpBall::CJumpBall(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CJumpBall::CJumpBall(const CJumpBall& rhs)
	: CGameObject(rhs)
{

}

CJumpBall::~CJumpBall(void)
{

}

HRESULT CJumpBall::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_bActive = true;
	m_fHitTime = 2.f;
	return S_OK;
}

_int CJumpBall::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);
	
	_vec3 vInitPos;
	m_pTransformCom->Get_INFO(INFO_POS, &vInitPos);
	m_pColliderCom->Set_Center(vInitPos);
	
	if (!m_bHit)
	{
		if (m_bCol)
		{
			dynamic_cast<CPlayer*>(CGameMgr::GetInstance()->GetPlayer())->Set_JumpBall(true);
			m_fHitSpeed = 0.f;

		}
	}

	Add_RenderGroup(RENDER_NONALPHA,this);
	return iExit;
}

void CJumpBall::Render_Object(void)
{
	LPD3DXEFFECT	 pEffect = m_pShaderCom->Get_EffectHandle();
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iMaxPass = 0;

	pEffect->Begin(&iMaxPass, NULL);		
					
	m_pColliderCom->Render_Collider(COLTYPE(m_bCol), m_pTransformCom->Get_WorldMatrix());

	pEffect->BeginPass(0);

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

}

CJumpBall * CJumpBall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CJumpBall*	pInstance = new CJumpBall(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

CJumpBall * CJumpBall::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh)
{
	CJumpBall*	pInstance = new CJumpBall(pGraphicDev);

	pInstance->SetProtoMesh(ProtoMesh);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CJumpBall::Free(void)
{
	CGameObject::Free();
}

HRESULT CJumpBall::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Transform", pComponent);

	// Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(m_pRendererCom, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);


	// Shader
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Prototype(L"Proto_Shader_Mesh"));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pMeshCom = dynamic_cast<CStaticMesh*>(Clone_Prototype(L"JumpBall"));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);


	pComponent = m_pColliderCom = CColliderSphere::Create(m_pGraphicDev, &_vec3(0.f,0.f,0.f),20.f);
	NULL_CHECK_RETURN(m_pColliderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Collider", pComponent);



	return S_OK;
}
HRESULT CJumpBall::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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