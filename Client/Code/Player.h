#ifndef Player_h__
#define Player_h__

#include "GameObject.h"
#include "Define.h"
#include "Enum.h"
BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
//class CColliderSphere;
END

class CPlayer : public CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual void Late_Ready_Object()override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

public:
	void		StateChange();
	void		StateLinker();		//다음 동작 연결 
	void		DirSet(War::DIR eDir, _float fTimeDelta);	//키에 따른 방향 결정
	void		RunState();
	//void		SetNextAniPos();
private:
	HRESULT			Add_Component();
	void			Key_Input(const _float& fTimeDelta);

private:
	CDynamicMesh*	m_pMeshCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;

private:
	//전투 상태판단
	_bool			m_bCombat = false;
	_bool			m_bCombo = false; // 콤보상태 판단
	_vec3 m_vDir;


	War::DIR		m_eDir;			//방향값 저장
	War::KeyState	m_eKeyState;	//콤보 키 상태 저장 

	//머신 상태
	War::STATE		m_eMachineState;
	War::STATE		m_ePreMachineState;

	//애니메이션 관리
	War::Ani		m_ePreAniState;
	War::Ani		m_eCurAniState;

	//캐릭터 상태 
	War::CHARSTATE  m_eCharState;	
	_double dAttackCheckFrame;

	
	//애니 동작 전 위치
	_vec3 m_vPreAniPos;


public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode);
	virtual void			Free(void);
};

#endif // Player_h__
