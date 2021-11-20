#ifndef WaterFall_h__
#define WaterFall_h__

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


class CWaterFall : public CGameObject
{
private:
	explicit CWaterFall(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWaterFall(const CWaterFall& rhs);
	virtual ~CWaterFall(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

public:
	void SetProtoMesh(wstring wstrProtoMesh) { m_wstrProtoMesh = wstrProtoMesh; }
private:
	HRESULT			Add_Component(void);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
private:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;


	CStaticMesh*		m_pMeshCom = nullptr;
	CShader*			m_pShaderCom = nullptr;


	_vec3		m_vDir;
	wstring		m_wstrProtoMesh;
public:
	static CWaterFall*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

	static CWaterFall*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh);
	virtual void			Free(void);
};

#endif // WaterFall_h__
