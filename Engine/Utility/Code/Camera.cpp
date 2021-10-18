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

	// ������ı��� ��ġ�� ��� 3���� ������ǥ�� ���� (-1,-1,0) ~ (1,1,1)������ ������ �ٲ��.
	// m_vtx�� �� ���������� ��谪�� �־�д�.
	m_vtx[0].x = -1.0f; m_vtx[0].y = -1.0f; m_vtx[0].z = 0.0f;
	m_vtx[1].x = 1.0f; m_vtx[1].y = -1.0f; m_vtx[1].z = 0.0f;
	m_vtx[2].x = 1.0f; m_vtx[2].y = -1.0f; m_vtx[2].z = 1.0f;
	m_vtx[3].x = -1.0f; m_vtx[3].y = -1.0f; m_vtx[3].z = 1.0f;
	m_vtx[4].x = -1.0f; m_vtx[4].y = 1.0f; m_vtx[4].z = 0.0f;
	m_vtx[5].x = 1.0f; m_vtx[5].y = 1.0f; m_vtx[5].z = 0.0f;
	m_vtx[6].x = 1.0f; m_vtx[6].y = 1.0f; m_vtx[6].z = 1.0f;
	m_vtx[7].x = -1.0f; m_vtx[7].y = 1.0f; m_vtx[7].z = 1.0f;

	// view * proj�� ������� ���Ѵ�.
	D3DXMatrixInverse(&matInv, NULL, pmatViewProj);

	for (i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&m_vtx[i], &m_vtx[i], &matInv);

	// 0���� 5���� ���������� near����� ������ܰ� �����ϴ��̹Ƿ�, ���� ��ǥ�� ���ؼ� 2�� ������
	// ī�޶��� ��ǥ�� ���� �� �ִ�.(��Ȯ�� ��ġ�ϴ� ���� �ƴϴ�.)
	m_vPos = (m_vtx[0] + m_vtx[5]) / 2.0f;

	// ����� ������ǥ�� �������� ����� �����
	// ���Ͱ� �������� ���ʿ��� �ٱ������� ������ �����̴�.
	D3DXPlaneFromPoints(&m_plane[0], m_vtx + 4, m_vtx + 7, m_vtx + 6); // �� ���(top)
	D3DXPlaneFromPoints(&m_plane[1], m_vtx, m_vtx + 1, m_vtx + 2); // �� ���(bottom)
	D3DXPlaneFromPoints(&m_plane[2], m_vtx, m_vtx + 4, m_vtx + 5); // �� ���(near)
	D3DXPlaneFromPoints(&m_plane[3], m_vtx + 2, m_vtx + 6, m_vtx + 7); // �� ���(far)
	D3DXPlaneFromPoints(&m_plane[4], m_vtx, m_vtx + 3, m_vtx + 7); // �� ���(left)
	D3DXPlaneFromPoints(&m_plane[5], m_vtx + 1, m_vtx + 5, m_vtx + 6); // �� ���(right)

	return TRUE;
}

Engine::_bool Engine::CCamera::IsIn(_vec3 * pPos)
{
	float fDist;
	int i;

	for (i = 0; i < 6; i++)
	{
		fDist = D3DXPlaneDotCoord(&m_plane[i], pPos);
		if (fDist > 0) return FALSE; // plane�� normal���Ͱ� far�� ���ϰ� �����Ƿ� ����̸� ���������� �ٱ���
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
		if (fDist > (fRadius + 0)) return FALSE; // ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����
	}

	return TRUE;
}
