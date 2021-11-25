#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Camera.h"
#include "Define.h"

class CDynamicCamera : public CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicCamera(const CDynamicCamera& rhs);
	virtual ~CDynamicCamera(void);

public:
	HRESULT Ready_Object(const CAMERA_DESC& tCameraDesc);
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void SetOption(void *pArg);
private:
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Move(void);
	void		Mouse_Fix(void);

private:
	_float		m_fSpeed = 20.f;
	_bool		m_bClick = false;
	_bool		m_bFix   = true;
	_uint		m_ActioniIdx = 0;

public:
	static CDynamicCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const CAMERA_DESC & tCameraDesc);
	
	virtual void			Free(void);

};

#endif // DynamicCamera_h__
