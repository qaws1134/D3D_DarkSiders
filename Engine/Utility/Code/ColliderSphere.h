#ifndef ColliderSphere_h__
#define ColliderSphere_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CColliderSphere : public CComponent
{
private:
	explicit CColliderSphere(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CColliderSphere(const CColliderSphere& rhs);
	virtual ~CColliderSphere(void);

public:
	 _matrix*			Get_ColliderWorld(void) { return &m_matColMatrix; }
	const _vec3*		Get_Center() { return &m_vCenter; }
	const _float*		Get_Radius() { return &m_fRadius; }


	void		Set_Center(_vec3 vCenter) { m_vCenter = vCenter; }
	void		Set_Radius(_float fRadius) { m_fRadius = fRadius; }
public:
	HRESULT			Ready_Collider(const _vec3* pPos,_float fRadius = 0);
	HRESULT			Ready_Collider(const _vec3* pPos,_ulong dwVtxCnt);
	void			Render_Collider(COLTYPE eType,  _matrix* pColliderMatrix);
	void			Update_Radius(_float fRadius);
private:
	_matrix			m_matColMatrix;
	_vec3			m_vCenter;
	_float			m_fRadius;
	
	ID3DXMesh* m_pSphereMesh = nullptr;



	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	LPDIRECT3DTEXTURE9			m_pTexture[COL_END];
	ID3DXBuffer* adjBuffer = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD        numMtrls = 0;



public:
	static CColliderSphere*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _float fRadius = 0);
	static CColliderSphere*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, _ulong dwVtxCnt);
	virtual CComponent*		Clone(void);
	virtual void			Free(void);
};

END
#endif // ColliderSphere_h__
