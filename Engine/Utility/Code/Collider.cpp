#include "Collider.h"

USING(Engine)

Engine::CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	,m_pIB(nullptr)
	,m_pVB(nullptr)
{

}

Engine::CCollider::CCollider(const CCollider& rhs)
	: CComponent(rhs)
	,m_pVB(rhs.m_pVB)
	,m_pIB(rhs.m_pIB)
	,m_vMin(rhs.m_vMin)
	,m_vMax(rhs.m_vMax)
	,m_matColMatrix(rhs.m_matColMatrix)
{
	for (_ulong i = 0; i < COL_END; i++)
	{
		m_pTexture[i] = rhs.m_pTexture[i];
		m_pTexture[i]->AddRef();
	}
}

Engine::CCollider::~CCollider(void)
{

}

HRESULT CCollider::Ready_Collider(const _vec3 * pPos,
									const _ulong & dwVtxCnt,
									const _ulong & dwStride)
{
	//�޽� ����� �´� �ٿ�� �ڽ��� ���۱�
	//���� ���� ��ǥ ���� ���� ū ��ǥ ���� ������ִ� �Լ�
	D3DXComputeBoundingBox(pPos, dwVtxCnt, sizeof(_vec3), &m_vMin, &m_vMax);

#ifdef  _DEBUG
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXCUBE) * 8,
		0,
		FVF_CUBE,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL)))
		return E_FAIL;

	if (FAILED(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 12,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL)))
		return E_FAIL;

	VTXCUBE* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	//�ո�
	pVertex[0].vPosition = _vec3(m_vMin.x, m_vMax.y, m_vMin.z);
	pVertex[0].vTexUV = pVertex[0].vPosition;

	pVertex[1].vPosition = _vec3(m_vMax.x, m_vMax.y, m_vMin.z);
	pVertex[1].vTexUV = pVertex[1].vPosition;

	pVertex[2].vPosition = _vec3(m_vMax.x, m_vMin.y, m_vMin.z);
	pVertex[2].vTexUV = pVertex[2].vPosition;
	
	pVertex[3].vPosition = _vec3(m_vMin.x, m_vMin.y, m_vMin.z);
	pVertex[3].vTexUV = pVertex[3].vPosition;

	//�޸�
	pVertex[4].vPosition = _vec3(m_vMin.x, m_vMax.y, m_vMax.z);
	pVertex[4].vTexUV = pVertex[4].vPosition;

	pVertex[5].vPosition = _vec3(m_vMax.x, m_vMax.y, m_vMax.z);
	pVertex[5].vTexUV = pVertex[5].vPosition;

	pVertex[6].vPosition = _vec3(m_vMax.x, m_vMin.y, m_vMax.z);
	pVertex[6].vTexUV = pVertex[6].vPosition;

	pVertex[7].vPosition = _vec3(m_vMin.x, m_vMin.y, m_vMax.z);
	pVertex[7].vTexUV = pVertex[7].vPosition;

	m_pVB->Unlock();

	INDEX16*	pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// X+
	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;

	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;

	// X-
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;

	// y+
	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;

	// y-
	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;

	// z+
	pIndex[8]._0 = 7;
	pIndex[8]._1 = 6;
	pIndex[8]._2 = 5;

	pIndex[9]._0 = 7;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 4;

	// z-
	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;

	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;
	
	m_pIB->Unlock();

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

void CCollider::Render_Collider(COLTYPE eType, const _matrix * pColliderMatrix)
{
	m_matColMatrix = *pColliderMatrix;

#ifdef _DEBUG
	m_pGraphicDev->SetTransform(D3DTS_WORLD, pColliderMatrix);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetTexture(0, m_pTexture[eType]);

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXCUBE));
	m_pGraphicDev->SetFVF(FVF_CUBE);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif

}

CCollider * CCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
								const _vec3 * pPos, 
								const _ulong & dwVtxCnt, 
								const _ulong & dwStride)
{
	CCollider*		pInstance = new	CCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_Collider(pPos, dwVtxCnt, dwStride)))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CComponent* Engine::CCollider::Clone(void)
{
	return new CCollider(*this);
}
void Engine::CCollider::Free(void)
{
	for (_ulong i = 0; i < COL_END; ++i)
		Safe_Release(m_pTexture[i]);

	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	CComponent::Free();
}


