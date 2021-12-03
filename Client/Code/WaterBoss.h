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
	void AtkColActive(double dStart, double dEnd, _uint iWeaponIdx, _vec3* pPos);


	void SpawnTentaFog(_float fAngle, _vec3 vPos);
public:
	void		StateChange();
	void		StateActor(_float fDeltaTime);
	void		StateLinker(_float fDeltaTime);		//���� ���� ���� 

	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	_bool		Pattern_Timer(_float fDeltaTime); // ���� ��� �ð� 

	void		SetPattern();
	void		SetSlamPattern();
	virtual void SetOption(void * pArg);

	void		SpawnOrb();
	void		DeadCheck();
	_float GetRandomFloat(_float lowBound, _float highBound);
	virtual void TakeDmg(_float Atk);


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

private:
	queue<EFFECT::TYPE2D> m_queReadyEff2D;
	queue<EFFECT::TYPE3D> m_queReadyEff3D;

	_vec3 m_vDir;

	//�׳� ���� ������ ������� ����

	//�ӽ� ��	��
	WaterBoss::STATE		m_eMachineState;
	WaterBoss::STATE		m_ePreMachineState;
	WaterBoss::STATE		m_LastPatternState;

	//�ִϸ��̼� ����
	WaterBoss::Ani		m_ePreAniState;
	WaterBoss::Ani		m_eCurAniState;


	//�ִϸ��̼� ����
	_bool	m_bBlend = true;

	_float m_fPatternTimer;
	_float m_fPatternSpeed;


	_uint m_iPatternNum;
	_uint m_iSlamPatternNum;

	_float m_fEffSpawnTime ;
	_float m_fEffSpawnSpeed;

	_bool	m_bSpawnEff =false;
	_bool	m_bPummel[4] = { false,false, false, false };

	//����Ʈ ó��
	_bool m_bOrb = false;
	_bool m_bOrbCross= false;

	_vec4 m_vColor;

public:
	static CWaterBoss*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CWaterBoss*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode);
	virtual void			Free(void);
};

#endif // WaterBoss_h__
