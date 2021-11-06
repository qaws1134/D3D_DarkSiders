#include "stdafx.h"
#include "StaticCamera.h"

#include "Export_Function.h"

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{

}

CStaticCamera::CStaticCamera(const CStaticCamera& rhs)
	: CCamera(rhs)
{

}

CStaticCamera::~CStaticCamera(void)
{

}

HRESULT CStaticCamera::Ready_Object(const CAMERA_DESC& tCameraDesc)
{
	m_tCameraDesc = tCameraDesc;

	memcpy(&m_tCameraDesc, &tCameraDesc, sizeof(CAMERA_DESC));


	// 플레이어 트랜스폼 가져옴 
	//	m_pTargetTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", ID_DYNAMIC));
	//m_pTargetTransform->AddRef();

	//if (nullptr == m_pTargetTransform)
	//{
	//	return E_FAIL;
	//}
	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);

	return S_OK;
}

Engine::_int CStaticCamera::Update_Object(const _float& fTimeDelta)
{
	if (m_pPreTarget !=  m_pTarget)
	{
		m_pTargetTransform = dynamic_cast<CTransform*>(m_pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC));
		m_pPreTarget = m_pTarget;
	}
	_int	iExit = CCamera::Update_Object(fTimeDelta);

	Move(fTimeDelta);

	return iExit;
}

void CStaticCamera::ChasingTarget(CTransform * pTargetTransform)
{
}

void CStaticCamera::ReleaseTarget()
{
}

void CStaticCamera::Move(_float fDeltaTime)
{
	_matrix matRot, matRotY;

	_vec3 vTargetRight;
	 m_pTargetTransform->Get_INFO(INFO_RIGHT,&vTargetRight);		// 플레이어 X축
	_vec3 vTargetUp;
	 m_pTargetTransform->Get_INFO(INFO_UP, &vTargetUp);		// 플레이어 Y축 
	_vec3 vTargetLook;
	m_pTargetTransform->Get_INFO(INFO_LOOK, &vTargetLook);		// 플레이어 Z축
	_vec3 vTargetPosition;
	m_pTargetTransform->Get_INFO(INFO_POS, &vTargetPosition);  // 포지션


	_vec3 vPlayerCameraDistance;

	if (GetAsyncKeyState(VK_ADD))
	{
		m_fDistanceFromTarget -= 10.f * fDeltaTime;
	}

	if (GetAsyncKeyState(VK_SUBTRACT))
	{
		m_fDistanceFromTarget += 10.f * fDeltaTime;
	}

	if (GetAsyncKeyState(VK_NUMPAD8))
	{
		m_fCamAngle += D3DXToRadian(45.f) * fDeltaTime;
	}

	if (GetAsyncKeyState(VK_NUMPAD2))
	{
		m_fCamAngle -= D3DXToRadian(45.f) * fDeltaTime;
	}

	D3DXVec3Normalize(&vTargetLook, &vTargetLook);
	D3DXVec3Normalize(&vTargetRight, &vTargetRight);
	D3DXVec3Normalize(&vTargetUp, &vTargetUp);


	vPlayerCameraDistance = (vTargetLook*1.1f) * -m_fDistanceFromTarget;
	D3DXMatrixRotationAxis(&matRot, &vTargetRight, m_fCamAngle); // 임의의 축 회전
	D3DXVec3TransformNormal(&vPlayerCameraDistance, &vPlayerCameraDistance, &matRot);

	m_tCameraDesc.vEye = vTargetPosition + vPlayerCameraDistance;


	m_tCameraDesc.vAt = vTargetPosition;

}







CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const CAMERA_DESC & tCameraDesc)
{
	CStaticCamera*		pInstance = new CStaticCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(tCameraDesc)))
		Safe_Release(pInstance);

	return pInstance;
}

void CStaticCamera::Free(void)
{
	Safe_Release(m_pTargetTransform);
	CCamera::Free();
}

