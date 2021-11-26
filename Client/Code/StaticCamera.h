#ifndef StaticCamera_h__
#define StaticCamera_h__

#include "Camera.h"
#include "Define.h"
#include "Struct.h"

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

	virtual void SetTarget(CGameObject* pTarget);
	void ReleaseTarget();
	void ReleaseView();	//이전으로 돌림
	void SetEventView();

	_float GetRotY() { return D3DXToDegree(m_fCamRotateY); }
	void Move(_float fDeltaTime);
	virtual void SetOption(void* pArg);
	//void GetCameraINFO(INFO eType, _vec3* pInfo);

	_bool EventTimer(_float fDeltaTime);
private:
	_float		m_fSpeed = 20.f;
	_bool		m_bClick = false;
	_bool		m_bFix   = true;


	CTransform*	m_pTargetTransform = nullptr;
	_float m_fDistanceFromTarget = 15.f;
	_float m_fCamAngle = D3DXToRadian(45.f);
	_float m_fCamRotateY = 0.f;
	_float m_fCamRotateX = D3DXToRadian(45.f);


	_float m_fPreDistanceFromTarget = 15.f;
	_float m_fPreCamAngle = D3DXToRadian(45.f);
	_float m_fPreCamRotateY = 0.f;
	_float m_fPreCamRotateX = D3DXToRadian(45.f);

	//이벤트 조작
	_uint	 m_iEventIdx = 0;
	_float	 m_fEventAngle = 0.f;
	_float	 m_fEventAngleSpeed = 0.f;
	vector<CAMEVENT> m_vecCamEvent;
	_bool	m_bEventView = false;
	_bool	m_EventViewEnd = false;
	CAMEVENT m_tCamEvent;

	CGameObject* m_pPreTarget  = nullptr;
	CTransform* m_pTransformCom = nullptr;

	_matrix m_matWorld;

	_bool m_bCross = false;

	_float m_fEventTime = 0.f;
	_float m_fEventSpeed = 8.f;
	_bool m_bEventTimerEnd = false;

public:
	static CStaticCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const CAMERA_DESC & tCameraDesc);
	
	virtual void			Free(void);

};

#endif // StaticCamera_h__
