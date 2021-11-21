#ifndef Grinner_h__
#define Grinner_h__

#include "GameObject.h"
#include "Define.h"
#include "Enum.h"
BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CShader;
//class CColliderSphere;
END

class CGrinner : public CGameObject
{
private:
	explicit CGrinner(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGrinner(const CGrinner& rhs);
	virtual ~CGrinner(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual void Late_Ready_Object()override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

public:
	void		StateChange();
	void		StateActor(_float fDeltaTime);
	void		StateLinker(_float fDeltaTime);		//다음 동작 연결 

	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	_bool		Pattern_Timer(_float fDeltaTime); // 패턴 대기 시간 

	void		SetPattern();
	void		SetSpawnType(_uint eSpawn) { m_eSpawnType = eSpawn; }
	virtual		void TakeDmg(_float fDmg);
#pragma region Set 함수
public:

#pragma  endregion Set 함수

#pragma region Get 함수

#pragma  endregion Set 함수


private:
	HRESULT			Add_Component();

private:
	CDynamicMesh*	m_pMeshCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;
	CShader*		m_pShaderCom = nullptr;

private:

	_vec3 m_vDir;

	//그냥 왼쪽 오른쪽 순서대로 ㄱㄱ

	//머신 상	태
	Grinner::STATE		m_eMachineState;
	Grinner::STATE		m_ePreMachineState;

	//애니메이션 관리
	Grinner::Ani		m_ePreAniState;
	Grinner::Ani		m_eCurAniState;


	//애니메이션 블랜드
	_bool	m_bBlend = true;
	_double m_bBlendTime;

	_float m_fPatternTimer;
	_float m_fPatternSpeed;

	_uint m_iPatternNum;

	_uint m_eSpawnType;

	CTransform*	m_pTargetTransform = nullptr;

public:
	static CGrinner*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CGrinner*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode);
	virtual void			Free(void);
};

#endif // Grinner_h__
