#ifndef Effect_Trail_h__
#define Effect_Trail_h__

#include "GameObject.h"
#include "Enum.h"
#include "Struct.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CRenderer;
class CTransform;
class CShader;
class CTrail_Texture;
END

class CEffect_Trail : public CGameObject
{
public:
	explicit	 CEffect_Trail(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit	 CEffect_Trail(const CEffect_Trail& rhs);
	virtual		~CEffect_Trail(void);



public:		// Setter
	void Set_MatrixInfo(const D3DXMATRIX* pPlayerMat, const D3DXMATRIX* pPlayerWeaponMat)
	{
		m_pmatPlayerInfo = pPlayerMat;
		m_pmatWeaponRef = pPlayerWeaponMat;
	}

public:

	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;


	virtual HRESULT Add_Component(void);
	virtual void SetOption(void* pArg);
private:		
	void Memorize_Point(const float & fTimeDelta);

private:
	CRenderer*				m_pRendererCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CTrail_Texture*			m_pBufferCom = nullptr;

private:
	const _matrix*		m_pmatPlayerInfo = nullptr;
	const _matrix*		m_pmatWeaponRef = nullptr;

private:
	list<_vec3>				m_Pointlist;
	typedef list<_vec3>	POINTLIST;

private:		// 트레일 속성
	//_int			m_iTrailCount;
	//_float			m_fTrailEmitRate;
	//_float			m_fTrailSize;
	//_vec4			m_vColor;

	TRAIL m_tTrailInfo;


private:
	_float			m_fAccTime;
	_float			m_fTimeDelta;
private:
	HRESULT Set_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CEffect_Trail* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);

};

#endif // Effect_Trail_h__