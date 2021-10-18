#include "Camera.h"

USING(Engine)

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

Engine::CCamera::CCamera(const CCamera& rhs)
	: CGameObject(rhs)
	, m_tCameraDesc(rhs.m_tCameraDesc)
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

	Make(&m_tCameraDesc.matView);
	return 0;
}

void Engine::CCamera::Free(void)
{
	CGameObject::Free();
}


Engine::_bool Engine::CCamera::Make(_matrix * pmatViewProj)
{
	int i;
	D3DXMATRIXA16 matInv;

	// 투영행렬까지 거치면 모든 3차원 월드좌표의 점은 (-1,-1,0) ~ (1,1,1)사이의 값으로 바뀐다.
	// m_vtx에 이 동차공간의 경계값을 넣어둔다.
	m_vtx[0].x = -1.0f; m_vtx[0].y = -1.0f; m_vtx[0].z = 0.0f;
	m_vtx[1].x = 1.0f; m_vtx[1].y = -1.0f; m_vtx[1].z = 0.0f;
	m_vtx[2].x = 1.0f; m_vtx[2].y = -1.0f; m_vtx[2].z = 1.0f;
	m_vtx[3].x = -1.0f; m_vtx[3].y = -1.0f; m_vtx[3].z = 1.0f;
	m_vtx[4].x = -1.0f; m_vtx[4].y = 1.0f; m_vtx[4].z = 0.0f;
	m_vtx[5].x = 1.0f; m_vtx[5].y = 1.0f; m_vtx[5].z = 0.0f;
	m_vtx[6].x = 1.0f; m_vtx[6].y = 1.0f; m_vtx[6].z = 1.0f;
	m_vtx[7].x = -1.0f; m_vtx[7].y = 1.0f; m_vtx[7].z = 1.0f;

	// view * proj의 역행렬을 구한다.
	D3DXMatrixInverse(&matInv, NULL, pmatViewProj);

	for (i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&m_vtx[i], &m_vtx[i], &matInv);

	// 0번과 5번은 프러스텀중 near평면의 좌측상단과 우측하단이므로, 둘의 좌표를 더해서 2로 나누면
	// 카메라의 좌표를 얻을 수 있다.(정확히 일치하는 것은 아니다.)
	m_vPos = (m_vtx[0] + m_vtx[5]) / 2.0f;

	// 얻어진 월드좌표로 프러스텀 평면을 만든다
	// 벡터가 프러스텀 안쪽에서 바깥쪽으로 나가는 평면들이다.
	D3DXPlaneFromPoints(&m_plane[0], m_vtx + 4, m_vtx + 7, m_vtx + 6); // 상 평면(top)
	D3DXPlaneFromPoints(&m_plane[1], m_vtx, m_vtx + 1, m_vtx + 2); // 하 평면(bottom)
	D3DXPlaneFromPoints(&m_plane[2], m_vtx, m_vtx + 4, m_vtx + 5); // 근 평면(near)
	D3DXPlaneFromPoints(&m_plane[3], m_vtx + 2, m_vtx + 6, m_vtx + 7); // 원 평면(far)
	D3DXPlaneFromPoints(&m_plane[4], m_vtx, m_vtx + 3, m_vtx + 7); // 좌 평면(left)
	D3DXPlaneFromPoints(&m_plane[5], m_vtx + 1, m_vtx + 5, m_vtx + 6); // 우 평면(right)

	return TRUE;
}

Engine::_bool Engine::CCamera::IsIn(_vec3 * pPos)
{
	float fDist;
	int i;

	for (i = 0; i < 6; i++)
	{
		fDist = D3DXPlaneDotCoord(&m_plane[i], pPos);
		if (fDist > 0) return FALSE; // plane의 normal벡터가 far로 향하고 있으므로 양수이면 프러스텀의 바깥쪽
	}

	return TRUE;
}

Engine::_bool Engine::CCamera::IsInSphere(_vec3 * pPos, _float fRadius)
{

	float fDist;
	int i;

	for (i = 0; i < 6; i++)
	{
		fDist = D3DXPlaneDotCoord(&m_plane[3], pPos);
		if (fDist > (fRadius + 0)) return FALSE; // 평면과 중심점의 거리가 반지름보다 크면 프러스텀에 없음
	}

	return TRUE;
}
