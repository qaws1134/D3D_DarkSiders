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
	void		StateLinker(_float fDeltaTime);		//���� ���� ���� 
	void		DirSet(War::DIR eDir, _float fTimeDelta, _float fAngleSpeed);	//Ű�� ���� ���� ����
	void		DirSet_Combo();

	void		ElementAniSet();
	_bool		Combat_to_Idle_Timer(_float fDeltaTime);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);


#pragma region Set �Լ�
public:
	void		Set_Element(War::WEAPON_ELEMENT eElement) { m_eElement = eElement; }
	void		Set_PlayerState(War::STATE eState) { m_eMachineState = eState; }
	void		Take_Dmg(_float fDmg) { m_fDmg = fDmg;  }
	void		Set_NaviMesh(CNaviMesh* pNavi) { m_pNavi = pNavi; }
#pragma  endregion Set �Լ�
#pragma region Get �Լ�

#pragma  endregion Set �Լ�


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
	


	_float m_fDmg=0.f;

public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode);
	virtual void			Free(void);
};

#endif // Player_h__
