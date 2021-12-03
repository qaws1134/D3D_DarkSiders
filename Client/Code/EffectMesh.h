#ifndef EffectMesh_h__
#define EffectMesh_h__

#include "GameObject.h"
#include "Enum.h"
BEGIN(Engine)

class CTransform;
class CRenderer;
class CCalculator;
class CColliderSphere;
class CStaticMesh;
class CShader;
class CTexture;
END


class CEffectMesh : public CGameObject
{
private:
	explicit CEffectMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffectMesh(const CEffectMesh& rhs);
	virtual ~CEffectMesh(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;
	_float GetRandomFloat(_float lowBound, _float highBound);
	virtual void SetOption(void* pArg);
	void UpdateEffect(const _float& fTimeDelta);
	void SetDir(_vec3 vDir) { m_vDir = vDir; }

public:
	void SetProtoMesh(wstring wstrProtoMesh) { m_wstrProtoMesh = wstrProtoMesh; }
private:
	HRESULT			Add_Component(void);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
private:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CStaticMesh*		m_pMeshCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CTexture*		m_pDissolveCom = nullptr;

	_float m_fAccTime;
	_float m_fUVSpeed;
	_vec3		m_vDir;
	wstring		m_wstrProtoMesh;
	_uint m_ClusteriCount = 0;

	EFFECT::TYPE3D	m_eInitNextEffect;
	EFFECT::TYPE3D	m_eNextEffect;
	EFFECT::TYPE3D	m_eEffect;

	queue<EFFECT::TYPE3D> m_queCluster;

	_float m_fSpawnTime;
	_float m_fSpawnSpeed;

	_float m_fLifeTimer = 0.f;
	_float m_fLifeSpeed= 0.f;
	_bool	m_bLife =false;

	_float m_fDissolveAmount = 0.f;
	_bool m_bDissolve=false;
	_vec4 m_vColor = {1.f,1.f,1.f,1.f};
	_bool m_bCluster =false;

public:
	static CEffectMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

	static CEffectMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh);
	virtual void			Free(void);
};

#endif // EffectMesh_h__
