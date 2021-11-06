#ifndef WaterBoss_h__
#define WaterBoss_h__

#include "GameObject.h"
#include "Enum.h"
BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CColliderSphere;

END


class CWaterBoss : public CGameObject
{
private:
	explicit CWaterBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWaterBoss(const CWaterBoss& rhs);
	virtual ~CWaterBoss(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT			Add_Component(void);
	//void			Key_Input(const _float& fTimeDelta);

private:
private:
	CDynamicMesh*	m_pMeshCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;
	CColliderSphere*		m_pColliderCom = nullptr;
	_vec3		m_vDir;

public:
	static CWaterBoss*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);
};

#endif // WaterBoss_h__
