#include "stdafx.h"
#include "EffectMesh.h"

#include "Export_Function.h"
#include "GameMgr.h"
#include "Player.h"

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
	
	m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
	m_pTransformCom->Update_Component(0.f);


	return S_OK;
}

_int CEffectMesh::Update_Object(const _float& fTimeDelta)
{


	_int iExit = CGameObject::Update_Object(fTimeDelta);
	
	if (m_bActive)
	{
		if (m_bLife)
		{
			m_fLifeSpeed += fTimeDelta;
			if (m_fLifeSpeed > m_fLifeTimer)
			{
				m_bDead = true;
				if (!m_bDissolve)
					CGameMgr::GetInstance()->RetunEffect3D(this);
				else
				{
					m_fDissolveAmount += fTimeDelta*0.4f;
					if (m_fDissolveAmount > 1.f)
					{
						CGameMgr::GetInstance()->RetunEffect3D(this);
					}
				}
			}
		}
		m_fAccTime += fTimeDelta;


		UpdateEffect(fTimeDelta);
		
		if(!m_bCluster&& m_bActive)
			Add_RenderGroup(RENDER_EFFECT, this);

	}
	
	return iExit;
}

void CEffectMesh::Render_Object(void)
{


	_float fTimeDelta = Get_TimeDelta(L"Timer_Immediate");

	if (m_pMeshCom)
	{
		LPD3DXEFFECT	 pEffect = m_pShaderCom->Get_EffectHandle();
		pEffect->AddRef();

		FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

		_uint iMaxPass = 0;

		pEffect->Begin(&iMaxPass, NULL);

		if (m_bDissolve&&m_bDead)
		{
			pEffect->SetFloat("g_DissolveAmount", m_fDissolveAmount);
			m_pDissolveCom->Set_Texture(pEffect, "g_DissolveTexture", 1);
			pEffect->BeginPass(4);
		}
		else
		{
			pEffect->BeginPass(0);

		}

		m_pMeshCom->Render_Meshes(pEffect);

		pEffect->EndPass();
		pEffect->End();

		Safe_Release(pEffect);
	}

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
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(m_pRendererCom, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);


	// Shader
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Prototype(L"Proto_Shader_Mesh"));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);


	pComponent = m_pDissolveCom = dynamic_cast<CTexture*>(Clone_Prototype(L"Proto_Texture_Dissolve"));
	NULL_CHECK_RETURN(m_pDissolveCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Dissolve", pComponent);


	return S_OK;
}
HRESULT CEffectMesh::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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

	pEffect->SetVector("g_vColor", (_vec4*)&m_vColor);

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
		memcpy(&m_eEffect, pArg, sizeof(EFFECT::TYPE3D));
	}
	m_bActive = true;
	

	switch (m_eEffect)
	{
	case EFFECT::EFFECT3D_STONECLUSTER:
		m_fSpawnTime = 0.01f;
		m_queCluster.emplace(EFFECT::EFFECT3D_STONE1);
		m_queCluster.emplace(EFFECT::EFFECT3D_STONE2);
		m_queCluster.emplace(EFFECT::EFFECT3D_STONE1);
		m_queCluster.emplace(EFFECT::EFFECT3D_STONE2);
		m_queCluster.emplace(EFFECT::EFFECT3D_STONE1);
		m_queCluster.emplace(EFFECT::EFFECT3D_STONE2);
		m_queCluster.emplace(EFFECT::EFFECT3D_STONE1);
		m_queCluster.emplace(EFFECT::EFFECT3D_STONE2);
		m_queCluster.emplace(EFFECT::EFFECT3D_STONE1);
		m_queCluster.emplace(EFFECT::EFFECT3D_STONE2);
		m_queCluster.emplace(EFFECT::EFFECT3D_STONE1);
		m_queCluster.emplace(EFFECT::EFFECT3D_STONE2);
		m_queCluster.emplace(EFFECT::EFFECT3D_STONE1);
		m_queCluster.emplace(EFFECT::EFFECT3D_STONE2);
		m_queCluster.emplace(EFFECT::EFFECT3D_STONE1);
		m_queCluster.emplace(EFFECT::EFFECT3D_STONE2);
		m_queCluster.emplace(EFFECT::EFFECT3D_STONE1);
		m_queCluster.emplace(EFFECT::EFFECT3D_STONE2);
		m_queCluster.emplace(EFFECT::EFFECT3D_STONE1);
		m_queCluster.emplace(EFFECT::EFFECT3D_STONE2);
		m_bCluster=true;
		return;
	case EFFECT::EFFECT3D_STONE1:
		m_wstrProtoMesh = L"StoneSingle0";
		
		m_pTransformCom->Set_Scale(GetRandomFloat(0.005f, 0.015f), GetRandomFloat(0.005f, 0.015f), GetRandomFloat(0.005f, 0.015f));
		m_pTransformCom->Update_Component(0.f);
		m_fLifeTimer = 2.f;
		m_fLifeSpeed = 0.f;
		m_bLife = true;
		m_bDissolve = true;
		break;
	case EFFECT::EFFECT3D_STONE2:
		m_wstrProtoMesh = L"StoneSingle1";
		m_pTransformCom->Set_Scale(GetRandomFloat(0.005f, 0.015f), GetRandomFloat(0.005f, 0.015f), GetRandomFloat(0.005f, 0.015f));
		m_pTransformCom->Update_Component(0.f);
		m_fLifeTimer = 2.f;
		m_fLifeSpeed = 0.f;
		m_bLife = true;
		m_bDissolve = true;
		break;
	case EFFECT::EFFECT3D_STONE_CIRCLE:
		m_wstrProtoMesh = L"StoneCluster0";
		m_pTransformCom->Set_Scale(0.013f, 0.013f,0.013f);
		m_pTransformCom->Update_Component(0.f);
		m_fLifeTimer = 2.f;
		m_fLifeSpeed = 0.f;
		m_bDissolve = true;
		m_bLife = true;
		break;
	case EFFECT::EFFECT3D_CHAGE:

		break;
	case EFFECT::EFFECT3D_CHAGE_CIRCLE:
		break;
	case EFFECT::EFFECT3D_END:
		m_fSpawnTime = 0.f;
		m_fSpawnSpeed= 0.f;
		m_ClusteriCount = 0;
		m_vDir = _vec3(0.f, 0.f, 0.f);
		m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
		m_bCluster = false;
		m_fLifeTimer = 0.f;
		m_fLifeSpeed = 0.f;
		m_bLife = false;
		m_bDissolve = false;
		m_bDead = false;
		m_bActive = false;
		return;
		break;
	default:
		break;
	}


	if (m_wstrProtoMesh == L"")
		return;
	USES_CONVERSION;
	////초기값 저장 
	m_eInitNextEffect = m_eNextEffect;

	const _tchar* pConvComponentTag = W2BSTR((m_wstrProtoMesh).c_str());


	auto& iter_find = find_if(m_mapComponent[ID_STATIC].begin(), m_mapComponent[ID_STATIC].end(), CTag_Finder(pConvComponentTag));

	if (iter_find == m_mapComponent[ID_STATIC].end())
	{
		//미리 등록해둔 텍스쳐 크론따서 컴포넌트로 저장
		pComponent = m_pMeshCom = dynamic_cast<CStaticMesh*>(Clone_Prototype(pConvComponentTag));
		NULL_CHECK_RETURN(m_pMeshCom, );
		m_mapComponent[ID_STATIC].emplace(pConvComponentTag, pComponent);
	}
	else
	{
		m_pMeshCom = dynamic_cast<CStaticMesh*>(iter_find->second);
	}


}

void CEffectMesh::UpdateEffect(const _float & fTimeDelta)
{

	switch (m_eEffect)
	{
	case EFFECT::EFFECT3D_STONECLUSTER:
		switch (m_eEffect)
		{
		case EFFECT::EFFECT3D_STONECLUSTER:
			if (!m_queCluster.empty())
			{
				m_fSpawnSpeed += fTimeDelta;
				if (m_fSpawnSpeed > m_fSpawnTime)
				{
					CGameObject* pObj = CGameMgr::GetInstance()->GetEffect3D(m_queCluster.front());
					m_queCluster.pop();
					_vec3* vPos = GetPos(ID_DYNAMIC);
					_vec3 vDir = dynamic_cast<CPlayer*>(CGameMgr::GetInstance()->GetPlayer())->GetDir();
					D3DXVec3Normalize(&vDir, &vDir);
					pObj->SetPos(*vPos + (vDir*(_float)m_ClusteriCount), ID_DYNAMIC);

					_vec3 vRight = _vec3(1.f, 0.f, 0.f);

					_vec3 vCross;
					D3DXVec3Cross(&vCross, &vRight, &vDir);
					D3DXVec3Normalize(&vRight, &vRight);
					_float fCos = D3DXVec3Dot(&vRight, &vDir);
					_float fAngle;
					fAngle = D3DXToDegree(acosf(fCos));
					if (vCross.y < 0.f)
					{
						fAngle = 360.f - fAngle;

					}
					CTransform* pTrans = dynamic_cast<CTransform*>(pObj->Get_Component(L"Com_Transform", ID_DYNAMIC));
					pTrans->Set_Rot(0.f, D3DXToRadian(fAngle + 90.f), 0.f);
					pObj = CGameMgr::GetInstance()->GetPlayerBullet(BULLET::BULLET_SPAWNFADE_PLAYER);
					pObj->SetPos(*vPos + (vDir*(_float)m_ClusteriCount), ID_DYNAMIC);
					m_ClusteriCount++;
					m_fSpawnSpeed = 0.f;
				}
				if (m_queCluster.empty())
				{
					CGameMgr::GetInstance()->RetunEffect3D(this);
				}
			}
			break;
		}
	case EFFECT::EFFECT3D_STONE1:
		break;
	case EFFECT::EFFECT3D_STONE2:
		break;
	case EFFECT::EFFECT3D_STONE_CIRCLE:
		break;
	case EFFECT::EFFECT3D_CHAGE:
		break;
	case EFFECT::EFFECT3D_CHAGE_CIRCLE:
		break;
	case EFFECT::EFFECT3D_END:
		break;
	default:
		break;
	}


}
