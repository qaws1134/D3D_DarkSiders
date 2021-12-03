#ifndef Vulgrim_h__
#define Vulgrim_h__

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

class CVulgrim : public CGameObject
{
private:
	explicit CVulgrim(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVulgrim(const CVulgrim& rhs);
	virtual ~CVulgrim(void);

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
	Vulgrim::STATE		m_eMachineState;
	Vulgrim::STATE		m_ePreMachineState;

	//애니메이션 관리
	Vulgrim::Ani		m_ePreAniState;
	Vulgrim::Ani		m_eCurAniState;

	//애니메이션 블랜드
	_bool	m_bBlend = true;

	_bool m_bUIOn = false;

	_bool m_bSound = false;

public:
	static CVulgrim*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CVulgrim*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode);
	virtual void			Free(void);
};

#endif // Vulgrim_h__
