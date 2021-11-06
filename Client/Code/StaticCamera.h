#ifndef StaticCamera_h__
#define StaticCamera_h__

#include "Camera.h"
#include "Define.h"

BEGIN(Engine)
class CTransform;
END




class CStaticCamera : public CCamera
{
private:
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaticCamera(const CStaticCamera& rhs);
	virtual ~CStaticCamera(void);

public:
	HRESULT Ready_Object(const CAMERA_DESC& tCameraDesc);
	virtual _int Update_Object(const _float& fTimeDelta) override;

	void ChasingTarget(CTransform* pTargetTransform);
	void ReleaseTarget();

	void Move(_float fDeltaTime);


private:
	_float		m_fSpeed = 20.f;
	_bool		m_bClick = false;
	_bool		m_bFix   = true;

	CTransform*	m_pTargetTransform = nullptr;
	_float m_fDistanceFromTarget = 15.f;
	_float m_fCamAngle = D3DXToRadian(45.f);
	_float m_fCamRotateY = D3DXToRadian(45.f);
	_float m_fCamRotateX = D3DXToRadian(45.f);

	CGameObject* m_pPreTarget  = nullptr;
public:
	static CStaticCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const CAMERA_DESC & tCameraDesc);
	
	virtual void			Free(void);

};

#endif // StaticCamera_h__
