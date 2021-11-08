//#ifndef UI_WeaponElement_h__
//#define UI_WeaponElement_h__
//
//#include "GameObject.h"
//
//#include "Define.h"
//
//
//
//class CUI_WeaponElement : public CGameObject
//{
//private:
//	explicit CUI_WeaponElement(LPDIRECT3DDEVICE9 pGraphicDev);
//	explicit CUI_WeaponElement(const CUI_WeaponElement& rhs);
//	virtual ~CUI_WeaponElement(void);
//
//public:
//	virtual		HRESULT Ready_Object(void) override;
//	virtual		_int Update_Object(const _float& fTimeDelta) override;
//	virtual		void Render_Object(void) override;
//	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
//
//private:
//	HRESULT			Add_Component(void);
//
//private:
//	_matrix			m_matProj;
//
//	_float			m_fX, m_fY;
//	_float			m_fSizeX, m_fSizeY;
//	_matrix			m_matOldProj, m_matOldView;
//
//private:
//	CRcTex*		m_pBufferCom = nullptr;
//	CTexture*	m_pTextureCom = nullptr;
//	CRenderer*	m_pRendererCom = nullptr;
//	CTransform*	m_pTransformCom = nullptr;
//	CShader*	m_pShaderCom = nullptr;
//
//public:
//	static CUI_WeaponElement*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
//	virtual void			Free(void);
//};
//
//#endif // UI_h__
