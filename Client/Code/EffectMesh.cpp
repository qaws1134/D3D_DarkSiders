#include "stdafx.h"
#include "EffectMesh.h"

#include "Export_Function.h"

CEffectMesh::CEffectMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CEffectMesh::CEffectMesh(const CEffectMesh& rhs)
	: CGameObject(rhs)
{

}

CEffectMesh::~CEffectMesh(void)
{

}

HRESULT CEffectMesh::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fUVSpeed = GetRandomFloat(0.2f, 0.5f);
	return S_OK;
}

_int CEffectMesh::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);
	m_fAccTime += fTimeDelta;
	Add_RenderGroup(RENDER_NONALPHA,this);
	return iExit;
}

void CEffectMesh::Render_Object(void)
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

CEffectMesh * CEffectMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectMesh*	pInstance = new CEffectMesh(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

CEffectMesh * CEffectMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh)
{
	CEffectMesh*	pInstance = new CEffectMesh(pGraphicDev);

	pInstance->SetProtoMesh(ProtoMesh);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CEffectMesh::Free(void)
{
	CGameObject::Free();
}

HRESULT CEffectMesh::Add_Component(void)
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
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Prototype(L"Proto_Shader_Effect"));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	if (m_wstrProtoMesh != L"")
	{
		//Mesh
		pComponent = m_pMeshCom = dynamic_cast<CStaticMesh*>(Clone_Prototype(m_wstrProtoMesh.c_str()));
		NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
		m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);
	}

	return S_OK;
}
HRESULT CEffectMesh::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
{
	_matrix		matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	_matrix matWorldView, matWorldViewProj;

	D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
	D3DXMatrixMultiply(&matWorldViewProj, &matWorldView, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld); // 월드행렬이 따로 필요할 수 있다.
	pEffect->SetMatrix("g_matWorldViewProj", &matWorldViewProj);


	pEffect->SetFloat("g_fUVTime", m_fAccTime);
	pEffect->SetFloat("g_fUVSpeed", m_fUVSpeed);




	return S_OK;
}


_float CEffectMesh::GetRandomFloat(_float lowBound, _float highBound)
{
	if (lowBound >= highBound) // bad input
		return lowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}

void CEffectMesh::SetOption(void * pArg)
{
	CComponent*		pComponent = nullptr;
	if (pArg)
	{
		memcpy(&m_eEffect, pArg, sizeof(EFFECT::TYPE));
	}
	m_bActive = true;
	
	switch (m_eEffect)
	{
	case EFFECT::EFFECT_LIGHTNING:
		break;
	case EFFECT::EFFECT_BOLT:
		break;
	case EFFECT::EFFECT_ELECTRIC1:
		break;
	case EFFECT::EFFECT_ELECTRIC2:
		break;
	case EFFECT::EFFECT_FOG:
		break;
	case EFFECT::EFFECT_FOGGROUP:
		break;
	case EFFECT::EFFECT_AURA:
		break;
	case EFFECT::EFFECT_WATERBOOM:
		break;
	case EFFECT::EFFECT_CHARGE:
		break;
	case EFFECT::EFFECT_END:
		break;
	default:
		break;
	}

	////초기값 저장 
	m_eInitNextEffect = m_eNextEffect;

	const _tchar* pConvComponentTag = W2BSTR((L"Com" + m_wstrProtoMesh).c_str());

	// Collider
	auto& iter_find = find_if(m_mapComponent[ID_STATIC].begin(), m_mapComponent[ID_STATIC].end(), CTag_Finder(pConvComponentTag));

	if (iter_find == m_mapComponent[ID_STATIC].end())
	{
		//미리 등록해둔 텍스쳐 크론따서 컴포넌트로 저장
		pComponent = m_pMeshCom = dynamic_cast<CStaticMesh*>(Clone_Prototype(m_wstrProtoMesh.c_str()));
		NULL_CHECK_RETURN(m_pMeshCom, );
		m_mapComponent[ID_STATIC].emplace(pConvComponentTag, pComponent);
	}
	else
	{
		m_pMeshCom = dynamic_cast<CStaticMesh*>(iter_find->second);
	}


}
