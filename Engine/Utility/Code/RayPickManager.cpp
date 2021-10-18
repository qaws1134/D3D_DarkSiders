#include "RayPickManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRayPickManager)
Engine::CRayPickManager::CRayPickManager(void)
{
}

Engine::CRayPickManager::~CRayPickManager(void)
{
	Free();
}

void Engine::CRayPickManager::Set_Device(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
	m_pDevice->AddRef();
}

Engine::RAY Engine::CRayPickManager::Create_MouseRay(const _vec2 & MousePos, const _vec2 & WindowSize)
{
	_vec3 vMouse = _vec3(0.f, 0.f, 0.f);

	vMouse.x = MousePos.x / (WindowSize.x * 0.5f) - 1.f;
	vMouse.y = 1.f - MousePos.y / (WindowSize.y * 0.5f);

	/* �������� ���� */
	// ���� ����� ��
	_matrix matProj;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);

	D3DXVec3TransformCoord(&vMouse, &vMouse, &matProj);

	/* �佺���̽����� ���� */

	// ī�޶� �������� Near������ ������ �����.(�̶� ������ �������� �����Ѵ�.)
	RAY Ray;
	Ray.vRayPos = _vec3(0.f, 0.f, 0.f);
	Ray.vRayDir = vMouse - Ray.vRayPos;

	_matrix matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	D3DXVec3TransformCoord(&Ray.vRayPos, &Ray.vRayPos, &matView);
	D3DXVec3TransformNormal(&Ray.vRayDir, &Ray.vRayDir, &matView);

	/* ������� �������� */

	return Ray;
}



void Engine::CRayPickManager::Free()
{
	Safe_Release(m_pDevice);
}
