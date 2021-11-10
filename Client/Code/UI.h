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

	void		SetUIPos(_vec2 vPos) { m_tInfo.vPos = vPos; }
	void		SetUI(UISET tInfo);
	wstring		&GetObjTag() { return m_tInfo.wstrObjTag; }	//���۷����� �Ѱܺ���

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
	_bool			m_bSubTex1 = false;	//�����ؽ��� Ȱ��ȭ ����
	_uint			m_iSubTexNum1;
	_bool			m_bSubTex2 =false;
	_uint			m_iSubTexNum2;
	
	_uint			m_iPassIdx;
	


	_float		m_fScaleSize;
	_bool		m_bScale;


	UI::TYPE m_eType;
	UI::SHADE m_eShade;
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
	virtual void			Free(void);
};

#endif // UI_h__
