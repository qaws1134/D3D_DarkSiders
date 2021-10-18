#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform(void);

public:
	 _matrix*			Get_WorldMatrix(void)  { return &m_matWorld; }
	void				Get_WorldMatrix(_matrix* pMatrix) ;
	void				Set_Scale( _float fX,  _float fY,  _float fZ);
	void				Set_Scale( _vec3* vScale);

	void				Set_Pos( _float& fX,  _float& fY,  _float& fZ);
	void				Set_Pos( _vec3* pPos);
	void				Set_Rot( _vec3* vAngle);
	void				Set_Rot( _float& fX,  _float& fY,  _float& fZ);
	void				Set_WorldMatrix( _matrix* pWorld);
	void				Move_Pos( _vec3* pDir,  _float& fSpeed,  _float& fTimeDelta);
	void				Get_INFO(INFO eType, _vec3* pInfo);
	void				Rotation(ROTATION eType,  _float& fAngle);

	_vec3				Get_Rot() { return m_vAngle; }
	_vec3				Get_Scale() { return m_vScale; }

public:
	HRESULT				Ready_Transform(void);
	virtual _int		Update_Component(const _float& fTimeDelta);

public:
	_vec3			m_vInfo[INFO_END];
	_vec3			m_vScale;
	_vec3			m_vAngle;
	_matrix			m_matWorld;

public:
	static CTransform*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	CComponent*		Clone(void);

private:
	virtual void	Free(void);
};


END
#endif // Transform_h__
