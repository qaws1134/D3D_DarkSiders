#include "RcTex.h"

USING(Engine)

Engine::CRcTex::CRcTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CRcTex::CRcTex(const CRcTex& rhs)
	: CVIBuffer(rhs)
	, m_pPos(rhs.m_pPos)
	, m_pUV(rhs.m_pUV)

{
}

Engine::CRcTex::~CRcTex(void)
{

}

void CRcTex::SetUV(_vec2 * pUV)
{

	VTXTEX*		pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for(_uint i = 0 ; i <4; i++)
		pVertex[i].vTexUV = pUV[i];


	m_pVB->Unlock();
}

void CRcTex::MoveUV(_vec2 vUV)
{
	m_pUV[0].x += vUV.x;
	m_pUV[1].x += vUV.x;
	m_pUV[2].x += vUV.x;
	m_pUV[3].x += vUV.x;

	m_pUV[0].y += vUV.y;
	m_pUV[1].y += vUV.y;
	m_pUV[2].y += vUV.y;
	m_pUV[3].y += vUV.y;
}

HRESULT Engine::CRcTex::Ready_Buffer(void)
{
	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);
	m_pPos = new _vec3[m_dwVtxCnt];
	m_pUV = new _vec2[m_dwVtxCnt];

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);


	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec3(0.5f, 0.5f, 0.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec3(0.5f, -0.5f, 0.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);
	
	for(_uint i = 0; i <4; i++)
		m_pPos[i] = pVertex[i].vPosition;

	for (_uint i = 0; i < 4; i++)
		m_pUV[i] = pVertex[i].vTexUV;

	
	INDEX16*	pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pVB->Unlock();
	m_pIB->Unlock();
	
	return S_OK;
}

void Engine::CRcTex::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

CRcTex* Engine::CRcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcTex*	pInstance = new CRcTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CRcTex::Free(void)
{
	if (false == m_bClone)
		Safe_Delete_Array(m_pPos);
	if (false == m_bClone)
	Safe_Delete_Array(m_pUV);

	CVIBuffer::Free();
}

CComponent* Engine::CRcTex::Clone(void)
{
	return new CRcTex(*this);
}

