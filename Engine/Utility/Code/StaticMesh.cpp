#include "StaticMesh.h"

USING(Engine)

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pOriMesh(nullptr)
	, m_pMesh(nullptr)
	, m_pAdjacency(nullptr)
	, m_pSubset(nullptr)
	, m_pMtrl(nullptr)
	, m_dwSubsetCnt(0)
	, m_ppTexture(nullptr)
{

}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
	: CComponent(rhs)
	, m_pOriMesh(rhs.m_pOriMesh)
	, m_pMesh(rhs.m_pMesh)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pSubset(rhs.m_pSubset)
	, m_pMtrl(rhs.m_pMtrl)
	, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
{
	m_ppTexture = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];

	for (_ulong i = 0; i < rhs.m_dwSubsetCnt; ++i)
	{
		m_ppTexture[i] = rhs.m_ppTexture[i];
		m_ppTexture[i]->AddRef();
	}
	m_pOriMesh->AddRef();
	m_pAdjacency->AddRef();
	m_pSubset->AddRef();
	m_pMesh->AddRef();
}

Engine::CStaticMesh::~CStaticMesh(void)
{

}

HRESULT Engine::CStaticMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar	szFullPath[MAX_PATH] = L"";

	// 메쉬의 최종 경로를 만들어주는 코드
	// Bin\Resource\Mesh\StaticMesh\Sword
	lstrcpy(szFullPath, pFilePath);
	// Bin\Resource\Mesh\StaticMesh\Sword\SWORD.x
	lstrcat(szFullPath, pFileName);

	if (FAILED(D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphicDev,
								&m_pAdjacency,
								&m_pSubset, 
								NULL,
								&m_dwSubsetCnt,
								&m_pOriMesh)))
								return E_FAIL;

	_ulong	dwFVF = m_pOriMesh->GetFVF();

	// 메쉬 정점 정보중 노말 값이 없는 경우
	if (!(dwFVF & D3DFVF_NORMAL))
	{
		// 현재 메쉬가 지닌 정점의 fvf정보를 2번째 인자값의 형태로 복제하는 함수
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &m_pMesh);
		// 인접한 정점 또는 폴리곤 정보를 근거로 법선을 만들어주는 함수
		D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
	}
	else
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pGraphicDev, &m_pMesh);
	}


	// m_pSubset가 갖고있는 정보의 가장 앞 주소를 리턴하는 함수
	m_pMtrl = (D3DXMATERIAL*)m_pSubset->GetBufferPointer();

	m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_tchar	szFileName[256] = L"";

		MultiByteToWideChar(CP_ACP, 0, m_pMtrl[i].pTextureFilename, strlen(m_pMtrl[i].pTextureFilename), szFileName, 256);

		// 메쉬 텍스처의 최종 경로를 만들어주는 코드
		lstrcpy(szFullPath, pFilePath);
		lstrcat(szFullPath, szFileName);

		if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppTexture[i])))
			return E_FAIL;
	}

	return S_OK;
}

void Engine::CStaticMesh::Render_Meshes(void)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pGraphicDev->SetTexture(0, m_ppTexture[i]);
		m_pMesh->DrawSubset(i);
	}
}

Engine::CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CStaticMesh*	pInstance = new CStaticMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CComponent* Engine::CStaticMesh::Clone(void)
{
	return new CStaticMesh(*this);
}

void Engine::CStaticMesh::Free(void)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
		Safe_Release(m_ppTexture[i]);

	Safe_Delete_Array(m_ppTexture);

	Safe_Release(m_pMesh);
	Safe_Release(m_pOriMesh);
	Safe_Release(m_pSubset);
	Safe_Release(m_pAdjacency);

	CComponent::Free();
}

