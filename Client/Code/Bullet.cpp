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
	
		if (LifeTimer(fTimeDelta))
		{
			CGameMgr::GetInstance()->RetunBullet(this);
			m_bActive = false;
		}
		
		
		Add_RenderGroup(RENDER_NONALPHA, this);
	}

	

	return iExit;
}

void CBullet::Render_Object(void)
{
	m_pColliderCom->Render_Collider(COLTYPE(m_bCol), m_pTransformCom->Get_WorldMatrix());


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
		m_fLifeTimer=0.5f;
		m_tColSphere.fRadius = 1.f;
		m_LifeTime = true;
		m_bActive = true;
		m_wstrBulletType = L"CallLightning";
		//�ؽ��� ���� ? �޽� ����?
		break;
	case BULLET::BULLET_ORBLIGHTNING:
		m_fLifeSpeed = 0.0f;
		m_fLifeTimer = 10.f;
		m_tColSphere.fRadius = 1.f;
		m_LifeTime = true;
		m_bActive = true;
		m_wstrBulletType = L"OrbLightning";
		break;
	case BULLET::BULLET_TSUNAMI:
		m_fLifeSpeed = 0.0f;
		m_fLifeTimer = 10.f;
		m_tColSphere.fRadius = 1.f;
		m_LifeTime = true;
		m_bActive = true;
		m_wstrBulletType = L"Tsunami";
		break;
	case BULLET::BULLET_END:
		m_fLifeSpeed = 0.f;
		m_fLifeTimer = 0.f;
		m_tColSphere.fRadius = 0.f;
		m_bActive = false;
		m_wstrBulletType = L"End";
		break;
	default:
		break;
	}
	m_tColSphere.vCenterPos= _vec3(0.f,m_tColSphere.fRadius,0.f);

	//�浹ü ã�Ƽ� �ٿ�
	//�浹ü ã�Ƽ� Ȱ��ȭ ���� -> ������Ʈ �̸��� �Ѿ˸�����
	//���� �ص�� ����� �ƴϸ� ã��
	//�̸� �ʱ�ȭ
	const _tchar* pConvComponentTag = W2BSTR((L"Com" + m_wstrBulletType).c_str());
	
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



	
	//�ؽ�ó ���� �ָ� ó��?
	//�Ž� ���� �ٿ�?

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
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(m_pRendererCom, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Calculator", pComponent);

	//// Collider
	//pComponent = m_pColliderCom = CColliderSphere::Create(m_pGraphicDev, &m_tColSphere.vCenterPos, m_tColSphere.fRadius);
	//NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Collider", pComponent);

	return S_OK;
}
