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


	_float m_fAccTime;
	_float m_fUVSpeed;
	_vec3		m_vDir;
	wstring		m_wstrProtoMesh;


	EFFECT::TYPE	m_eInitNextEffect;
	EFFECT::TYPE	m_eNextEffect;
	EFFECT::TYPE	m_eEffect;



public:
	static CEffectMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

	static CEffectMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh);
	virtual void			Free(void);
};

#endif // EffectMesh_h__
