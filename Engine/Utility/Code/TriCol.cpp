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
	// �ܺ� �������� ���ؽ� ������ ������ �� ������ �޸𸮸� ��״� ����
	// 3���� ���� ���ؽ� ���ۿ� ����� ���ؽ��� �� ù ��° �ּ� ���� ��ȯ

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

//���� ���� �޾Ƽ� �׸� 
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
	// �ܺ� �������� ���ؽ� ������ ������ �� ������ �޸𸮸� ��״� ����
	// 3���� ���� ���ؽ� ���ۿ� ����� ���ؽ��� �� ù ��° �ּ� ���� ��ȯ

	//�� 3���� ���� ��Ʈ������ ���� 
	_matrix* pWorld = nullptr;

	for (_uint i = 0; i< m_dwVtxCnt; i++)
	{
		D3DXVec3TransformCoord(pArryPos+i, pArryPos+i, pWorld +i);
		//����ķ� ��������� ���� 
		D3DXMatrixInverse(pWorld + i, NULL, pWorld + i);
		pVertex[i].vPosition = _vec3((pWorld+i)->_41, (pWorld + i)->_42, (pWorld + i)->_43);
		pVertex[i].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	}

	m_pVB->Unlock();

	INDEX16*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

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

