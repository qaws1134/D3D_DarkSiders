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



	_float GetColShpereRadius() { return m_tColSphere.fRadius; }
	_vec3* GetPos(COMPONENTID eID);

	void SetPos(_vec3 vPos, COMPONENTID eID);
	void SetZPos(_float fzPos, COMPONENTID eID);
	_float GetWorldZ(COMPONENTID eID);
	void	SetColTarget(CGameObject* pColTarget) { m_pColTarget = pColTarget; }


public:
	void	SetCol(_bool bCol) { m_bCol = bCol; }
	_bool	GetCol() { return m_bCol; }

	_bool	HitTimer(_float fDeltaTime);
public:
	virtual	HRESULT		Ready_Object(void);
	virtual void		Late_Ready_Object(void);
	virtual	_int		Update_Object(const _float& fTimeDelta);
	virtual	void		Render_Object(void);
public :
	CGameObject* GetTarget() { return m_pTarget; }
	_bool GetActive() { return m_bActive; }
	wstring& GetObjTag() { return m_wstrObjTag; }
	map<const _tchar*, CGameObject*>& GetColmap() { return m_mapColider; }
	COLLIDERSPHERE m_tColSphere;

	void SetObjTag(wstring& wstrObjTag) { m_wstrObjTag = wstrObjTag; }
	void SetTarget(CGameObject* pTarget) { m_pTarget = pTarget; }
	void SetActive(_bool bActive) { m_bActive = bActive; }

	void Set_Component(const _tchar* pComTag, CComponent* pComponent,COMPONENTID eID);


	//체력, 데미지
	virtual void TakeDmg(_float fDmg) { m_tCharInfo.fDmg = fDmg; }
	void ResetDmg() { m_tCharInfo.fDmg = 0.f; }
	void SetHP(_float fHp) { m_tCharInfo.fHp = fHp; }
	void SetAtk(_float fAtk) { m_tCharInfo.fAtk = fAtk; }


	_float GetAtk() { return m_tCharInfo.fAtk; }
	_float GetHp() { return m_tCharInfo.fHp; }
	_float GetDmg() { return m_tCharInfo.fDmg; }
	_bool  GetHit() { return m_bHit; }
	

	//충돌체
	void EmplaceCol(wstring ObjTag, CGameObject* pGameObject);
	void SetCharInfo(_float fHp, _float fAtk);

	virtual void SetOption(void* arg = nullptr) {}
	void SetHitTime(_float fHitTime) { m_fHitTime = fHitTime; }
	void SetHitSpeed(_float fHitSpeed){ m_fHitSpeed = fHitSpeed; }

	void	SetDead(_bool bDead) { m_bDead = bDead; }
	_bool	GetDead() { return m_bDead; }
protected:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	map<const _tchar*, CComponent*>		m_mapComponent[ID_END];
	CGameObject* m_pTarget= nullptr;
	CGameObject* m_pColTarget = nullptr;
	_float								m_fViewZ;

	wstring m_wstrObjTag = L""; 

	_bool					m_bActive;
private:
	CComponent*		Find_Component(const _tchar* pComponentTag, COMPONENTID eID);

protected:
	_bool	m_bCol = false;
	map<const _tchar*, CGameObject*>m_mapColider;

	_float m_fHitTime;
	_float m_fHitSpeed;
	_bool	m_bHit = false;
	
	_bool	m_bDead = false;
	
	CHARINFO m_tCharInfo;
	_vec3 m_vPos;

public:
	virtual void	Free(void);
};

END
#endif // GameObject_h__
