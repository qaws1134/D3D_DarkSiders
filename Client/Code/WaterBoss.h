#ifndef WaterBoss_h__
#define WaterBoss_h__

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

class CWaterBoss : public CGameObject
{
private:
	explicit CWaterBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWaterBoss(const CWaterBoss& rhs);
	virtual ~CWaterBoss(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual void Late_Ready_Object()override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;
	void AtkColActive(double dStart, double dEnd, _uint iWeaponIdx);

public:
	void		StateChange();
	void		StateActor(_float fDeltaTime);
	void		StateLinker(_float fDeltaTime);		//다음 동작 연결 

	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	_bool		Pattern_Timer(_float fDeltaTime); // 패턴 대기 시간 

	void		SetPattern();
	void		SetSlamPattern();
	virtual void SetOption(void * pArg);

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
	WaterBoss::STATE		m_eMachineState;
	WaterBoss::STATE		m_ePreMachineState;
	WaterBoss::STATE		m_LastPatternState;

	//애니메이션 관리
	WaterBoss::Ani		m_ePreAniState;
	WaterBoss::Ani		m_eCurAniState;


	//애니메이션 블랜드
	_bool	m_bBlend = true;

	_float m_fPatternTimer;
	_float m_fPatternSpeed;


	_uint m_iPatternNum;
	_uint m_iSlamPatternNum;


public:
	static CWaterBoss*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CWaterBoss*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode);
	virtual void			Free(void);
};

#endif // WaterBoss_h__
