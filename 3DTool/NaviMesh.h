#ifndef Navimesh_h__
#define Navimesh_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CTransform;
class CRenderer;
class CCalculator;
class CColliderSphere;

END

class CNaviMesh : public CGameObject
{
private:
	explicit CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNaviMesh(const CNaviMesh& rhs);
	virtual ~CNaviMesh(void);

public:
	virtual HRESULT Ready_Object( );
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

public:
	void SetPos(_vec3 vPos) { m_vPos = vPos; }
private:
	HRESULT			Add_Component();


private:
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;
	CColliderSphere*m_pColliderCom = nullptr;

	_vec3		m_vPos;
	_bool		m_bColl = false;
public:
	static CNaviMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual void			Free(void);
};

#endif // Navi_h__
