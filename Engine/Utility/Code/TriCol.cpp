#include "TriCol.h"

USING(Engine)

Engine::CTriCol::CTriCol(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CTriCol::CTriCol(const CTriCol& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CTriCol::~CTriCol(void)
{

}

HRESULT Engine::CTriCol::Ready_Buffer(void)
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 3;
	m_dwFVF = FVF_COL;
	m_dwIdxSize = sizeof(INDEX16);
	m_dwTriCnt = 1;
	m_IdxFmt = D3DFMT_INDEX16;
	
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCOL*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// 외부 공간에서 버텍스 정보에 접근할 수 없도록 메모리를 잠그는 행위
	// 3인자 값은 버텍스 버퍼에 저장된 버텍스들 중 첫 번째 주소 값을 반환

	pVertex[0].vPosition = _vec3(0.f, 0.5f, 0.f);
	pVertex[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[1].vPosition = _vec3(0.5f, -0.5f, 0.f);
	pVertex[1].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[2].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[2].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	m_pIB->Unlock();

	return S_OK;
}

//세개 점을 받아서 그림 
HRESULT CTriCol::Ready_Buffer(_vec3 * pArryPos)
{	
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 3;
	m_dwFVF = FVF_COL;
	m_dwIdxSize = sizeof(INDEX16);
	m_dwTriCnt = 1;
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCOL*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// 외부 공간에서 버텍스 정보에 접근할 수 없도록 메모리를 잠그는 행위
	// 3인자 값은 버텍스 버퍼에 저장된 버텍스들 중 첫 번째 주소 값을 반환

	_matrix pWorld[3];
	for (_uint i =  0 ; i < 3; i++)
		D3DXMatrixIdentity(&pWorld[i]);

	//월드 점 받아서 로컬로 내려서 그림
	for (_uint i = 0; i < m_dwVtxCnt; i++)
	{
		m_pGraphicDev->GetTransform(D3DTS_WORLD, pWorld + i);
		D3DXMatrixInverse(pWorld + i, NULL, pWorld + i);
		D3DXVec3TransformCoord(pArryPos + i, pArryPos + i, (pWorld + i));
		//역행렬로 로컬행렬을 구함 
		pVertex[i].vPosition = _vec3((pArryPos + i)->x, (pArryPos + i)->y, (pArryPos + i)->z);
		pVertex[i].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	}


	m_pVB->Unlock();
	INDEX16*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
	//인덱싱해서 그림
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	m_pIB->Unlock();

	return S_OK;
}

void Engine::CTriCol::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

void CTriCol::Update_Buffer(_vec3 * pArryPos)
{
	VTXCOL*		pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_matrix pWorld[3];
	for (_uint i = 0; i < 3; i++)
		D3DXMatrixIdentity(&pWorld[i]);

	//월드 점 받아서 로컬로 내려서 그림
	for (_uint i = 0; i < m_dwVtxCnt; i++)
	{
		m_pGraphicDev->GetTransform(D3DTS_WORLD, pWorld + i);
		D3DXMatrixInverse(pWorld + i, NULL, pWorld + i);
		D3DXVec3TransformCoord(pArryPos + i, pArryPos + i, (pWorld + i));
		//역행렬로 로컬행렬을 구함 
		pVertex[i].vPosition = _vec3((pArryPos + i)->x, (pArryPos + i)->y, (pArryPos + i)->z);
		pVertex[i].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	}

	m_pVB->Unlock();
}

void Engine::CTriCol::Free(void)
{
	CVIBuffer::Free();
}

CComponent* Engine::CTriCol::Clone(void)
{
	return new CTriCol(*this);
}


CTriCol* Engine::CTriCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTriCol*	pInstance = new CTriCol(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
		Safe_Release(pInstance);

	return pInstance;
}

CTriCol * CTriCol::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * pPos)
{
	CTriCol*	pInstance = new CTriCol(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(pPos)))
		Safe_Release(pInstance);

	return pInstance;
}

