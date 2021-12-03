#ifndef SkyBox_h__
#define SkyBox_h__

#include "GameObject.h"
#include "Enum.h"
BEGIN(Engine)

class CTransform;
class CRenderer;
class CCalculator;
class CColliderSphere;
class CStaticMesh;
class CShader;
class CColliderSphere;
END


class CSkyBox : public CGameObject
{
private:
	explicit CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSkyBox(const CSkyBox& rhs);
	virtual ~CSkyBox(void);

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
	CColliderSphere*	m_pColliderCom = nullptr;

	_float m_fAccTime;
	_float m_fUVSpeed;
	_vec3		m_vDir;
	wstring		m_wstrProtoMesh;
public:
	static CSkyBox*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

	static CSkyBox*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh);
	virtual void			Free(void);
};

#endif // SkyBox_h__
