#ifndef Player_h__
#define Player_h__

#include "GameObject.h"
#include "Define.h"
#include "Enum.h"
#include "Struct.h"
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
	void		StateLinker(_float fDeltaTime);		//���� ���� ���� 
	void		DirSet(War::DIR eDir, _float fTimeDelta, _float fAngleSpeed);	//Ű�� ���� ���� ����
	void		DirSet_Combo();

	void		ElementAniSet();

	void		SetShake(_bool bShake) { m_bShake = bShake; }


	_bool		Combat_to_Idle_Timer(_float fDeltaTime);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	virtual		void TakeDmg(_float fDmg);
	void		AtkColActive(double dStart,double dEnd );
	void		AtkColLoop();
	_bool		GlideEndTimer(_float fTimeDelta);
	_bool		GlideTimer(_float fTimeDelta);
	_float		m_fGlideSpeed;
	_float		m_fGlideTime;
	_bool		m_bGlide = false;
	_bool		m_bGlideEnd = false;
	_float		m_fGlideEndSpeed;
	_float		m_fGlideEndTime;
	_bool		m_bGlideOn = false;

	_bool m_bShake = false;


	void		InteractionTimer(_float fDeltaTime);
	_bool		GetInteraction() {  return m_bInteraction; }
	void		ResetInteraction() { m_fInteractionSpeed = 1.f; }
	_bool		m_bInteraction = false;
	_float		m_fInteractionTime = 1.f;
	_float		m_fInteractionSpeed= 1.f;
	_bool		m_bInteractionOn = false;

	void		SpawnTrail();
	void		SetOnUI(_bool bUIOn) { m_bUIOn = bUIOn; }

	_float m_fFrameSpeed = 0.f;
	_float	m_fChestOpenTime = 1.f;
	_float	m_fChestOpenSpeed = 1.f;
	_bool m_bChestOpen = false;

#pragma region Set �Լ�
public:
	void		Set_Element(War::WEAPON_ELEMENT eElement) { m_eElement = eElement; }
	void		Set_PlayerState(War::STATE eState) { m_eMachineState = eState; }
	//void		Take_Dmg(_float fDmg) { m_fDmg = fDmg;  }
	//void		Set_NaviMesh(CNaviMesh* pNavi);
	void		Set_JumpBall(_bool bJumpBall) {m_bJumpBall= bJumpBall;}


#pragma  endregion Set �Լ�
#pragma region Get �Լ�

	_vec3	GetDir() { return m_vDir; }

#pragma  endregion Get �Լ�


private:
	HRESULT			Add_Component();
	void			Key_Input(const _float& fTimeDelta);

private:
	CDynamicMesh*	m_pMeshCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CNaviMesh*		m_pNavi = nullptr;

private:

	_vec3 m_vDir;
	_vec3 m_vRight;

	War::DIR		m_eDir;			//���Ⱚ ����
	War::KeyState	m_eKeyState;	//�޺� Ű ���� ���� 

	//�ӽ� ����
	War::STATE		m_eMachineState;
	War::STATE		m_ePreMachineState;

	//�ִϸ��̼� ����
	War::Ani		m_ePreAniState;
	War::Ani		m_eCurAniState;

	//ĳ���� ���� 
	War::CHARSTATE  m_eCharState;	
	_double dAttackCheckFrame;
	_double dDashCheckFrame;
	_double dJumpLandCheckFrame;

	War::WEAPON_ELEMENT m_eElement;
	War::WEAPON_ELEMENT m_ePreElement;
	TRAIL m_tTrail;
	TRAIL m_tOutTrail;	//�������� Ʈ���� 
	_float m_fTrailSizeOffset = 0.f;

	//�ִϸ��̼� ����
	_bool	m_bBlend = true;

	//Ÿ�̸�
	_float m_fCToITime = 5.f;
	_float m_fCToISpeed = 0.f;


	_bool m_bUIOn = false;//UI ���½� �ٸ� ���� Ű�� ���� ���ϵ��� �ϴ� ���� 

	_bool m_bUIShowing = false;
	//Ʈ������
	_float m_fMoveSpeed = 5.f;
	_float m_fInitDashSpeed ;
	_float m_fDashSpeed;
	_bool m_bDashMoveEnd = false;
	_float m_fHitMoveSpeed = 50.f;

	//���� �̵� ����
	_float m_fInitAttackMoveSpeed ;
	_float m_fAttackMoveSpeed;

	_bool m_bAttackMoveEnd = false;
	_bool m_bNexAni = false;
	_bool m_bNexAni2 = false;
	_float m_fAddPower = 0.f;

	_bool  m_bSkill2End = false;
	//����
	_float	m_fJumpTime = 0.f;
	_float	m_fJumpPower = 0.f;
	_float	m_fInitJumpPower = 0.f;
	_float	m_fJumpY = 0.f;
	_bool	m_bJumpEnd = false;
	_bool	m_bJumpAniEnd = false;
	
	_uint   m_eCompareType;

	//���� �浹ü �����ð�
	_double m_dAtkActiveStart=0.0;
	_double m_dAtkActiveEnd= 0.0;


	_bool	m_bJumpBall = false;
	_float	m_fJumpBallTime = 1.f;
	_float	m_fJumpBallSpeed = 1.f;
	_bool	m_bJumpBallStart = false;

	_float m_fSoundTime = 0.f;
	_float m_fSoundSpeed = 0.f;
	_bool  m_bSound = false;


	_bool m_bTrail = false;
	_vec3	m_vHitDir;

	_bool m_bElement =false;
	_float m_fElementTime = 0.5f;
	_float m_fElementSpeed = 0.5f; 


public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode);
	virtual void			Free(void);
};

#endif // Player_h__
