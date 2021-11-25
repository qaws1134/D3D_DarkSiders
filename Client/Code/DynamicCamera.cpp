#include "stdafx.h"
#include "DynamicCamera.h"

#include "Export_Function.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{

}

CDynamicCamera::CDynamicCamera(const CDynamicCamera& rhs)
	: CCamera(rhs)
{

}

CDynamicCamera::~CDynamicCamera(void)
{

}

HRESULT CDynamicCamera::Ready_Object(const CAMERA_DESC& tCameraDesc)
{
	m_tCameraDesc = tCameraDesc;

	memcpy(&m_tCameraDesc, &tCameraDesc, sizeof(CAMERA_DESC));

	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);

	return S_OK;
}

Engine::_int CDynamicCamera::Update_Object(const _float& fTimeDelta)
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

void CDynamicCamera::SetOption(void * pArg)
{

	CComponent*		pComponent = nullptr;
	if (pArg)
	{
		memcpy(&m_ActioniIdx, pArg, sizeof(_uint));
	}


	switch (m_ActioniIdx)
	{
	case 4:

		break;
	case 10:
		break;
	case 14:
		break;
	case 31:
		break;
	case 43:
		break;
	default:
		break;
	}


}

void CDynamicCamera::Key_Input(const _float& fTimeDelta)
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

void CDynamicCamera::Mouse_Move(void)
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

void CDynamicCamera::Mouse_Fix(void)
{
	POINT		pt = { WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);

}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const CAMERA_DESC & tCameraDesc)
{
	CDynamicCamera*		pInstance = new CDynamicCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(tCameraDesc)))
		Safe_Release(pInstance);

	return pInstance;
}

void CDynamicCamera::Free(void)
{
	CCamera::Free();
}

