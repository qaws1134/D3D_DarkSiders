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
	void		StateLinker(_float fDeltaTime);		//���� ���� ���� 

	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	_bool		Pattern_Timer(_float fDeltaTime); // ���� ��� �ð� 

	void		SetPattern();
	void		SetSpawnType(_uint eSpawn) { m_eSpawnType = eSpawn; }
	virtual		void TakeDmg(_float fDmg);
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

	_vec3 m_vDir;

	//�׳� ���� ������ ������� ����

	//�ӽ� ��	��
	Grinner::STATE		m_eMachineState;
	Grinner::STATE		m_ePreMachineState;

	//�ִϸ��̼� ����
	Grinner::Ani		m_ePreAniState;
	Grinner::Ani		m_eCurAniState;


	//�ִϸ��̼� ����
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
