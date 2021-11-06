#ifndef GameObject_h__
#define GameObject_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject(void);


public:
	_float			Get_ViewZ(void) { return m_fViewZ; }
	CComponent*		Get_Component(const _tchar* pComponentTag, COMPONENTID eID);
	void			Compute_ViewZ(const _vec3* pPos);

public:
	void	SetCol(_bool bCol) { m_bCol = bCol; }
	_bool	GetCol() { return m_bCol; }


public:
	virtual		HRESULT		Ready_Object(void);
	virtual     void		Late_Ready_Object(void);
	virtual		_int		Update_Object(const _float& fTimeDelta);
	virtual		void		Render_Object(void);
public :
	CGameObject* GetTarget() { return m_pTarget; }
	void SetTarget(CGameObject* pTarget) { m_pTarget = pTarget; }


protected:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	map<const _tchar*, CComponent*>		m_mapComponent[ID_END];
	CGameObject* m_pTarget;
	_float								m_fViewZ;

private:
	CComponent*		Find_Component(const _tchar* pComponentTag, COMPONENTID eID);

protected:
	_bool	m_bCol = false;
public:
	virtual void	Free(void);
};

END
#endif // GameObject_h__
