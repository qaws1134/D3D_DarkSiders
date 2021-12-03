#ifndef ColMesh_h__
#define ColMesh_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CTransform;
class CRenderer;
class CCalculator;
class CColliderSphere;

END

class CColMesh : public CGameObject
{
private:
	explicit CColMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CColMesh(const CColMesh& rhs);
	virtual ~CColMesh(void);

public:
	virtual HRESULT Ready_Object( );
	virtual void Late_Ready_Object()override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

public:
	void SetPos(_vec3 vPos) { m_vPos = vPos; }
	void SetBone(wstring wstrBone) { m_wstrBone = wstrBone; }
	void SetRadius(_float fRadius) { m_fRadius = fRadius; }
	//void SetCol(_bool bCol) { m_bCol = bCol; }
	void SetColSphere(COLLIDERSPHERE tColSphere) {m_fRadius = tColSphere.fRadius; m_vPos = tColSphere.vCenterPos;}
	wstring GetBone() { return m_wstrBone; }
	_float GetRadius() { return m_fRadius; }

	const _matrix* GetParantBoneMatrix() { return m_pParentBoneMatrix; }
	const _matrix* GetParantWorldMatrix(){ return m_pParentWorldMatrix; }

	void SetParantBoneMatrix(_matrix* pParantBone) { m_pParentBoneMatrix = pParantBone; }
	void SetParantWorldMatrix(_matrix* pParantWorld) { m_pParentWorldMatrix = pParantWorld; }
private:
	HRESULT			Add_Component();


private:
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;
	CColliderSphere*m_pColliderCom = nullptr;


private:
	_vec3		m_vPos = {0.f,0.f,0.f};
	_float		m_fRadius = 0.f;
	wstring		m_wstrBone;
//	_bool		m_bCol = false;
	COLLIDERSPHERE	m_tColSphere;

	const _matrix* m_pParentBoneMatrix;	//충돌체가 따라 붙을 Bone 메트릭스
	const _matrix* m_pParentWorldMatrix;	//충돌체가 따라 붙을 부모 transform 메트릭스
	const D3DXFRAME_DERIVED * m_pFrame; //따라붙을 뼈 
	

public:
	static CColMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _float fRadius=1.f , wstring pBone = L"");
	static CColMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev,COLLIDERSPHERE tColSphere, wstring pBone = L"");
	virtual void			Free(void);
};

#endif // Navi_h__
