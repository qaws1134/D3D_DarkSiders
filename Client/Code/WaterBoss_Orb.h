#ifndef WaterBoss_Orb_h__
#define WaterBoss_Orb_h__

#include "GameObject.h"
#include "Enum.h"
BEGIN(Engine)

class CTransform;
class CRenderer;
class CCalculator;
class CColliderSphere;
class CStaticMesh;
class CShader;
END


class CWaterBoss_Orb : public CGameObject
{
private:
	explicit CWaterBoss_Orb(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWaterBoss_Orb(const CWaterBoss_Orb& rhs);
	virtual ~CWaterBoss_Orb(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT			Add_Component(void);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
private:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CColliderSphere*	m_pColliderCom = nullptr;
	CStaticMesh*		m_pMeshCom = nullptr;
	CShader*			m_pShaderCom = nullptr;


	_vec3		m_vDir;

public:
	static CWaterBoss_Orb*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);
};

#endif // WaterBoss_Orb_h__
