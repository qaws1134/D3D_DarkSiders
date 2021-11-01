#ifndef DynamicMeshObj_h__
#define DynamicMeshObj_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CColliderSphere;
END

class CDynamicMeshObj : public CGameObject
{
private:
	explicit CDynamicMeshObj(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicMeshObj(const CDynamicMeshObj& rhs);
	virtual ~CDynamicMeshObj(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual void Late_Ready_Object()override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

public:
	void SetProtoMesh(wstring wstrProtoMesh) { m_wstrProtoMesh = wstrProtoMesh; }
	void SetColMode(_bool bColMode) { m_bColMode = bColMode; }
	//void SetCol(_bool	bCol) { m_bCol = bCol;}
private:
	HRESULT			Add_Component();
	//void			Key_Input(const _float& fTimeDelta);
	//void			SetUp_OnTerrain(void);
	//_vec3			PickUp_OnTerrain(void);

private:
	CDynamicMesh*	m_pMeshCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;
	CColliderSphere*		m_pColliderCom = nullptr;
	_vec3			m_vDir;

	wstring			m_wstrProtoMesh;

	_bool	m_bColMode = true;

	//_bool			m_bCol;
public:
	static CDynamicMeshObj*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CDynamicMeshObj*		Create(LPDIRECT3DDEVICE9 pGraphicDev,wstring ProtoMesh, _bool bColMode);
	virtual void			Free(void);
};

#endif // DynamicMeshObj_h__
