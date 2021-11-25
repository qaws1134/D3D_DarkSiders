#ifndef Player_Barrier_h__
#define Player_Barrier_h__

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

class CPlayer_Barrier : public CGameObject
{
private:
	explicit CPlayer_Barrier(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer_Barrier(const CPlayer_Barrier& rhs);
	virtual ~CPlayer_Barrier(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual void Late_Ready_Object()override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

public:
	void		StateChange();
	void		StateActor(_float fDeltaTime);
	void		StateLinker(_float fDeltaTime);		//���� ���� ���� 
	virtual void		SetOption(void* pArg);
	
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);


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
	Player_Barrier::STATE		m_eMachineState;
	Player_Barrier::STATE		m_ePreMachineState;
	Player_Barrier::STATE		m_LastPatternState;

	//�ִϸ��̼� ����
	Player_Barrier::Ani		m_ePreAniState;
	Player_Barrier::Ani		m_eCurAniState;

	_float m_fFrameSpeed = 0.f;

	_float m_fSpawnTime;
	_float m_fSpawnSpeed = 10.f;
	_bool	m_bSpawn = false;
	//�ִϸ��̼� ����
	_bool	m_bBlend = true;
	

public:
	static CPlayer_Barrier*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CPlayer_Barrier*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode);
	virtual void			Free(void);
};

#endif // Player_Barrier_h__
