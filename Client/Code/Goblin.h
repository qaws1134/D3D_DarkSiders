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

public:
	void		StateChange();
	void		StateLinker(_float fDeltaTime);		//���� ���� ���� 

	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	_bool		Pattern_Timer(_float fDeltaTime); // ���� ��� �ð� 

	void		SetPattern();

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
	Goblin::STATE		m_eMachineState;
	Goblin::STATE		m_ePreMachineState;


	//�ִϸ��̼� ����
	Goblin::Ani		m_ePreAniState;
	Goblin::Ani		m_eCurAniState;


	//�ִϸ��̼� ����
	_bool	m_bBlend = true;

	_float m_fPatternTimer;
	_float m_fPatternSpeed;

	_uint m_iPatternNum;


public:
	static CGoblin*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CGoblin*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode);
	virtual void			Free(void);
};

#endif // Goblin_h__
