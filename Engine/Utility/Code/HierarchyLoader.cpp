#include "HierarchyLoader.h"

USING(Engine)

Engine::CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath)
	: m_pGraphicDev(pGraphicDev)
	, m_pPath(pPath)
{
	m_pGraphicDev->AddRef();
}

Engine::CHierarchyLoader::~CHierarchyLoader(void)
{

}

STDMETHODIMP Engine::CHierarchyLoader::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	D3DXFRAME_DERIVED*		pDerivedMatrix = new D3DXFRAME_DERIVED;
	ZeroMemory(pDerivedMatrix, sizeof(D3DXFRAME_DERIVED));

	Allocate_Name(&pDerivedMatrix->Name, Name);

	pDerivedMatrix->CombinedTransformMatrix = *D3DXMatrixIdentity(&pDerivedMatrix->TransformationMatrix);

	*ppNewFrame = pDerivedMatrix;

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::CreateMeshContainer(THIS_ LPCSTR Name, 
															CONST D3DXMESHDATA *pMeshData, 
															CONST D3DXMATERIAL *pMaterials,
															CONST D3DXEFFECTINSTANCE *pEffectInstances,
															DWORD NumMaterials,
															CONST DWORD *pAdjacency,
															LPD3DXSKININFO pSkinInfo,
															LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pDerivedMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));

	Allocate_Name(&pDerivedMeshContainer->Name, Name);

	pDerivedMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	LPD3DXMESH	pMesh = pMeshData->pMesh;

	// 메쉬가 지닌 폴리곤의 개수를 반환
	_ulong	dwNumFaces = pMesh->GetNumFaces();

	pDerivedMeshContainer->pAdjacency = new _ulong[dwNumFaces * 3];
	memcpy(pDerivedMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * dwNumFaces * 3);

	

	_ulong	dwFVF = pMesh->GetFVF();

	//if (!(dwFVF & D3DFVF_NORMAL))
	//{



	pMesh->CloneMesh(pMesh->GetOptions(), vertexDecl, m_pGraphicDev, &pDerivedMeshContainer->MeshData.pMesh);
	// 현재 메쉬가 지닌 정점의 fvf정보를 2번째 인자값의 형태로 복제하는 함수
	//m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &m_pMesh);
	// 인접한 정점 또는 폴리곤 정보를 근거로 법선을 만들어주는 함수
	D3DXComputeNormals(pDerivedMeshContainer->MeshData.pMesh, pDerivedMeshContainer->pAdjacency);
	D3DXComputeTangent(pDerivedMeshContainer->MeshData.pMesh, 0, 0, 0, 0, pDerivedMeshContainer->pAdjacency);

	/*	pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &pDerivedMeshContainer->MeshData.pMesh);
		D3DXComputeNormals(pDerivedMeshContainer->MeshData.pMesh, pDerivedMeshContainer->pAdjacency);*/
	//}
	//else
	//{

	//	const D3DVERTEXELEMENT9 vertexDecl[] = 
	//	{
	//		{}
	//	}


	//	pMesh->CloneMesh(pMesh->GetOptions(), )




	//	pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF, m_pGraphicDev, &pDerivedMeshContainer->MeshData.pMesh);
	//}

	pDerivedMeshContainer->NumMaterials = (NumMaterials == 0 ? 1 : NumMaterials);

	pDerivedMeshContainer->pMaterials = new D3DXMATERIAL[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->pMaterials, sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials);
	
	pDerivedMeshContainer->ppTexture = new LPDIRECT3DTEXTURE9[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->ppTexture, sizeof(LPDIRECT3DTEXTURE9) * pDerivedMeshContainer->NumMaterials);

	pDerivedMeshContainer->ppNormalTextures = new LPDIRECT3DTEXTURE9[pDerivedMeshContainer->NumMaterials]();
	//pDerivedMeshContainer->ppEmissiveTextures = new LPDIRECT3DTEXTURE9[pDerivedMeshContainer->NumMaterials]();





	if (0 != NumMaterials)
	{
		memcpy(pDerivedMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials);
		
		_tchar szNormalTex[128] = L"";
		_tchar szSpecTex[128] = L"";
		
		for (_ulong i = 0; i < pDerivedMeshContainer->NumMaterials; ++i)
		{
			_tchar	szFullPath[MAX_PATH] = L"";
			_tchar	szFileName[256] = L"";

			MultiByteToWideChar(CP_ACP, 0, pDerivedMeshContainer->pMaterials[i].pTextureFilename, 
				strlen(pDerivedMeshContainer->pMaterials[i].pTextureFilename), szFileName, 256);

			// 메쉬 텍스처의 최종 경로를 만들어주는 코드
			lstrcpy(szFullPath, m_pPath);
			lstrcat(szFullPath, szFileName);

			if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pDerivedMeshContainer->ppTexture[i])))
				return E_FAIL;


			wstring wstrFileName = szFileName;
			size_t iSize= wstrFileName.find(L"_d.",0);
			wstrFileName[iSize + 1] = L'n';

			lstrcpy(szFullPath, m_pPath);
			lstrcat(szFullPath, wstrFileName.c_str());



			if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pDerivedMeshContainer->ppNormalTextures[i])))
			{
				pDerivedMeshContainer->ppNormalTextures[i] = nullptr;
			}

			// wstrFileName = szFileName;
			//iSize = wstrFileName.find(L"_d.", 0);
			//wstrFileName[iSize + 1] = L'e';

			//lstrcpy(szFullPath, m_pPath);
			//lstrcat(szFullPath, wstrFileName.c_str());

			//if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pDerivedMeshContainer->ppEmissiveTextures[i])))
			//{
			//	pDerivedMeshContainer->ppEmissiveTextures[i] = nullptr;
			//}
		}

	}
	// 재질 정보가 없는 경우
	else
	{
		pDerivedMeshContainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Power = 0.f;

		pDerivedMeshContainer->ppTexture[0] = nullptr;
		pDerivedMeshContainer->ppNormalTextures[0] = nullptr;
		//pDerivedMeshContainer->ppEmissiveTextures[0] = nullptr;
	}

	if (nullptr == pSkinInfo)
		return S_OK;

	pDerivedMeshContainer->pSkinInfo = pSkinInfo;
	pDerivedMeshContainer->pSkinInfo->AddRef();

	//pDerivedMeshContainer->MeshData.pMesh->CloneMeshFVF(pDerivedMeshContainer->MeshData.pMesh->GetOptions(), pDerivedMeshContainer->MeshData.pMesh->GetFVF(), m_pGraphicDev, &pDerivedMeshContainer->pOriMesh);
	pDerivedMeshContainer->MeshData.pMesh->CloneMesh(pDerivedMeshContainer->MeshData.pMesh->GetOptions(), vertexDecl, m_pGraphicDev, &pDerivedMeshContainer->pOriMesh);
	// 뼈의 개수를 반환
	pDerivedMeshContainer->dwNumBones = pDerivedMeshContainer->pSkinInfo->GetNumBones();

	pDerivedMeshContainer->pFrameOffSetMatrix = new _matrix[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->pFrameOffSetMatrix, sizeof(_matrix) * pDerivedMeshContainer->dwNumBones);

	pDerivedMeshContainer->ppCombinedTransformMatrix = new _matrix*[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->ppCombinedTransformMatrix, sizeof(_matrix*) * pDerivedMeshContainer->dwNumBones);

	pDerivedMeshContainer->pRenderingMatrix = new _matrix[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->pRenderingMatrix, sizeof(_matrix) * pDerivedMeshContainer->dwNumBones);

	for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
		pDerivedMeshContainer->pFrameOffSetMatrix[i] = *pDerivedMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
	
	*ppNewMeshContainer = pDerivedMeshContainer;

	m_pGraphicDev->CreateVertexDeclaration(vertexDecl, &pDerivedMeshContainer->pDecl);
	Safe_Release(pMesh);
	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	if (nullptr == pFrameToFree)
		return S_OK;
	Safe_Delete_Array(pFrameToFree->Name);

	if (nullptr != pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	if (nullptr != pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	if (nullptr != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	Safe_Delete(pFrameToFree);

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;

	for (_ulong i = 0; i < pDerivedMeshContainer->NumMaterials; ++i)
	{
		Safe_Release(pDerivedMeshContainer->ppTexture[i]);

		if (nullptr != pDerivedMeshContainer->ppNormalTextures[i])
			Engine::Safe_Release(pDerivedMeshContainer->ppNormalTextures[i]);

		//if (nullptr != pDerivedMeshContainer->ppEmissiveTextures[i])
		//	Engine::Safe_Release(pDerivedMeshContainer->ppEmissiveTextures[i]);

	}
	Safe_Delete_Array(pDerivedMeshContainer->ppTexture);
	Engine::Safe_Delete_Array(pDerivedMeshContainer->ppNormalTextures);
	//Engine::Safe_Delete_Array(pDerivedMeshContainer->ppEmissiveTextures);
	Safe_Delete_Array(pDerivedMeshContainer->Name);
	Safe_Delete_Array(pDerivedMeshContainer->pAdjacency);
	Safe_Delete_Array(pDerivedMeshContainer->pMaterials);

	Safe_Delete_Array(pDerivedMeshContainer->pFrameOffSetMatrix);
	Safe_Delete_Array(pDerivedMeshContainer->ppCombinedTransformMatrix);
	Safe_Delete_Array(pDerivedMeshContainer->pRenderingMatrix);

	Safe_Release(pDerivedMeshContainer->pDecl);

	Safe_Release(pDerivedMeshContainer->pOriMesh);
	Safe_Release(pDerivedMeshContainer->MeshData.pMesh);
	Safe_Release(pDerivedMeshContainer->pSkinInfo);
	
	Safe_Delete(pDerivedMeshContainer);

	return S_OK;
}

void Engine::CHierarchyLoader::Allocate_Name(char** ppName, const char* pFrameName)
{
	if (nullptr == pFrameName)
		return;

	_uint	iLength = strlen(pFrameName);

	*ppName = new char[iLength + 1];

	strcpy_s(*ppName, iLength + 1, pFrameName);
}

Engine::CHierarchyLoader* Engine::CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath)
{
	return new CHierarchyLoader(pGraphicDev, pPath);
}

Engine::_ulong Engine::CHierarchyLoader::Release(void)
{
	Safe_Release(m_pGraphicDev);

	delete this;

	return 0;
}

