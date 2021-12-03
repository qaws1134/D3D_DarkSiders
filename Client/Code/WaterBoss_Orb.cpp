#include "stdafx.h"
#include "WaterBoss_Orb.h"
#include "ColMesh.h"
#include "Export_Function.h"
#include "GameMgr.h"
#include "Bullet.h"
#include "SoundMgr.h"
CWaterBoss_Orb::CWaterBoss_Orb(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CWaterBoss_Orb::CWaterBoss_Orb(const CWaterBoss_Orb& rhs)
	: CGameObject(rhs)
{

}

CWaterBoss_Orb::~CWaterBoss_Orb(void)
{

}

HRESULT CWaterBoss_Orb::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(0.15f, 0.15f, 0.15f);
	m_bActive = false;
	m_vColor = _vec4(0.0f, 0.05f,0.6f, 1.f);
	m_iPass = 6;

	m_fHitTime = 0.5f;
	SetCharInfo(5.f, 0.f);

	return S_OK;
}

_int CWaterBoss_Orb::Update_Object(const _float& fTimeDelta)
{


	if (!m_bActive)
	{
		m_fBulletSpeed = 0.f;
		return 0;
	}

	DeadCheck();

	_int iExit = CGameObject::Update_Object(fTimeDelta);

	m_vColor = _vec4(0.0f, 0.05f, 0.6f, 1.f);

	if (!m_bDead)
	{
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(2.f));
		m_pTransformCom->Rotation(ROT_X, D3DXToRadian(1.f));

		m_fBulletSpeed += fTimeDelta;
		if (m_fBulletSpeed > m_fBulletTime)
		{
			CGameObject* pBullet = nullptr;
			CTransform* pBulletTrans = nullptr;


			_vec3 vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			CTransform* pTrans = dynamic_cast<CTransform*>(CGameMgr::GetInstance()->GetPlayer()->Get_Component(L"Com_Transform", ID_DYNAMIC));

			_vec3 vPlayerPos;
			pTrans->Get_INFO(INFO_POS, &vPlayerPos);
			_vec3 vBulletDir = vPlayerPos - vPos;

			D3DXVec3Normalize(&vBulletDir, &vBulletDir);
			_vec3 vRight = _vec3(1.f, 0.f, 0.f);

			_vec3 vCross;
			D3DXVec3Cross(&vCross, &vRight, &vBulletDir);
			D3DXVec3Normalize(&vRight, &vRight);
			m_vDir = vRight;
			_float fCos = D3DXVec3Dot(&vRight, &vBulletDir);
			_float fAngle;
			fAngle = D3DXToDegree(acosf(fCos));

			if (vCross.y < 0.f)
			{
				fAngle = 360.f - fAngle;

			}
			pBullet = CGameMgr::GetInstance()->GetEnemyBullet(BULLET::BULLET_ORBLIGHTNING);
			dynamic_cast<CBullet*>(pBullet)->SetMove(true);
			pBulletTrans = dynamic_cast<CTransform*>(pBullet->Get_Component(L"Com_Transform", ID_DYNAMIC));

			pBulletTrans->Set_Rot(0.f, D3DXToRadian(fAngle + 90.f), 0.f);
			pBulletTrans->Set_Pos(&vPos);

			m_fBulletSpeed = 0.f;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_MONSTER_ATK_4);
			CSoundMgr::Get_Instance()->PlaySound(L"en_waterboss_summon_orb_start_01.ogg", CSoundMgr::CHANNEL_MONSTER_ATK_4);
		}
		if (m_bCol)
		{
			m_vColor = _vec4(1.f, 1.f, 1.f, 1.f);
			m_fHitSpeed = 0.f;
			m_bCol = false;
		}
		//_vec3 vPos;
		//m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		

	}
		Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CWaterBoss_Orb::Render_Object(void)
{

	_float fTimeDelta = Get_TimeDelta(L"Timer_Immediate");

	LPD3DXEFFECT	 pEffect = m_pShaderCom->Get_EffectHandle();
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iMaxPass = 0;

	pEffect->Begin(&iMaxPass, NULL);		// 1인자 : 현재 쉐이더 파일이 반환하는 pass의 최대 개수
				
						

	if(!m_bDead)
		pEffect->BeginPass(m_iPass);
	else
	{
		m_fDissolveAmount += fTimeDelta*1.f;
		pEffect->SetFloat("g_DissolveAmount", m_fDissolveAmount);
		m_pDissolveCom->Set_Texture(pEffect, "g_DissolveTexture", 1);

		pEffect->BeginPass(m_iPass);
	}


	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

}

CWaterBoss_Orb * CWaterBoss_Orb::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWaterBoss_Orb*	pInstance = new CWaterBoss_Orb(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CWaterBoss_Orb::Free(void)
{
	CGameObject::Free();
}

HRESULT CWaterBoss_Orb::Add_Component(void)
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

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Prototype(L"Proto_Shader_Mesh"));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pMeshCom = dynamic_cast<CStaticMesh*>(Clone_Prototype(L"Orb"));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Collider
	COLLIDERSPHERE tSphere;
	tSphere.fRadius = 0.01f;
	tSphere.vCenterPos= _vec3(0.f,0.f,0.f);

	pComponent = m_pColliderCom = CColliderSphere::Create(m_pGraphicDev, &m_tColSphere.vCenterPos, m_tColSphere.fRadius);
	NULL_CHECK_RETURN(m_pColliderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Collider", pComponent);



	pComponent = m_pDissolveCom = dynamic_cast<CTexture*>(Clone_Prototype(L"Proto_Texture_Dissolve"));
	NULL_CHECK_RETURN(m_pDissolveCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Dissolve", pComponent);


	return S_OK;
}
HRESULT CWaterBoss_Orb::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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
	pEffect->SetVector("g_vColor", &m_vColor);




	return S_OK;
}
void CWaterBoss_Orb::DeadCheck()
{
	if (m_tCharInfo.fHp <= 0.f)
	{
		m_bDead = true;
		m_iPass = 4;
		if (m_fDissolveAmount > 1.f)
		{
			CGameMgr::GetInstance()->RetunWaterBossOrb(this);
		}
	}
		
}
void CWaterBoss_Orb::TakeDmg(_float Atk)
{
	m_tCharInfo.fDmg = Atk;
	m_tCharInfo.fHp -= Atk;
	m_fHitSpeed = 0.f;
	
}