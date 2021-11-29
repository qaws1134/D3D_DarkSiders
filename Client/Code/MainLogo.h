#ifndef MainLogo_h__
#define MainLogo_h__

#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CRenderer;
class CTransform;
class CShader;

END

class CMainLogo : public CGameObject
{
private:
	explicit CMainLogo(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMainLogo(const CMainLogo& rhs);
	virtual ~CMainLogo(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT			Add_Component(void);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

private:
	CRcTex*		m_pBufferCom = nullptr;
	CTexture*	m_pTextureCom = nullptr;
	CRenderer*	m_pRendererCom = nullptr;
	CTransform*	m_pTransformCom = nullptr;
	CShader*	m_pShaderCom = nullptr;

private:
	_matrix m_matProj;


	_float			m_fX, m_fY;
	_float			m_fSizeX, m_fSizeY;

public:
	static CMainLogo*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);
};

#endif // MainLogo_h__
