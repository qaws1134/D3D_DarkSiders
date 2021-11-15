#ifndef GameObject_h__
#define GameObject_h__

#include "Component.h"
#include "Transform.h"

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

	void SetPos(_vec3 vPos, COMPONENTID eID);
	void SetZPos(_float fzPos, COMPONENTID eID);
	_float GetWorldZ(COMPONENTID eID);

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
	_bool GetActive() { return m_bActive; }
	wstring& GetObjTag() { return m_wstrObjTag; }

	void SetObjTag(wstring& wstrObjTag) { m_wstrObjTag = wstrObjTag; }
	void SetTarget(CGameObject* pTarget) { m_pTarget = pTarget; }
	void SetActive(_bool bActive) { m_bActive = bActive; }
	void EmplaceCol(wstring ObjTag, CGameObject* pGameObject) { m_mapColider.emplace(ObjTag, pGameObject); }
protected:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	map<const _tchar*, CComponent*>		m_mapComponent[ID_END];
	CGameObject* m_pTarget;
	_float								m_fViewZ;

	wstring m_wstrObjTag = L""; 

	_bool					m_bActive;
private:
	CComponent*		Find_Component(const _tchar* pComponentTag, COMPONENTID eID);

protected:
	_bool	m_bCol = false;
	map<wstring, CGameObject*>m_mapColider;

public:
	virtual void	Free(void);
};

END
#endif // GameObject_h__
