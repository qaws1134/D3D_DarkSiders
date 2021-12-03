#ifndef Item_h__
#define Item_h__

#include "GameObject.h"
#include "Enum.h"
#include "Struct.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CRenderer;
class CTransform;
class CShader;
class CNaviMesh;
class CCalculator;
class CColliderSphere;
END

class CItem : public CGameObject
{
private:
	explicit CItem(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CItem(const CItem& rhs);
	virtual ~CItem(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;
	virtual void SetOption(void* pArg);
	void SetNavi(CNaviMesh* pNavi) { m_pNavi = pNavi; }

	HRESULT SetUp_ConstantTable(LPD3DXEFFECT& pItem);

public:
	void BillBord();
	_float GetRandomFloat(_float lowBound,_float highBound);
	wstring m_wstrTexture;

	

	void LifeTime(_float fTimeDelta);
private:
	HRESULT			Add_Component(void);

private:
	CRcTex*			m_pBufferCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CNaviMesh*		m_pNavi = nullptr;
	CColliderSphere* m_pColliderCom = nullptr;


	_uint m_iTextureNum;

	STONE m_tStone;
	DROPITEMINFO m_tItem;
	_float m_fAngle;
	_float m_fSpeed;

	_uint m_iPassIdx = 0;
	_vec3 m_vDir;
	_bool m_bMoveEnd = false;
	_float m_fMoveSpeed = 1.f;
	_float m_fChaseSpeed = 0.f;
	_bool m_bApexEnd = false;
	_float m_fNaviY = 0.f;


	_float m_fLifeTime = 7.f;
	_float m_fLifeSpeed = 7.f;



public:
	static CItem*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);
};

#endif // Item_h__
