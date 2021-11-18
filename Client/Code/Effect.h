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
	EFFECT::TYPE GetNextEffect() { return m_eNextEffect; }

	//진행 중간에 에니메이션이 실행할 때 해당 bool 로 판단ㅇ
	_bool GetNextStart() { return m_bNextStart; }

	void SetNextStart(_bool bStart) { m_bNextStart = bStart; }
	void ResetNextEffect() { m_eNextEffect = m_eInitNextEffect; }
	void FrameChange(_float fDeltaTime);
	void BillBord();
	void UpdateEffect(_float fTimeDelta);



	void SetVertiQuarterUV();
	void SetCenterQuarterUV();
	void SetHoriQuarterUV();
	void Set66UV();
	void Set44UV();

private:
	HRESULT			Add_Component(void);

private:
	CRcTex*		m_pBufferCom = nullptr;
	CTexture*	m_pTextureCom = nullptr;
	CRenderer*	m_pRendererCom = nullptr;
	CTransform*	m_pTransformCom = nullptr;
	CShader*	 m_pShaderCom = nullptr;

	EFFECT::TYPE	m_eInitNextEffect;
	EFFECT::TYPE	m_eNextEffect;
	EFFECT::TYPE	m_eEffect;
	_float			m_fFrame = 0.f;
	_float m_fFrameSpeed;
	
	_float m_fNextStartFrame = 1.f;



	wstring			m_wstrTexture;

	_bool		m_bNextStart =false;
	

	_bool m_bLoop =false;



	_uint m_iPassIdx = 0;

	_vec2* m_vUV;
	_vec2	m_vMoveUV;
	_uint m_iUVIdx = 0;


public:
	static CEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);
};

#endif // Effect_h__
