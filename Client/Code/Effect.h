#ifndef Effect_h__
#define Effect_h__

#include "GameObject.h"
#include "Enum.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CRenderer;
class CTransform;
class CShader;
END

class CEffect : public CGameObject
{
private:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect(const CEffect& rhs);
	virtual ~CEffect(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;
	virtual void SetOption(void* pArg);


	void SetBufferUV(_vec2* vUV);
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
public:
	//저장된 다음 애니메이션 넘겨줌 
	EFFECT::TYPE2D GetNextEffect() { return m_eNextEffect; }

	//진행 중간에 에니메이션이 실행할 때 해당 bool 로 판단ㅇ
	_bool GetNextStart() { return m_bNextStart; }

	void SetNextStart(_bool bStart) { m_bNextStart = bStart; }
	void SetDir(_vec3 vDir) { m_vDir = vDir; m_bMove = true; }

	void FrameChange(_float fDeltaTime);
	void BillBord();
	void UpdateEffect(_float fTimeDelta);


	_uint GetNextEff() { return m_eNextEffect; }
	void EffectEnd();

	void SetVertiQuarterUV();
	void SetCenterQuarterUV();
	void SetHoriQuarterUV();
	void Set66UV();
	void Set44UV();
	void SetProtoUV();

	void SetNextEffect(EFFECT::TYPE2D eEffectSet) { m_eNextEffect = eEffectSet; }
	void SetNextEffect2(EFFECT::TYPE2D eEffectSet) { m_eNextEffect2 = eEffectSet; }

	void SetEffColor(_vec4 vColor) { m_vColor = vColor; }
	void SetLifetime(_float fTimer, _float fSpeed) { m_fLifeTime = fTimer; m_fLifeSpeed = fSpeed; m_bLife = true; }

	_float GetRandomFloat(_float lowBound, _float highBound);

	_bool m_bVertiQuarter = false;
	_bool m_bCenterQuarter= false;
	_bool m_b66 =false;
	_bool m_b44 =false;




private:
	HRESULT			Add_Component(void);

private:
	CRcTex*		m_pBufferCom = nullptr;
	CTexture*	m_pTextureCom = nullptr;
	CRenderer*	m_pRendererCom = nullptr;
	CTransform*	m_pTransformCom = nullptr;
	CShader*	 m_pShaderCom = nullptr;

	EFFECT::TYPE2D	m_eNextEffect = EFFECT::EFFECT_END;
	EFFECT::TYPE2D m_eNextEffect2 = EFFECT::EFFECT_END;
	EFFECT::TYPE2D	m_eEffect;
	EFFECT::SHADER	m_eShader;
	_float			m_fFrame = 0.f;
	_float m_fFrameSpeed;
	
	_float m_fNextStartFrame = 1.f;
	_uint m_iTextIdx;


	_float m_fActionTime;
	_float m_fActionSpeed;
	_bool m_bAction;

	_float m_fLifeTime;
	_float m_fLifeSpeed;
	_bool m_bLife;

	_float m_fBeginTime;
	_float m_fBeginSpeed;
	_bool m_bBegin=false;


	_float m_fMaxSize = 0.f;

	wstring			m_wstrTexture;
	wstring			m_wstrBuffer;
	_bool		m_bNextStart =false;
	

	_bool	m_bCluster =false;
	_bool m_bLoop =false;


	_vec2 m_vMaxUv;
	_uint m_iPassIdx = 0;

	_vec4 m_vColor = {1.f,1.f,1.f,1.f};

	_bool m_bBillbord = true;

	_vec2* m_vUV;
	_vec2	m_vMoveUV;
	_uint m_iUVIdx = 0;

	
	_vec3 m_vDir;
	_bool m_bMove;
	_float m_fMoveSpeed;

	_bool m_bStay;

public:
	static CEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);
};

#endif // Effect_h__
