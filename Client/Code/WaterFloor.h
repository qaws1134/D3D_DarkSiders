#ifndef WaterFloor_h__
#define WaterFloor_h__

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


class CWaterFloor : public CGameObject
{
private:
	explicit CWaterFloor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWaterFloor(const CWaterFloor& rhs);
	virtual ~CWaterFloor(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;
	_float GetRandomFloat(_float lowBound, _float highBound);
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
	_uint m_iOption = 1;

	_vec4 m_vColor;
public:
	static CWaterFloor*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

	static CWaterFloor*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh);
	virtual void			Free(void);
};

#endif // WaterFloor_h__
