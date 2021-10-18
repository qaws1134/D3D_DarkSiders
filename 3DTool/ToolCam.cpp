#include "stdafx.h"
#include "ToolCam.h"
#include "Define.h"
#include "Export_Function.h"

CToolCam::CToolCam(LPDIRECT3DDEVICE9 pDevice)
	: CCamera(pDevice)
{
	
}

CToolCam::CToolCam(const CToolCam & rhs)
	: CCamera(rhs)
{
}

HRESULT CToolCam::Ready_Object(const CAMERA_DESC& tCameraDesc)
{
	m_tCameraDesc = tCameraDesc;

	memcpy(&m_tCameraDesc, &tCameraDesc, sizeof(CAMERA_DESC));
	CCamera::Ready_Object();
	return S_OK;
}

_int CToolCam::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CCamera::Update_Object(fTimeDelta);


	Key_Input(fTimeDelta);

	if (true == m_bFix)
	{
		Mouse_Fix();
		Mouse_Move();
	}


	return iExit;
}

void CToolCam::Resetting_Camera(const CAMERA_DESC & CamDesc)
{
	memcpy(&m_tCameraDesc, &CamDesc, sizeof(CAMERA_DESC));
}

const CAMERA_DESC & CToolCam::Get_CameraDesc(void)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_tCameraDesc;
}


void CToolCam::Key_Input(const _float & fTimeDelta)
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_tCameraDesc.matView);

	if (Get_DIKeyState(DIK_W) & 0x80)
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));
		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeed * fTimeDelta;
		m_tCameraDesc.vEye += vLength;
		m_tCameraDesc.vAt += vLength;
	}

	if (Get_DIKeyState(DIK_S) & 0x80)
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));
		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeed * fTimeDelta;
		m_tCameraDesc.vEye -= vLength;
		m_tCameraDesc.vAt -= vLength;
	}

	if (Get_DIKeyState(DIK_D) & 0x80)
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));
		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeed * fTimeDelta;
		m_tCameraDesc.vEye += vLength;
		m_tCameraDesc.vAt += vLength;
	}

	if (Get_DIKeyState(DIK_A) & 0x80)
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));
		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeed * fTimeDelta;
		m_tCameraDesc.vEye -= vLength;
		m_tCameraDesc.vAt -= vLength;
	}
	if (Get_DIKeyState(DIK_Q) & 0x80)
	{
		if (true == m_bClick)
			return;

		m_bClick = true;
		if (true == m_bFix)
			m_bFix = false;
		else
			m_bFix = true;
	}
	else
		m_bClick = false;

	if (false == m_bFix)
		return;
}

void CToolCam::Mouse_Move(void)
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_tCameraDesc.matView);

	_long	dwMouse = 0;

	if (dwMouse = Get_DIMouseMove(DIMS_Y))
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLook = m_tCameraDesc.vAt - m_tCameraDesc.vEye;
		_matrix		matRot;
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouse / 10.f));

		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_tCameraDesc.vAt = m_tCameraDesc.vEye + vLook;
	}

	if (dwMouse = Get_DIMouseMove(DIMS_X))
	{
		_vec3	vUp = _vec3(0.f, 1.f, 0.f);

		_vec3	vLook = m_tCameraDesc.vAt - m_tCameraDesc.vEye;

		_matrix		matRot;
		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouse / 10.f));

		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_tCameraDesc.vAt = m_tCameraDesc.vEye + vLook;
	}

}

void CToolCam::Mouse_Fix(void)
{
	POINT		pt = { WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &pt);
	//SetCursorPos(pt.x, pt.y);
}
CToolCam * CToolCam::Create(LPDIRECT3DDEVICE9 pGraphicDev, const CAMERA_DESC & tCameraDesc)
{
	CToolCam*		pInstance = new CToolCam(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(tCameraDesc)))
		Safe_Release(pInstance);

	return pInstance;
}

void CToolCam::Free(void)
{
	CCamera::Free();
}
//
//void CToolCam::Resetting_Camera(const CAMERA_DESC & CamDesc)
//{
//	memcpy(&m_CameraDesc, &CamDesc, sizeof(CAMERA_DESC));
//}
//
//const CAMERA_DESC & CToolCam::Get_CameraDesc(void)
//{
//	// TODO: 여기에 반환 구문을 삽입합니다.
//	return m_CameraDesc;
//}
//
//
//HRESULT CToolCam::ReadyPrototype()
//{
//	CCamera::ReadyPrototype();
//
//	return S_OK;
//}
//
//HRESULT CToolCam::ReadyClone(void * pArg/* = nullptr*/, void* pArg2 /*= nullptr*/)
//{
//	CCamera::ReadyClone(pArg, pArg2);
//
//
//	D3DVIEWPORT9		tViewport;
//	ZeroMemory(&tViewport, sizeof(D3DVIEWPORT9));
//
//	tViewport.X = 0;
//	tViewport.Y = 0;
//	tViewport.Width = g_iWINCX;
//	tViewport.Height = g_iWINCY;
//	tViewport.MinZ = 0;
//	tViewport.MaxZ = 1;
//
//
//	CCamera::Set_Viewport(tViewport);
//
//
//
//	return S_OK;
//}
//
//_uint CToolCam::Update(_float fDeltaTime)
//{
//	CCamera::Update(fDeltaTime);
//
//	return NO_EVENT;
//}
//
//_uint CToolCam::LateUpdate(_float fDeltaTime)
//{	
//	Movement(fDeltaTime);
//
//	return CCamera::LateUpdate(fDeltaTime);
//}
//
//_uint CToolCam::Render(_float fDeltaTime)
//{
//	CCamera::Render(fDeltaTime);
//
//	return NO_EVENT;
//}
//
//_uint CToolCam::Movement(_float fDeltaTime)
//{
//	_float4x4	matView, matRot;
//
//
//	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
//	D3DXMatrixInverse(&matView, 0, &matView);
//
//	// At 바라보는 벡터 만들고
//	_float3 vDir = m_CameraDesc.vAt - m_CameraDesc.vEye;
//	D3DXVec3Normalize(&vDir, &vDir);
//
//	if (GetAsyncKeyState('W') & 0x8000)
//	{
//		_float3 vRight, vUp, vLook;
//		memcpy(&vRight, &matView.m[0][0], sizeof(_float3));
//
//		D3DXVec3Normalize(&vRight, &vRight);
//
//		vUp = _float3(0.f, 1.f, 0.f);
//
//		D3DXVec3Cross(&vLook, &vRight, &vUp);
//
//		D3DXVec3Normalize(&vLook, &vLook);
//
//
//		m_CameraDesc.vEye += vLook * 50.f * fDeltaTime;
// 		m_CameraDesc.vAt += vLook  * 50.f * fDeltaTime;
//	}
//
//
//	if (GetAsyncKeyState('S') & 0x8000)
//	{
//		_float3 vRight, vUp, vLook;
//		memcpy(&vRight, &matView.m[0][0], sizeof(_float3));
//
//		D3DXVec3Normalize(&vRight, &vRight);
//
//		vUp = _float3(0.f, 1.f, 0.f);
//
//		D3DXVec3Cross(&vLook, &vRight, &vUp);
//
//		D3DXVec3Normalize(&vLook, &vLook);
//
//
//		m_CameraDesc.vEye += vLook * -50.f * fDeltaTime;
//		m_CameraDesc.vAt += vLook * -50.f * fDeltaTime;
//	}
//
//
//	if (GetAsyncKeyState('D') & 0x8000)
//	{
//		_float3 vRight;
//
//		memcpy(&vRight, &matView.m[0][0], sizeof(_float3));
//
//		D3DXVec3Normalize(&vRight, &vRight);
//
//
//
//
//		m_CameraDesc.vEye += vRight * 50.f * fDeltaTime;
//		m_CameraDesc.vAt += vRight * 50.f * fDeltaTime;
//	}
//
//
//	if (GetAsyncKeyState('A') & 0x8000)
//	{
//		_float3 vRight;
//
//		memcpy(&vRight, &matView.m[0][0], sizeof(_float3));
//
//		D3DXVec3Normalize(&vRight, &vRight);
//
//
//		m_CameraDesc.vEye -= vRight * 50.f * fDeltaTime;
//		m_CameraDesc.vAt -= vRight * 50.f * fDeltaTime;
//
//	}
//
//	if (GetAsyncKeyState(VK_ADD) & 0x8000)
//	{
//		if (m_fDistanceFromTarget > 3.f)
//		{
//			m_fDistanceFromTarget -= 50.f * fDeltaTime;
//
//			//_float3 vDir = m_CameraDesc.vAt - m_CameraDesc.vEye;
//			//D3DXVec3Normalize(&vDir, &vDir);
//			//m_CameraDesc.vEye = vDir * -m_fDistanceFromTarget;
//
//		}
//	}
//
//
//	if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000)
//	{
//		m_fDistanceFromTarget += 50.f * fDeltaTime;
//
//		//_float3 vDir = m_CameraDesc.vAt - m_CameraDesc.vEye;
//		//D3DXVec3Normalize(&vDir, &vDir);
//		//m_CameraDesc.vEye = vDir * -m_fDistanceFromTarget;
//	}
//
//	/*if (GetAsyncKeyState(VK_NUMPAD8))
//	{
//		m_fCamAngle += D3DXToRadian(45.f) * fDeltaTime;
//	}
//
//	if (GetAsyncKeyState(VK_NUMPAD2))
//	{
//		m_fCamAngle -= D3DXToRadian(45.f) * fDeltaTime;
//	}*/
//
//	/* y축 기준 좌우 회전 */
//	if (GetAsyncKeyState('Q') & 0x8000)
//	{
//		m_fCamAngle = D3DXToRadian(m_fCamRotationSpeed) * fDeltaTime;
//		D3DXMatrixRotationY(&matRot, m_fCamAngle);
//		D3DXVec3TransformNormal(&vDir, &vDir, &matRot);
//
//	}
//	if (GetAsyncKeyState('E') & 0x8000)
//	{
//		m_fCamAngle = D3DXToRadian(m_fCamRotationSpeed) * -fDeltaTime;
//		D3DXMatrixRotationY(&matRot, m_fCamAngle);
//		D3DXVec3TransformNormal(&vDir, &vDir, &matRot);
//
//	}
//	if (GetAsyncKeyState('Z') & 0x8000)
//	{
//		
//		m_fCamAngle = m_fCamRotationSpeed * -fDeltaTime;
//
//		D3DXMatrixRotationX(&matRot, D3DXToRadian(m_fCamAngle));
//		D3DXVec3TransformNormal(&vDir, &vDir, &matRot);
//	}
//	if (GetAsyncKeyState('C') & 0x8000)
//	{
//		m_fCamAngle = m_fCamRotationSpeed * fDeltaTime;
//
//		D3DXMatrixRotationX(&matRot, D3DXToRadian(m_fCamAngle));
//		D3DXVec3TransformNormal(&vDir, &vDir, &matRot);
//
//	}
//
//	// 역벡터로 만들고 거리 띄운다.
//	vDir *= -m_fDistanceFromTarget;
//	
//
//	m_CameraDesc.vEye = vDir + m_CameraDesc.vAt;
//	
//
//
//
//
//
//	return NO_EVENT;
//}
//
//CToolCam * CToolCam::Create(LPDIRECT3DDEVICE9 pDevice)
//{
//	CToolCam* pInstance = new CToolCam(pDevice);
//	if (FAILED(pInstance->ReadyPrototype()))
//	{
//		ERR_MSG(L"Error", L"Failed To Create  ToolCam Prototype");
//		SafeRelease(pInstance);
//	}
//
//	return pInstance;
//}
//
//IPrototype * CToolCam::Clone(void * pArg/* = nullptr*/, void* pArg2 /*= nullptr*/)
//{
//	CToolCam* pInstance = new CToolCam(*this); // 복사생성자 호출
//	if (FAILED(pInstance->ReadyClone(pArg, pArg2)))
//	{
//		ERR_MSG(L"Error", L"Failed To Clone  ToolCam Prototype");
//		SafeRelease(pInstance);
//	}
//
//	return pInstance;
//}
//
//CToolCam * CToolCam::Create(LPDIRECT3DDEVICE9 pGraphicDev, const CAMERA_DESC & m_tCameraDesc)
//{
//	return nullptr;
//}
//
//void CToolCam::Free()
//{
//	SafeRelease(m_pDevice);
//	CGameObject::Free();
//}
//
//void CToolCam::Render_Object(void)
//{
//}
