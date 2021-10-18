#pragma once
#ifndef __ToolCam_H__

#include "Camera.h"

USING(Engine)
class CToolCam final : public CCamera
{
private:
	explicit CToolCam(LPDIRECT3DDEVICE9 pDevice);
	explicit CToolCam(const CToolCam& rhs);
	virtual ~CToolCam() = default;
public:
	HRESULT Ready_Object(const CAMERA_DESC& tCameraDesc);
	virtual _int Update_Object(const _float& fTimeDelta) override;


public:
	void	Resetting_Camera(const CAMERA_DESC& CamDesc);
	const CAMERA_DESC&	Get_CameraDesc(void);

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Move(void);
	void		Mouse_Fix(void);

private:
	_float		m_fSpeed = 20.f;
	_bool		m_bClick = false;
	_bool		m_bFix = true;


public:
	static CToolCam*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const CAMERA_DESC& tCameraDesc);

	virtual void			Free(void);

};

#define __ToolCam_H__
#endif