#ifndef Bullet_h__
#define Bullet_h__

#include "GameObject.h"
#include "Enum.h"
BEGIN(Engine)

class CTransform;
class CRenderer;
class CCalculator;
class CColliderSphere;

END


class CBullet : public CGameObject
{
private:
	explicit CBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBullet(const CBullet& rhs);
	virtual ~CBullet(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	void SetColSphere(COLLIDERSPHERE tColSphere) { m_tColSphere = tColSphere; }

	virtual void SetOption(void* pArg);

	//타이머셋팅, 초기화
	void SetLifeTimer(_float fTimer) { m_fLifeTimer = fTimer; m_fLifeSpeed = 0.f; }
	_bool LifeTimer(_float fDeltaTime);
	
	void SetMove(_bool bMove ,_vec3 vDir) { m_bMove = bMove; m_vDir = vDir; }
	void SetMove(_bool bMove) { m_bMove = bMove; }

private:

	HRESULT			Add_Component(void);

private:
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;
	CColliderSphere*		m_pColliderCom = nullptr;

	_vec3		m_vDir;
	BULLET::TYPE m_eType;
	wstring			m_wstrBulletType =L"";
	_float m_fLifeTimer =0.f;
	_float m_fLifeSpeed = 0.f;
	_bool  m_LifeTime = false;
	
	//이동
	_bool m_bMove =false;
	_float m_fMoveSpeed;

	BULLET::ID m_eID;

public:
	static CBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDevs);
	static CBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev, COLLIDERSPHERE tColShpere);
	virtual void			Free(void);
};

#endif // Bullet_h__
