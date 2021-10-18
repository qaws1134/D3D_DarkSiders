#include "Camera.h"

USING(Engine)

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CCamera::CCamera(const CCamera& rhs)
	: CGameObject(rhs)
	, m_tCameraDesc(rhs.m_tCameraDesc)
	, m_matView(rhs.m_matView)
	, m_matProj(rhs.m_matProj)
{

}

Engine::CCamera::~CCamera(void)
{

}

HRESULT Engine::CCamera::Ready_Object(void)
{
	D3DXMatrixLookAtLH(
		&m_tCameraDesc.matView,
		&m_tCameraDesc.vEye,
		&m_tCameraDesc.vAt,
		&m_tCameraDesc.vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_tCameraDesc.matView);

	D3DXMatrixPerspectiveFovLH(
		&m_tCameraDesc.matProj,
		m_tCameraDesc.fFovY,
		m_tCameraDesc.fAspect,
		m_tCameraDesc.fNear,
		m_tCameraDesc.fFar);

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_tCameraDesc.matProj);

	return S_OK;
}

Engine::_int Engine::CCamera::Update_Object(const _float& fTimeDelta)
{

	D3DXMatrixLookAtLH(
		&m_tCameraDesc.matView,
		&m_tCameraDesc.vEye,
		&m_tCameraDesc.vAt,
		&m_tCameraDesc.vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_tCameraDesc.matView);

	return 0;
}

void Engine::CCamera::Free(void)
{
	CGameObject::Free();
}


