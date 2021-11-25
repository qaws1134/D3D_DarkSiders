#ifndef Goblin_h__
#define Goblin_h__

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

class CGoblin : public CGameObject
{
private:
	explicit CGoblin(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGoblin(const CGoblin& rhs);
	virtual ~CGoblin(void);

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
	void		SetSpawnSpeed(_float fSpawnSpeed) { m_fSpawnSpeed = fSpawnSpeed; }
	virtual		void TakeDmg(_float fDmg);
	void AtkColActive(double dStart, double dEnd, _uint iWeaponIdx);
	void		SetNaviIdx(_uint iIdx) { m_iNaviIdx = iIdx; }


	void DirSet(_float fDeltaTime, _float fAngleSpeed, _bool bAngleSet);
	void SpawnBarfinBullet(_float  fAngle);
	void	DeadCheck();
	void EnemyTurn(_float fAngle);

	virtual void SetOption(void* pArg);
	_float GetRandomFloat(_float lowBound, _float highBound);
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
	_float m_fAngle = 0.f;
	_float m_fMoveSpeed = 3.f;
	//�׳� ���� ������ ������� ����

	//�ӽ� ��	��
	Goblin::STATE		m_eMachineState;
	Goblin::STATE		m_ePreMachineState;

	//�ִϸ��̼� ����
	Goblin::Ani		m_ePreAniState;
	Goblin::Ani		m_eCurAniState;


	//�ִϸ��̼� ����
	_bool	m_bBlend = true;
	_double m_bBlendTime;

	//���� 
	_float m_fPatternTimer;
	_float m_fPatternSpeed;
	_uint m_iPatternNum;
	_bool m_bBackStep = false;

	//���� ����
	_float	m_fAttackMoveSpeed;
	_float	m_fInitAttackMoveSpeed;
	_bool 	m_bAttackMoveEnd = false;
	_bool	m_bNexAni = false;

	_bool	m_bSpear= false;
	_float	m_fSpearAngle = 0.f;
	_float m_fBulletSpeed = 10.f;

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
	_bool m_bSpawnAniEnd = false;
	_uint m_eSpawnType;
	_bool	m_bSpawnDir = false;
	_uint m_iNaviIdx = 0;

public:
	static CGoblin*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CGoblin*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoTag);
	virtual void			Free(void);
};

#endif // Goblin_h__
