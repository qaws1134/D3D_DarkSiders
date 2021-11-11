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
	void UI_CoreTreeUpdate(const _float& fTimeDelta);
	void UI_StoneListUpdate(const _float& fTimeDelta);
	void UpdateColRect();


	void		SetFont(UIFONT tFont) { m_tFont = tFont; }
	void		SetUIPos(_vec2 vPos) { m_tInfo.vPos = vPos; }
	void		SetUI(UISET tInfo);
	void		SetObjTag(wstring& wstrObjTag) { m_tInfo.wstrObjTag = wstrObjTag; }

	_vec2		GetUIPos() { return m_tInfo.vPos; }
	_vec2		GetFontPos() { return m_tFont.vPos; }
	wstring	&	GetObjTag() { return m_tInfo.wstrObjTag; }	//레퍼런스로 넘겨볼까
	wstring	&	GetProtoTag() { return m_tInfo.wstrTexture; }
	_uint &		GetTextureNum() { return m_tInfo.iTextureNum; }
	_vec2		GetInitPos() { return m_vInitPos; }
	_uint		GetType() { return m_eType; }
	

	void SetSubTex1(wstring wstrProtoTag,_uint iTextureNum);
	void SetSubTex2(wstring wstrProtoTag,_uint iTextureNum);

	void SetType(UI::TYPE eType) { m_eType = eType; }
	void SetShade(UI::SHADE eShade) { m_eShade = eShade; }
private:
	HRESULT			Add_Component(void);

private:
	_matrix			m_matProj;
	_matrix			m_matOldProj, m_matOldView;
	UISET			m_tInfo;
	RECT			m_tRcUI;
	_bool			m_bSubTex1 = false;	//서브텍스쳐 활성화 유무
	_uint			m_iSubTexNum1;
	_bool			m_bSubTex2 =false;
	_uint			m_iSubTexNum2;
	
	_uint			m_iPassIdx;
	
	_float			fOffset = 0.f;

	_vec2			m_vInitPos;


	_float		m_fScaleSize;
	_bool		m_bScale;

	_uint		m_iPreStoneIdx = 0;
	UI::TYPE m_eType;
	UI::SHADE m_eShade;

	_bool	m_bStart = false;;
	//폰트 정의 
	UIFONT		m_tFont;
private:
	CRcTex*		m_pBufferCom = nullptr;
	CTexture*	m_pTextureCom = nullptr;
	CTexture*	m_pSubTextureCom1 = nullptr;
	CTexture*	m_pSubTextureCom2 = nullptr;

	CRenderer*	m_pRendererCom = nullptr;
	CTransform*	m_pTransformCom = nullptr;
	CCalculator* m_pCalculatorCom = nullptr;
	CShader*	m_pShaderCom = nullptr;

public:
	static CUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev,UISET tInfo,_bool bActive,UI::TYPE eType = UI::TYPE_DEFAULT);
	static CUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, UIFONT tFont, _bool bActive,UI::TYPE eType = UI::TYPE_FONT);
	virtual void			Free(void);
};

#endif // UI_h__
