#include "ColliderSphere.h"

USING(Engine)

Engine::CColliderSphere::CColliderSphere(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pIB(nullptr)
	, m_pVB(nullptr)
{

}

Engine::CColliderSphere::CColliderSphere(const CColliderSphere& rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_fRadius(rhs.m_fRadius)
	, m_vCenter(rhs.m_vCenter)
	, m_matColMatrix(rhs.m_matColMatrix)
{
	for (_ulong i = 0; i < COL_END; i++)
	{
		m_pTexture[i] = rhs.m_pTexture[i];
		m_pTexture[i]->AddRef();
	}
}

Engine::CColliderSphere::~CColliderSphere(void)
{

}

HRESULT CColliderSphere::Ready_Collider(const _vec3 * pPos,_float fRadius)
{
	//메시 사이즈에 맞는 바운딩 박스를 만글기
	//가장 작은 좌표 값과 가증 큰 좌표 값을 만들어주는 함수
	//D3DXComputeBoundingSphere(pPos, dwVtxCnt, sizeof(_vec3), &m_vCenter, &fRadius);

#ifdef  _DEBUG
	
	D3DXCreateSphere(
		m_pGraphicDev,
		fRadius,
		20,
		20,
		&m_pSphereMesh,
		0);

	for (_ulong i = 0; i < COL_END; ++i)
	{
		D3DXCreateTexture(m_pGraphicDev,
			1,
			1,
			1,
			0,
			D3DFMT_A8R8G8B8,
			D3DPOOL_MANAGED,
			&m_pTexture[i]);

		D3DLOCKED_RECT LockRect;
		m_pTexture[i]->LockRect(0, &LockRect, NULL, 0);
		*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f*i, 1.f*(1.f - i), 0.f, 1.f);

		m_pTexture[i]->UnlockRect(0);
	}

#endif //  _DEBUG

	return S_OK;
}

void CColliderSphere::Render_Collider(COLTYPE eType,  _matrix * pColliderMatrix)
{
	m_matColMatrix = *pColliderMatrix;

#ifdef _DEBUG
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matColMatrix);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetTexture(0, m_pTexture[eType]);
	
	
	m_pSphereMesh->DrawSubset(0);


	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif

}


CColliderSphere * CColliderSphere::Create(LPDIRECT3DDEVICE9 pGraphicDev
	, _vec3* pPos
	, _float fRadius)
{
	CColliderSphere*	pInstance = new	CColliderSphere(pGraphicDev);

	if (FAILED(pInstance->Ready_Collider(pPos, fRadius)))
		Safe_Release(pInstance);

	return pInstance;
}


Engine::CComponent* Engine::CColliderSphere::Clone(void)
{
	return new CColliderSphere(*this);
}

void Engine::CColliderSphere::Free(void)
{
	for (_ulong i = 0; i < COL_END; ++i)
		Safe_Release(m_pTexture[i]);

	Safe_Release(m_pIB);
	Safe_Release(m_pVB);
	Safe_Release(m_pSphereMesh);
	CComponent::Free();
}


