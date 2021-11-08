#ifndef UI_h__
#define UI_h__

#include "GameObject.h"

#include "Define.h"
#include "Struct.h"
#include "Enum.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CRenderer;
class CTransform;
class CCalculator;
class CShader;

END

class CUI : public CGameObject
{
private:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI(const CUI& rhs);
	virtual ~CUI(void);

public:
	virtual		HRESULT Ready_Object(void) override;
	virtual		_int Update_Object(const _float& fTimeDelta) override;
	virtual		void Render_Object(void) override;
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	void UI_ElementUpdate(const _float& fTimeDelta);

	void		SetUI(UISET tInfo);
	wstring		&GetObjTag() { return m_tInfo.wstrObjTag; }	//레퍼런스로 넘겨볼까

private:
	HRESULT			Add_Component(void);

private:
	_matrix			m_matProj;
	_matrix			m_matOldProj, m_matOldView;
	UISET			m_tInfo;
	RECT			m_tRcUI;

private:
	CRcTex*		m_pBufferCom = nullptr;
	CTexture*	m_pTextureCom = nullptr;
	CRenderer*	m_pRendererCom = nullptr;
	CTransform*	m_pTransformCom = nullptr;
	CCalculator* m_pCalculatorCom = nullptr;
	CShader*	m_pShaderCom = nullptr;

public:
	static CUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev,UISET tInfo,_bool bActive);
	virtual void			Free(void);
};

#endif // UI_h__
