#ifndef Building_h__
#define Building_h__

#include "GameObject.h"
#include "Enum.h"
BEGIN(Engine)

class CTransform;
class CRenderer;
class CCalculator;
class CColliderSphere;
class CStaticMesh;
class CShader;
END


class CBuilding : public CGameObject
{
private:
	explicit CBuilding(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBuilding(const CBuilding& rhs);
	virtual ~CBuilding(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;


public :
	void SetProtoMesh(wstring wstrProtoMesh) { m_wstrProtoMesh = wstrProtoMesh; }
private:
	HRESULT			Add_Component(void);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
private:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CCalculator*		m_pCalculatorCom = nullptr;
	CColliderSphere*	m_pColliderCom = nullptr;
	CStaticMesh*		m_pMeshCom = nullptr;
	CShader*			m_pShaderCom = nullptr;


	_vec3		m_vDir;
	wstring		m_wstrProtoMesh;
public:
	static CBuilding*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CBuilding*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh);

private:
	virtual void			Free(void);
};

#endif // Building_h__
