#include "stdafx.h"
#include "StaticCamera.h"
#include "GameMgr.h"
#include "Export_Function.h"
#include "Player.h"

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
	m_vecCamEvent.reserve(7);

	CComponent*		pComponent = nullptr;
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);


	memcpy(&m_tCameraDesc, &tCameraDesc, sizeof(CAMERA_DESC));
	D3DXMatrixIdentity(&m_matWorld);

	// 플레이어 트랜스폼 가져옴 
	//	m_pTargetTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", ID_DYNAMIC));
	//m_pTargetTransform->AddRef();

	//if (nullptr == m_pTargetTransform)
	//{
	//	return E_FAIL;
	//}
	m_pTarget = CGameMgr::GetInstance()->GetPlayer();
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


	if (m_bShake)
	{
		if (m_fShakeTime > 0.f)
		{
			m_tCameraDesc.vEye = GetRandomSphere()*m_fShakePower + m_vInitPos;
			m_fShakeTime -= fTimeDelta;
		}
		else
		{
			m_fShakeTime = 0.0f;
			m_tCameraDesc.vEye = m_vInitPos;
			m_bShake = false;
		}
	}

	//m_pGraphicDev->GetTransform(D3DTS_WORLD, &m_matWorld);
	_int	iExit = CCamera::Update_Object(fTimeDelta);
	Move(fTimeDelta);


	return iExit;
}

void CStaticCamera::SetOption(void * pArg)
{

	CComponent*		pComponent = nullptr;
	if (pArg)
	{
		memcpy(&m_iEventIdx, pArg, sizeof(_uint));
	}

	CAMEVENT tCamEvent;
	tCamEvent.bEventEnd = false;
	tCamEvent.iEventIdx = m_iEventIdx;
	switch (m_iEventIdx)
	{
	case 0:
	//	tCamEvent.iEventIdx = 0;
		tCamEvent.fEventAngle = 45.f;
		tCamEvent.fEventAngleSpeed = 1.f;
		m_fCamRotateY = D3DXToRadian(45.f);
		m_pTransformCom->Set_Rot(0.f, D3DXToRadian(45.f), 0.f);
		m_vecCamEvent.emplace_back(tCamEvent);
		break;
	case 4:
		//tCamEvent.iEventIdx = 4;

		m_fEventTime = 5.f;
		m_fEventSpeed = 0.f;
		tCamEvent.fEventFov =5;
		tCamEvent.fEventFovSpeed= 2.f;

		//이전값 저장 
		m_fPreCamRotateY = m_fCamRotateY;
		m_fPreDistanceFromTarget = m_fDistanceFromTarget;

		//이후 값 셋팅 
		m_fCamRotateY = D3DXToRadian(300.f);
		m_pTransformCom->Set_Rot(0.f, D3DXToRadian(300.f), 0.f);

		m_vecCamEvent.emplace_back(tCamEvent);
		break;
	case 14:

		m_fEventTime = 3.f;
		m_fEventSpeed = 0.f;
		tCamEvent.fEventFov = 14;
		//이전값 저장 
		m_fPreCamRotateY = m_fCamRotateY;
		m_fPreDistanceFromTarget = m_fDistanceFromTarget;

		//이후 값 셋팅 
		m_fCamRotateY = D3DXToRadian(90.f);
		m_pTransformCom->Set_Rot(0.f, D3DXToRadian(90.f), 0.f);
		m_vecCamEvent.emplace_back(tCamEvent);
		break;
	case 22:
		tCamEvent.fEventFov = 18;
		tCamEvent.fEventFovSpeed = 2.f;
		m_vecCamEvent.emplace_back(tCamEvent);
		break;
	case 27:	
		tCamEvent.fEventAngle = 90.f;
		tCamEvent.fEventAngleSpeed = 0.5f;
		m_vecCamEvent.emplace_back(tCamEvent);
		break;
	case 31:
		tCamEvent.fEventAngle = 210.f;
		tCamEvent.fEventAngleSpeed = 0.5f;
		m_vecCamEvent.emplace_back(tCamEvent);
		break;
	case 43:
		tCamEvent.fEventAngle = 140.f;
		m_fEventTime = 5.f;
		m_fEventSpeed = 0.f;
		tCamEvent.fEventFov = 30.f;
		tCamEvent.fEventFovSpeed = 2.f;
		//tCamEvent.fEventAngleSpeed = 0.5f;
		//보스 시작 씬 추가 
		m_vecCamEvent.emplace_back(tCamEvent);
		break;
	default:
		break;
	}


}

//
//void CStaticCamera::GetCameraINFO(INFO eType, _vec3* pInfo)
//{
//	memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3));
//}

_bool CStaticCamera::EventTimer(_float fDeltaTime)
{
	m_fEventSpeed += fDeltaTime;
	if (m_fEventSpeed > m_fEventTime)
	{
		return true;
	}
	return false;

}

//_matrix CStaticCamera::GetWorldMat()
//{
//	m_pGraphicDev->GetTransform(D3DTS_WORLD, &m_matWorld);
//	return m_matWorld;
//}
void CStaticCamera::SetTarget(CGameObject* pTarget)
{
	m_pPreTarget = m_pTarget;
	m_pTarget= pTarget;
}
void CStaticCamera::ReleaseTarget()
{
	m_pTarget = CGameMgr::GetInstance()->GetPlayer();
}

void CStaticCamera::ReleaseView()
{
	m_EventViewEnd = true;
}

void CStaticCamera::SetEventView()
{
	m_EventViewEnd = false;
	CTransform* pTransform = dynamic_cast<CTransform*>(m_pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC));
	_vec3 vLook;
	_vec3 vTargetRight;
	pTransform->Get_INFO(INFO_RIGHT, &vTargetRight);
	m_pTransformCom->Get_INFO(INFO_LOOK, &vLook);

	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vTargetRight, &vTargetRight);

	_vec3 vCross;
	D3DXVec3Cross(&vCross, &vLook, &-vTargetRight);

	_float fCos = D3DXVec3Dot(&vLook, &-vTargetRight);
	_float fAngle = D3DXToDegree(acosf(fCos));

	if (vCross.y < 0.f)
	{
		//fAngle = 360.f - fAngle;
		m_bCross = true;
	}

	m_tCamEvent.fEventAngle = fAngle-20.f;
	m_tCamEvent.fEventFov = 8.f;
	m_tCamEvent.fEventAngleSpeed = 1.f;
	m_tCamEvent.fEventFovSpeed= 5.f;

	m_tCamEvent.fEventAngleX = 30.f;
	m_tCamEvent.fEventAngleXSpeed = 1.f;

	m_fPreCamRotateY = m_fCamRotateY;
	m_fPreCamRotateX = m_fCamRotateX;
	m_fPreDistanceFromTarget= m_fDistanceFromTarget;

	m_bEventView = true;


}

_vec3 CStaticCamera::GetRandomSphere()
{
	_float fAngle = GetRandomFloat(0.f, 360.f);

	return _vec3{ cosf(D3DXToRadian(fAngle)),0.f,-sinf(D3DXToRadian(fAngle)) };
}

void CStaticCamera::Move(_float fDeltaTime)
{
	_matrix matRot, matRotY;
	_vec3 vTargetRight;
	_vec3 vTargetLook;
	_vec3 vTargetUp;
	_vec3 vTargetPosition;
	if (m_pTargetTransform)
	{
		m_pTargetTransform->Get_INFO(INFO_RIGHT, &vTargetRight);		// 플레이어 X축
		m_pTargetTransform->Get_INFO(INFO_UP, &vTargetUp);		// 플레이어 Y축 
		m_pTargetTransform->Get_INFO(INFO_LOOK, &vTargetLook);		// 플레이어 Z축
		m_pTargetTransform->Get_INFO(INFO_POS, &vTargetPosition);  // 포지션
	}
	else
	{
		vTargetPosition = _vec3{ 0.f,0.f,0.f };
		vTargetUp = _vec3{ 0.f,1.f,0.f };
	}
	vTargetRight = _vec3{ 1.f,0.f,0.f };
	vTargetLook = _vec3{ 0.f,0.f,1.f };

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

	 if (GetAsyncKeyState(VK_NUMPAD7))
	 {
		 m_fCamRotateY -= D3DXToRadian(45.f) * fDeltaTime;

		// m_pTargetTransform->Rotation(ROT_Y,-D3DXToRadian(45.f)*fDeltaTime);
		 m_pTransformCom->Rotation(ROT_Y, -D3DXToRadian(45.f)*fDeltaTime);

	 }
	 if (GetAsyncKeyState(VK_NUMPAD9))
	 {
		 m_fCamRotateY += D3DXToRadian(45.f) * fDeltaTime;
		// m_pTargetTransform->Rotation(ROT_Y, D3DXToRadian(45.f)*fDeltaTime);
		 m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(45.f)*fDeltaTime);
	 }

	 if (m_bEventView)
	 {
		// _float fAngle = D3DXToDegree(m_fCamRotateY);
		 if (!m_bCross)
		 {
			 if (m_fCamRotateY < D3DXToRadian(m_tCamEvent.fEventAngle))
			 {
				 m_fCamRotateY += D3DXToRadian(45.f) * fDeltaTime*(m_tCamEvent.fEventAngleSpeed);
				 m_pTransformCom->Rotation(ROT_Y, +D3DXToRadian(45.f)*fDeltaTime*m_tCamEvent.fEventAngleSpeed);
			 }
		 }
		 else
		 {
			 if (m_fCamRotateY > D3DXToRadian(m_tCamEvent.fEventAngle))
			 {
				 m_fCamRotateY -= D3DXToRadian(45.f) * fDeltaTime*(m_tCamEvent.fEventAngleSpeed);
				 m_pTransformCom->Rotation(ROT_Y, -D3DXToRadian(45.f)*fDeltaTime*m_tCamEvent.fEventAngleSpeed);
			 }
		 }
		 _float fAngle = D3DXToDegree(m_fCamRotateX);
		 if (m_fCamRotateX > D3DXToRadian(m_tCamEvent.fEventAngleX))
		 {
			 m_fCamRotateX -= D3DXToRadian(45.f) * fDeltaTime*(m_tCamEvent.fEventAngleXSpeed);
			 m_pTransformCom->Rotation(ROT_X, -D3DXToRadian(45.f)*fDeltaTime*m_tCamEvent.fEventAngleXSpeed);
		 }


		 if (m_fDistanceFromTarget > m_tCamEvent.fEventFov)
		 {
			 m_fDistanceFromTarget -= fDeltaTime*(m_tCamEvent.fEventFovSpeed);
		 }

	 }
	 if (m_EventViewEnd)
	 {
		 m_bEventView = false;
		 if (!m_bCross)
		 {
			 if (m_fCamRotateY > m_fPreCamRotateY)
			 {
				 m_fCamRotateY -= D3DXToRadian(45.f) * fDeltaTime*(m_tCamEvent.fEventAngleSpeed);
				 m_pTransformCom->Rotation(ROT_Y, -D3DXToRadian(45.f)*fDeltaTime*(m_tCamEvent.fEventAngleSpeed));
			 }
		 }
		 else
		 {
			 if (m_fCamRotateY < m_fPreCamRotateY)
			 {
				 m_fCamRotateY += D3DXToRadian(45.f) * fDeltaTime*m_tCamEvent.fEventAngleSpeed;
				 m_pTransformCom->Rotation(ROT_Y, +D3DXToRadian(45.f)*(fDeltaTime*m_tCamEvent.fEventAngleSpeed));
			 }
		 }
		 if (m_fCamRotateX <m_fPreCamRotateX)
		 {
			 m_fCamRotateX += D3DXToRadian(45.f) * fDeltaTime*m_tCamEvent.fEventAngleXSpeed;
			 m_pTransformCom->Rotation(ROT_X, D3DXToRadian(45.f)*fDeltaTime*m_tCamEvent.fEventAngleXSpeed);
		 }

		 if (m_fDistanceFromTarget< m_fPreDistanceFromTarget)
		 {
			 m_fDistanceFromTarget += fDeltaTime*(m_tCamEvent.fEventFovSpeed);
		 }
		 if(!m_bCross)
		 {
			 if (m_fDistanceFromTarget >= m_fPreDistanceFromTarget&&
				 m_fCamRotateY <= m_fPreCamRotateY&&
				 m_fCamRotateX >= m_fPreCamRotateX)
			 {
				 m_EventViewEnd = false;
			 }
		 }
		 else
		 {
			 if (m_fDistanceFromTarget >= m_fPreDistanceFromTarget&&
				 m_fCamRotateY >= m_fPreCamRotateY&&
				 m_fCamRotateX >= m_fPreCamRotateX)
			 {
				 m_EventViewEnd = false;
			 }
		 }

	 }

	 for (auto& iter : m_vecCamEvent)
	 {
		 if (iter.bEventEnd)
		 {
			 switch (iter.iEventIdx)
			 {
			 case 43:
				 if (m_fDistanceFromTarget < iter.fEventFov)
				 {
					 m_fDistanceFromTarget += fDeltaTime*iter.fEventFovSpeed;
				 }
			 default:
				 break;
			 }
			 continue;
		 }

		

		 switch (iter.iEventIdx)
		 {
		 case 0:

				// m_fCamRotateY += D3DXToRadian(45.f*iter.fEventAngleSpeed) * fDeltaTime;
				 iter.bEventEnd = true;
			 break;
		 case 4:
			if (!iter.bEventEnd)
			{
				 if (m_fDistanceFromTarget > iter.fEventFov)
				 {
					 m_fDistanceFromTarget -= iter.fEventFovSpeed*fDeltaTime;
				 }
				 if (m_fDistanceFromTarget < 10.f)
				 {
					 if (m_fCamRotateY < D3DXToRadian(450.f))
					 {
						 m_fCamRotateY += D3DXToRadian(45.f) * fDeltaTime;
						 m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(45.f)*fDeltaTime);
					 }
				 }
				 if (EventTimer(fDeltaTime))
				 {
					 ReleaseTarget();
					 m_fDistanceFromTarget = m_fPreDistanceFromTarget;
					 m_fCamRotateY = m_fPreCamRotateY;
					 m_pTransformCom->Set_Rot(0.f, m_fCamRotateY, 0.f);
					 iter.bEventEnd = true;
				 }
			 }
			 break;
		 case 14:
			 m_fDistanceFromTarget = iter.fEventFov;
			 m_fCamRotateY = D3DXToRadian(100.f);
			 m_pTransformCom->Set_Rot(0.f, D3DXToRadian(100.f), 0.f);
			 if (EventTimer(fDeltaTime))
			 {
				 ReleaseTarget();
				 m_fDistanceFromTarget = m_fPreDistanceFromTarget;
				 m_fCamRotateY = m_fPreCamRotateY;
				 m_pTransformCom->Set_Rot(0.f, m_fCamRotateY, 0.f);
				 iter.bEventEnd = true;
			 }
			 break;
		 case 22:
			 //if (m_fCamRotateY < D3DXToRadian(iter.fEventAngle))
			 //{
				// m_fCamRotateY += D3DXToRadian(45.f) * fDeltaTime*iter.fEventAngleSpeed;
				// m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(45.f)*fDeltaTime*iter.fEventAngleSpeed);
			 //}
			 if (m_fDistanceFromTarget < iter.fEventFov)
			 {
				 m_fDistanceFromTarget += fDeltaTime;
			 }
			 else
				 iter.bEventEnd = true;

			 break;
		 case 27:
			 if (m_fCamRotateY < D3DXToRadian(iter.fEventAngle))
			 {
				  m_fCamRotateY += D3DXToRadian(45.f) * fDeltaTime*iter.fEventAngleSpeed;
				  m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(45.f)*fDeltaTime*iter.fEventAngleSpeed);
			 }
			 else
			 {
				 iter.bEventEnd = true;
			 }
			 break;
		 case 31:
			 if (m_fCamRotateY < D3DXToRadian(iter.fEventAngle))
			 {
				 m_fCamRotateY += D3DXToRadian(45.f) * fDeltaTime*iter.fEventAngleSpeed;
				 m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(45.f)*fDeltaTime*iter.fEventAngleSpeed);
			 }
			 else
			 {
				 iter.bEventEnd = true;
			 }
			 break;
		 case 43:
			 if (m_fCamRotateY > D3DXToRadian(iter.fEventAngle))
			 {
				 m_fCamRotateY -= D3DXToRadian(45.f) * fDeltaTime;
				 m_pTransformCom->Rotation(ROT_Y, -D3DXToRadian(45.f)*fDeltaTime);
			 }
			if (EventTimer(fDeltaTime))
			{
				ReleaseTarget();
				//m_fDistanceFromTarget = m_fPreDistanceFromTarget;
				//m_fCamRotateY = m_fPreCamRotateY;
				//m_pTransformCom->Set_Rot(0.f, m_fCamRotateY, 0.f);
				iter.bEventEnd = true;
			}
			
			 break;

		 default:
			 break;
		 }
	 }


	D3DXVec3Normalize(&vTargetLook, &vTargetLook);
	D3DXVec3Normalize(&vTargetRight, &vTargetRight);
	D3DXVec3Normalize(&vTargetUp, &vTargetUp);


	vPlayerCameraDistance = (vTargetLook*1.1f) * -m_fDistanceFromTarget;
	D3DXMatrixRotationAxis(&matRot, &vTargetRight, m_fCamRotateX); // 임의의 축 회전
	D3DXVec3TransformNormal(&vPlayerCameraDistance, &vPlayerCameraDistance, &matRot);

	D3DXMatrixRotationAxis(&matRot, &vTargetUp, m_fCamRotateY); // 임의의 축 
	D3DXVec3TransformNormal(&vPlayerCameraDistance, &vPlayerCameraDistance, &matRot);

	m_tCameraDesc.vEye = vTargetPosition + vPlayerCameraDistance;


	m_tCameraDesc.vAt = vTargetPosition;
	m_pTransformCom->Update_Component(fDeltaTime);
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

void CStaticCamera::CameraShake(_float fPower, _float fShakeTime)
{
	m_fShakePower = fPower;
	m_fShakeTime = fShakeTime;
	m_vInitPos = m_tCameraDesc.vEye;
	m_bShake = true;
}

_float CStaticCamera::GetRandomFloat(_float lowBound, _float highBound)
{
	if (lowBound >= highBound) // bad input
		return lowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}