#ifndef Serpent_h__
#define Serpent_h__

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

class CSerpent : public CGameObject
{
private:
	explicit CSerpent(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSerpent(const CSerpent& rhs);
	virtual ~CSerpent(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual void Late_Ready_Object()override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

public:
	void		StateChange();
	void		StateLinker(_float fDeltaTime);		//다음 동작 연결 

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


	//그냥 왼쪽 오른쪽 순서대로 ㄱㄱ

	//머신 상	태
	Serpent::STATE		m_eMachineState;
	Serpent::STATE		m_ePreMachineState;

	//애니메이션 관리
	Serpent::Ani		m_ePreAniState;
	Serpent::Ani		m_eCurAniState;

	//애니메이션 블랜드
	_bool	m_bBlend = true;

	_bool m_bSpawn = false;
	_double m_dAniPosition;

public:
	static CSerpent*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CSerpent*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode);
	virtual void			Free(void);
};

#endif // Serpent_h__
