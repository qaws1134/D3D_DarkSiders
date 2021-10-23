#ifndef Stone_h__
#define Stone_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CTransform;
class CRenderer;
class CCalculator;
class CStaticMesh;

END

class CStaticMeshObj : public CGameObject
{
private:
	explicit CStaticMeshObj(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaticMeshObj(const CStaticMeshObj& rhs);
	virtual ~CStaticMeshObj(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual void Late_Ready_Object()override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

public:
	void SetProtoMesh(wstring wstrProtoMesh) { m_wstrProtoMesh = wstrProtoMesh; }

private:
	HRESULT			Add_Component();
	//void			Key_Input(const _float& fTimeDelta);
	//void			SetUp_OnTerrain(void);
	//_vec3			PickUp_OnTerrain(void);

private:
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CCalculator*	m_pCalculatorCom = nullptr;
	CStaticMesh*	m_pMeshCom = nullptr;

	_vec3		m_vDir;
	wstring		m_wstrProtoMesh;

public:
	static CStaticMeshObj*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CStaticMeshObj*		Create(LPDIRECT3DDEVICE9 pGraphicDev,wstring ProtoMesh);
	virtual void			Free(void);
};

#endif // Stone_h__
