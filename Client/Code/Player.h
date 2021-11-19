#ifndef Player_h__
#define Player_h__

#include "GameObject.h"
#include "Define.h"
#include "Enum.h"
BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CShader;
class CNaviMesh;
//class CColliderSphere;
END

class CPlayer : public CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual void Late_Ready_Object()override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

public:
	void		StateChange();
	void		StateActer(_float fDeltaTime);
	void		StateLinker(_float fDeltaTime);		//다음 동작 연결 
	void		DirSet(War::DIR eDir, _float fTimeDelta, _float fAngleSpeed);	//키에 따른 방향 결정
	void		DirSet_Combo();

	void		ElementAniSet();
	_bool		Combat_to_Idle_Timer(_float fDeltaTime);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);


#pragma region Set 함수
public:
	void		Set_Element(War::WEAPON_ELEMENT eElement) { m_eElement = eElement; }
	void		Set_PlayerState(War::STATE eState) { m_eMachineState = eState; }
	void		Take_Dmg(_float fDmg) { m_fDmg = fDmg;  }
	void		Set_NaviMesh(CNaviMesh* pNavi) { m_pNavi = pNavi; }
#pragma  endregion Set 함수
#pragma region Get 함수

#pragma  endregion Set 함수


private:
	HRESULT			Add_Component();
	void			Key_Input(const _float& fTimeDelta);

private:
	CDynamicMesh*	m_pMeshCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CNaviMesh*		m_pNavi = nullptr;

private:

	_vec3 m_vDir;
	_vec3 m_vRight;

	War::DIR		m_eDir;			//방향값 저장
	War::KeyState	m_eKeyState;	//콤보 키 상태 저장 

	//머신 상태
	War::STATE		m_eMachineState;
	War::STATE		m_ePreMachineState;

	//애니메이션 관리
	War::Ani		m_ePreAniState;
	War::Ani		m_eCurAniState;

	//캐릭터 상태 
	War::CHARSTATE  m_eCharState;	
	_double dAttackCheckFrame;
	_double dDashCheckFrame;
	_double dJumpLandCheckFrame;

	War::WEAPON_ELEMENT m_eElement;


	//애니메이션 블랜드
	_bool	m_bBlend = true;

	//타이머
	_float m_fCToITime = 5.f;
	_float m_fCToISpeed = 0.f;


	_bool m_bUIOn = false;//UI 상태시 다른 조작 키에 접근 못하도록 하는 변수 

	_bool m_bUIShowing = false;
	//트랜스폼
	_float m_fMoveSpeed = 5.f;
	_float m_fInitDashSpeed ;
	_float m_fDashSpeed;
	_bool m_bDashMoveEnd = false;
	//공격 이동 조작
	_float m_fInitAttackMoveSpeed ;
	_float m_fAttackMoveSpeed;

	_bool m_bAttackMoveEnd = false;
	_bool m_bNexAni = false;
	_bool m_bNexAni2 = false;
	_float m_fAddPower = 0.f;

	_bool  m_bSkill2End = false;
	//점프
	_float	m_fJumpTime = 0.f;
	_float	m_fJumpPower = 0.f;
	_float	m_fInitJumpPower = 0.f;
	_float	m_fJumpY = 0.f;
	_bool	m_bJumpEnd = false;
	_bool	m_bJumpAniEnd = false;
	


	_float m_fDmg=0.f;

public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode);
	virtual void			Free(void);
};

#endif // Player_h__
