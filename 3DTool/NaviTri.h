#ifndef NaviTri_h__
#define NaviTri_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRenderer;
class CCalculator;
class CTriCol;
class CTransform;

END

class CNaviTri : public CGameObject
{
private:
	explicit CNaviTri(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNaviTri(const CNaviTri& rhs);
	virtual ~CNaviTri(void);

public:
	virtual HRESULT Ready_Object(_vec3 *pAryPos);
	virtual void Late_Ready_Object()override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	void Update_Tri(_vec3*pAryPos);
private:
	HRESULT			Add_Component();
	//void			Key_Input(const _float& fTimeDelta);
	//void			SetUp_OnTerrain(void);
	//_vec3			PickUp_OnTerrain(void);

private:
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;
	CTriCol*		m_pTriColCom= nullptr;

	_vec3			m_pTriPos[3];


public:
	static CNaviTri*		Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 *pAryPos);

	virtual void			Free(void);
};

#endif // Stone_h__
