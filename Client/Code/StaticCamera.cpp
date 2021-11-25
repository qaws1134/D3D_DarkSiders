#include "stdafx.h"
#include "StaticCamera.h"
#include "GameMgr.h"
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
		tCamEvent.fEventAngle = 110.f;
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

	 for (auto& iter : m_vecCamEvent)
	 {
		 if(iter.bEventEnd )
			 continue;
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
			 else
			 {
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
	D3DXMatrixRotationAxis(&matRot, &vTargetRight, m_fCamAngle); // 임의의 축 회전
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

