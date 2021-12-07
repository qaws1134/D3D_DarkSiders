#include "stdafx.h"
#include "Bullet.h"
#include "Export_Function.h"
#include "GameMgr.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CBullet::CBullet(const CBullet& rhs)
	: CGameObject(rhs)
{

}

CBullet::~CBullet(void)
{

}

HRESULT CBullet::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
	m_pTransformCom->Update_Component(0.f);
	return S_OK;
}

_int CBullet::Update_Object(const _float& fTimeDelta)
{


	_int iExit = CGameObject::Update_Object(fTimeDelta);
	if (m_bActive)
	{
		
		if (m_bMove)
		{
			m_pTransformCom->Get_INFO(INFO_LOOK, &m_vDir);
			D3DXVec3Normalize(&m_vDir, &m_vDir);
			m_pTransformCom->Move_Pos(&m_vDir, m_fMoveSpeed);
		}


	
		if (LifeTimer(fTimeDelta))
		{
			if(BULLET::BULLET_ENEMY == m_eID )
				CGameMgr::GetInstance()->RetunEnemyBullet(this);
			else
				CGameMgr::GetInstance()->RetunPlayerBullet(this);

			m_bActive = false;
		}
		
		
 		Add_RenderGroup(RENDER_NONALPHA, this);
	}

	

	return iExit;
}

void CBullet::Render_Object(void)
{
	m_pColliderCom->Render_Collider(COLTYPE(m_bCol), m_pTransformCom->Get_WorldMatrix());
	if (m_pMesh)
	{
		LPD3DXEFFECT	 pEffect = m_pShaderCom->Get_EffectHandle();
		pEffect->AddRef();

		FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

		_uint iMaxPass = 0;

		pEffect->Begin(&iMaxPass, NULL);		// 1인자 : 현재 쉐이더 파일이 반환하는 pass의 최대 개수
												// 2인자 : 시작하는 방식을 묻는 FLAG
		pEffect->BeginPass(0);

		m_pMesh->Render_Meshes(pEffect);

		pEffect->EndPass();
		pEffect->End();

		Safe_Release(pEffect);
	}

}

CBullet * CBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev,COLLIDERSPHERE tColShpere)
{
	CBullet*	pInstance = new CBullet(pGraphicDev);
	pInstance->SetColSphere(tColShpere);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}
CBullet * CBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBullet*	pInstance = new CBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}
void CBullet::Free(void)
{
	CGameObject::Free();
}

void CBullet::SetOption(void * pArg)
{
	USES_CONVERSION;
	CComponent*		pComponent = nullptr;
	if (pArg)
	{
		memcpy(&m_eType, pArg, sizeof(BULLET::TYPE));
	}

	switch (m_eType)
	{
	case BULLET::BULLET_CALLLIGHTNING:
		m_fLifeSpeed = 0.0f;
		m_fLifeTimer=0.7f;
		m_tColSphere.fRadius = 100.f;
		m_LifeTime = true;
		m_bActive = true;
		m_wstrBulletType = L"CallLightning";
		//텍스쳐 셋팅 ? 메시 셋팅?
		m_tCharInfo.fAtk = 3.f;
		m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
		m_eID = BULLET::BULLET_ENEMY;
		break;
	case BULLET::BULLET_ORBLIGHTNING:
		m_fLifeSpeed = 0.0f;
		m_fLifeTimer = 4.f;
		m_tColSphere.fRadius = 1.f;
		m_fMoveSpeed = 0.5f;
		m_LifeTime = true;
		m_bActive = true;
		m_tCharInfo.fAtk = 1.f;
		m_wstrBulletType = L"WaterBossBullet";
		m_pTransformCom->Set_Scale(0.05f, 0.05f, 0.05f);
		m_eID = BULLET::BULLET_ENEMY;
		break;
	case BULLET::BULLET_TSUNAMI:
		m_fLifeSpeed = 0.0f;
		m_fLifeTimer = 10.f;
		m_tColSphere.fRadius = 1.f;
		m_LifeTime = true;
		m_bActive = true;
		m_wstrBulletType = L"Tsunami";
		m_tCharInfo.fAtk = 3.f;
		m_eID = BULLET::BULLET_ENEMY;
		break;
	case BULLET::BULLET_GRINNERBARFIN:
		m_fLifeSpeed = 0.0f;
		m_fLifeTimer = 4.f;
		m_tColSphere.fRadius = 0.5f;
		m_fMoveSpeed = 0.4f;
		m_LifeTime = true;
		m_bActive = true;
		m_tCharInfo.fAtk = 1.f;
		m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
		m_wstrBulletType = L"GoblinSpear";
		m_eID = BULLET::BULLET_ENEMY;
		break;
	case BULLET::BULLET_GOBLINSPEAR:
		m_fLifeSpeed = 0.0f;
		m_fLifeTimer = 4.f;
		m_tColSphere.fRadius = 0.5f;
		m_fMoveSpeed = 0.4f;
		m_LifeTime = true;
		m_bActive = true;
		m_tCharInfo.fAtk = 1.f;
		m_wstrBulletType = L"GoblinSpear";
		m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
		m_eID = BULLET::BULLET_ENEMY;
		break;
	case BULLET::BULLET_SPAWNFADE_PLAYER:
		m_fLifeSpeed = 0.0f;
		m_fLifeTimer = 0.7f;
		m_tColSphere.fRadius = 1.f;
		m_LifeTime = true;
		m_bActive = true;
		m_wstrBulletType = L"SpawnFade";
		//텍스쳐 셋팅 ? 메시 셋팅?
		m_tCharInfo.fAtk = 2.f;
		m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
		m_eID = BULLET::BULLET_PLAYER;
		break;
	case BULLET::BULLET_END:
		m_fLifeSpeed = 0.f;
		m_fLifeTimer = 0.f;
		m_tColSphere.fRadius = 150.f;
		m_vDir = _vec3(0.f, 0.f, 0.f);
		m_bMove = false;
		m_pTransformCom->Set_Rot(0.f, 0.f, 0.f);
		m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
		m_pTransformCom->Set_Scale(0.f, 0.f, 0.f);
		m_LifeTime = false;
		m_bActive = false;
		m_fMoveSpeed = 0.f;
		m_wstrBulletType = L"End";
		m_pMesh = nullptr;
		m_eID = BULLET::ID_END;
		break;
	default:
		break;
	}
	m_tColSphere.vCenterPos= _vec3(0.f,m_tColSphere.fRadius,0.f);

	//충돌체 찾아서 붙여
	//충돌체 찾아서 활성화 시켜 -> 컴포넌트 이름을 총알명으로
	//이터 앤드면 만들고 아니면 찾아
	//미리 초기화



	const _tchar* pConvComponentTag = W2BSTR((L"Com_Col" + m_wstrBulletType).c_str());
	
	// Collider
	auto& iter_find = find_if(m_mapComponent[ID_STATIC].begin(), m_mapComponent[ID_STATIC].end(), CTag_Finder(pConvComponentTag));

	if (iter_find == m_mapComponent[ID_STATIC].end())
	{
		pComponent = m_pColliderCom = CColliderSphere::Create(m_pGraphicDev, &m_tColSphere.vCenterPos, m_tColSphere.fRadius);
		NULL_CHECK_RETURN(m_pColliderCom, );
		m_mapComponent[ID_STATIC].emplace(pConvComponentTag, pComponent);
	}
	else
	{
		m_pColliderCom = dynamic_cast<CColliderSphere*>(iter_find->second);
	}


	if (m_wstrBulletType == L"GoblinSpear"|| m_wstrBulletType == L"WaterBossBullet")
	{

		const _tchar* pConvMeshTag = W2BSTR((L"Com_Mesh" + m_wstrBulletType).c_str());
		iter_find = find_if(m_mapComponent[ID_STATIC].begin(), m_mapComponent[ID_STATIC].end(), CTag_Finder(pConvMeshTag));

		if (iter_find == m_mapComponent[ID_STATIC].end())
		{
			pComponent = m_pMesh = dynamic_cast<CStaticMesh*>(Clone_Prototype(m_wstrBulletType.c_str()));
			NULL_CHECK_RETURN(m_pColliderCom, );
			m_mapComponent[ID_STATIC].emplace(pConvMeshTag, pComponent);
		}
		else
		{
			m_pMesh = dynamic_cast<CStaticMesh*>(iter_find->second);
		}
	}
	
	//텍스처 없는 애만 처리?
	//매시 뼈에 붙여?

}

_bool CBullet::LifeTimer(_float fDeltaTime)
{
	m_fLifeSpeed += fDeltaTime;
	if (m_fLifeSpeed > m_fLifeTimer)
	{
		return true;
	}
	return false;
}

HRESULT CBullet::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

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
	//// Collider
	//pComponent = m_pColliderCom = CColliderSphere::Create(m_pGraphicDev, &m_tColSphere.vCenterPos, m_tColSphere.fRadius);
	//NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Collider", pComponent);

	return S_OK;
}
HRESULT CBullet::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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