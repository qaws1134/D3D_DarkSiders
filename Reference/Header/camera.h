#ifndef Camera_h__
#define Camera_h__

#include "GameObject.h"

BEGIN(Engine)

typedef struct tagCameraDesc
{
	_matrix matView, matProj;

	_vec3 vEye = _vec3(0.f, 0.f, 0.f); // ī�޶� ��ġ
	_vec3 vAt = _vec3(0.f, 0.f, 0.f); // �ٶ� ��ġ
	_vec3 vUp = _vec3(0.f, 1.f, 0.f); // ī�޶� ����� ���� �� �ʿ��� �༮

	_float fFovY = 0.f; // Field Of View
	_float fAspect = 0.f; // ��Ⱦ��
	_float fNear = 0.f; // near ����� Z�� (�佺���̽� ��)
	_float fFar = 0.f; // far ����� Z�� (�佺���̽� ��)
}CAMERA_DESC;

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera(void);

public:
	////����ü �ø�
	//_bool			Make(_matrix* pmatViewProj);
	//_bool			IsIn(_vec3* pPos);
	//_bool			IsInSphere(_vec3* pPos, _float fRadius);

	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;

protected:
	CAMERA_DESC		m_tCameraDesc;
	_matrix			m_matView, m_matProj;


	//_vec3 m_vtx[8];
	//_vec3 m_vPos;
	//D3DXPLANE m_plane[6];

public:
	virtual void Free(void) override;
};

END
#endif // Camera_h__

