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
	void		StateLinker(_float fDeltaTime);		//다음 동작 연결 
	virtual void		SetOption(void* pArg);
	
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);


#pragma region Set 함수
public:

#pragma  endregion Set 함수

#pragma region Get 함수

#pragma  endregion Set 함수


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

	//그냥 왼쪽 오른쪽 순서대로 ㄱㄱ

	//머신 상	태
	Player_Barrier::STATE		m_eMachineState;
	Player_Barrier::STATE		m_ePreMachineState;
	Player_Barrier::STATE		m_LastPatternState;

	//애니메이션 관리
	Player_Barrier::Ani		m_ePreAniState;
	Player_Barrier::Ani		m_eCurAniState;

	_float m_fFrameSpeed = 0.f;

	_float m_fSpawnTime;
	_float m_fSpawnSpeed = 10.f;
	_bool	m_bSpawn = false;
	//애니메이션 블랜드
	_bool	m_bBlend = true;
	

public:
	static CPlayer_Barrier*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CPlayer_Barrier*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode);
	virtual void			Free(void);
};

#endif // Player_Barrier_h__
