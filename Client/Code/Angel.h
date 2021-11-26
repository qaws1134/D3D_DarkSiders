#ifndef Angel_h__
#define Angel_h__

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

class CAngel : public CGameObject
{
private:
	explicit CAngel(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAngel(const CAngel& rhs);
	virtual ~CAngel(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual void Late_Ready_Object()override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	void SetProtoTag(wstring wstrProtoTag) { m_wstrProtoTag = wstrProtoTag; }
public:
	void		StateChange();
	void		StateActor(_float fDeltaTime);
	void		StateLinker(_float fDeltaTime);		//���� ���� ���� 

	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	_bool		Pattern_Timer(_float fDeltaTime); // ���� ��� �ð� 

	void		SetAtkPattern();
	void		SetSpawnType(_uint eSpawn) { m_eSpawnType = eSpawn; }
	virtual		void TakeDmg(_float fDmg);
	void AtkColActive(double dStart, double dEnd, _uint iWeaponIdx);

	void DirSet(_float fDeltaTime,_float fAngleSpeed, _bool bAngleSet);
	void	DeadCheck();
	void EnemyTurn(_float fAngle);

	virtual void SetOption(void* pArg);
	void	SetNaviIdx(_uint iIdx) { m_iNaviIdx = iIdx; }
	//void SetInitNaviIdx(_uint iInitIdx) {m_iInitNaviIdx = iInitIdx}

#pragma region Set �Լ�
public:

#pragma  endregion Set �Լ�

#pragma region Get �Լ�

#pragma  endregion Set �Լ�


private:
	HRESULT			Add_Component();

private:
	CDynamicMesh*	m_pMeshCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CNaviMesh*		m_pNavi = nullptr;
private:
	wstring m_wstrProtoTag = L"";
	_vec3 m_vDir;
	_float m_fAngle =0.f;
	_float m_fMoveSpeed = 3.f;
	//�׳� ���� ������ ������� ����

	//�ӽ� ��	��
	Angel::STATE		m_eMachineState;
	Angel::STATE		m_ePreMachineState;

	//�ִϸ��̼� ����
	Angel::Ani		m_ePreAniState;
	Angel::Ani		m_eCurAniState;


	//�ִϸ��̼� ����
	_bool	m_bBlend = true;
	_double m_bBlendTime;

	//���� 
	_float m_fPatternTimer;
	_float m_fPatternSpeed;
	_uint m_iPatternNum;

	//���� ����
	_float	m_fAttackMoveSpeed;
	_float	m_fInitAttackMoveSpeed;
	_bool 	m_bAttackMoveEnd = false;
	_bool	m_bNexAni =false;


	_float m_fDragonTime = 3.f;
	_float m_fDragonSpeed= 3.f;

	_float m_fFrameSpeed = 0.f;

	//ü�̽� ��
	_bool	m_bChasingEnd = false;

	//�� ��
	_bool m_bTurnEnd = false;
	_float m_fTurnAngle = 0.f;
	_bool	m_bTurnCross = false;
	_bool	m_bTurnStartCross = false;

	//��Ʈ ����
	_bool  m_bHitStart = false;
	_bool	m_bHitEnd = false;
	_bool	m_bHitAniEnd = false;

	//Ÿ�� dir ���ϱ����ؼ�
	CTransform*	m_pTargetTransform = nullptr;
	_vec3	m_vTargetDir;
	_float m_fAngleSpeed;	//ȸ��
	_float m_fNaviY;
	
	//����
	_float m_fSpawnSpeed = 50.f;
	_bool m_bSpawnEnd = false;
	_uint m_eSpawnType;
	_bool	m_bSpawnDir = false;

	_uint m_iNaviIdx = 0;
public:
	static CAngel*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CAngel*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoTag);
	virtual void			Free(void);
};

#endif // Angel_h__
