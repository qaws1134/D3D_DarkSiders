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
	void		StateLinker(_float fDeltaTime);		//���� ���� ���� 
	void		DirSet(War::DIR eDir, _float fTimeDelta, _float fAngleSpeed);	//Ű�� ���� ���� ����
	void		DirSet_Combo();
	void		ElementSet();
	_bool		Combat_to_Idle_Timer(_float fDeltaTime);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
private:
	HRESULT			Add_Component();
	void			Key_Input(const _float& fTimeDelta);

private:
	CDynamicMesh*	m_pMeshCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;
	CShader*			m_pShaderCom = nullptr;

private:
	//���� �����Ǵ�
	_bool			m_bCombat = false;
	_bool			m_bCombo = false; // �޺����� �Ǵ�
	_vec3 m_vDir;


	War::DIR		m_eDir;			//���Ⱚ ����

	War::KeyState	m_eKeyState;	//�޺� Ű ���� ���� 

	_bool m_bCancle;
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
	
	//�ִ� ���� �� ��ġ
	_vec3 m_vPreAniPos;


	//Ÿ�̸�
	_float m_fCToITime = 5.f;
	_float m_fCToISpeed = 0.f;




public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode);
	virtual void			Free(void);
};

#endif // Player_h__
