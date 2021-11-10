#ifndef Stone_h__
#define Stone_h__

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

class CStone : public CGameObject
{
private:
	explicit CStone(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStone(const CStone& rhs);
	virtual ~CStone(void);

public:
	virtual		HRESULT Ready_Object(void) override;
	virtual		_int Update_Object(const _float& fTimeDelta) override;
	virtual		void Render_Object(void) override;
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);



private:
	HRESULT			Add_Component(void);

public:
	void			Set_CreatureInfo(_uint iCreatureNum);

private:
	_matrix			m_matProj;
	_matrix			m_matOldProj, m_matOldView;
	UISET			m_tInfo;
	RECT			m_tRcStone;

	_uint			m_iPassIdx;
	//이름 속성 정보 크리쳐 레어도 
	//랜더 -> 텍스쳐-> 보석 랜더  
	//리스트에 크리쳐는 그냥 새로 출력 ㄱㄱ 

	_float		m_fScaleSize;
	_bool		m_bScale;
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
	static CStone*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CStone*		Create(LPDIRECT3DDEVICE9 pGraphicDev,_uint iCreature);
	virtual void			Free(void);
};

#endif // Stone_h__
