#ifndef Chest_h__
#define Chest_h__

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

class CChest : public CGameObject
{
private:
	explicit CChest(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CChest(const CChest& rhs);
	virtual ~CChest(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual void Late_Ready_Object()override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

public:
	void		StateChange();
	void		StateLinker(_float fDeltaTime);		//���� ���� ���� 

	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

	virtual void TakeDmg(_float fAtk);
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


	//�׳� ���� ������ ������� ����

	//�ӽ� ��	��
	Chest::STATE		m_eMachineState;
	Chest::STATE		m_ePreMachineState;

	//�ִϸ��̼� ����
	Chest::Ani		m_ePreAniState;
	Chest::Ani		m_eCurAniState;

	//�ִϸ��̼� ����
	_bool	m_bBlend = true;

	_bool m_bSpawnItem = false;


public:
	static CChest*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CChest*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode);
	virtual void			Free(void);
};

#endif // Chest_h__
