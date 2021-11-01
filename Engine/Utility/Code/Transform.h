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
	void				Get_WorldMatrix(_matrix* pMatrix) const;

	void				Set_Scale(const _float& fX, const _float& fY, const _float& fZ);
	void				Set_Scale(const _vec3* vScale);

	void				Set_Pos(const _float& fX, const _float& fY, const _float& fZ);
	void				Set_Pos(const _vec3* pPos);
	void				Set_Rot(const _vec3* vAngle);
	void				Set_Rot(const _float& fX, const _float& fY, const _float& fZ);
	void				Set_CenterPos(const _vec3* vPos);
	
	void				Set_WorldMatrix(const _matrix* pWorld);
	void				Move_Pos(const _vec3* pDir, const _float& fSpeed, const _float& fTimeDelta);
	void				Get_INFO(INFO eType, _vec3* pInfo);
	void				Rotation(ROTATION eType, const _float& fAngle);

	_vec3				Get_CenterPos() { return m_vCenterPos; }
	_vec3				Get_Rot() { return m_vAngle; }
	_vec3				Get_Scale() { return m_vScale; }


	void				Set_ParentMatrix(_matrix* pParent);


public:
	HRESULT				Ready_Transform(void);
	virtual _int		Update_Component(const _float& fTimeDelta);

public:
	_vec3			m_vInfo[INFO_END];
	_vec3			m_vScale;
	_vec3			m_vAngle;
	_matrix			m_matWorld;
	_vec3			m_vCenterPos;
public:
	static CTransform*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	CComponent*		Clone(void);

private:
	virtual void	Free(void);
};


END
#endif // Transform_h__
