#include "DynamicMesh.h"

USING(Engine)


Engine::CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pLoader(nullptr)
	, m_pAniCtrl(nullptr)
{

}

Engine::CDynamicMesh::CDynamicMesh(const CDynamicMesh& rhs)
	: CComponent(rhs)
	, m_pRootFrame(rhs.m_pRootFrame)
	, m_pLoader(rhs.m_pLoader)
	, m_MeshContainerList(rhs.m_MeshContainerList)
	, m_mapBoneName(rhs.m_mapBoneName)
	, m_matParent(rhs.m_matParent)
{


	m_pAniCtrl = CAniCtrl::Create(*rhs.m_pAniCtrl);
}

Engine::CDynamicMesh::~CDynamicMesh(void)
{

}

void CDynamicMesh::Set_AnimationIndex(const _uint & iIndex,_bool bBlend, _double dBlendTime)
{
	
	m_pAniCtrl->Set_AnimationIndex(iIndex, bBlend);
}

void CDynamicMesh::Play_Animation(const _float & fTimeDelta)
{
	m_pAniCtrl->Play_Animation(fTimeDelta);
	

	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixRotationY(&m_matParent, D3DXToRadian(270.f)));
}

Engine::_bool Engine::CDynamicMesh::Is_AnimationsetFinish(void)
{
	return m_pAniCtrl->Is_AnimationsetFinish();
}

_bool CDynamicMesh::Is_Animationset(_double dRadius)
{
	return m_pAniCtrl->Is_Animationset(dRadius);
}



_vec3 CDynamicMesh::GetBonePos(const char * pFrameName)
{
	_vec3 vBonePos;

	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);

	_matrix matBone = pFrame->CombinedTransformMatrix;
	vBonePos.x =  matBone._41;
	vBonePos.y = matBone._42;
	vBonePos.z = matBone._43;

	return vBonePos;
}

const			Engine::D3DXFRAME_DERIVED* Engine::CDynamicMesh::Get_FrameByName(const char* pFrameName)
{
	return (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
}



HRESULT Engine::CDynamicMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar	szFullPath[MAX_PATH] = L"";
	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(m_pGraphicDev, pFilePath);
	NULL_CHECK_RETURN(m_pLoader, E_FAIL);

	LPD3DXANIMATIONCONTROLLER		pAniCtrl = nullptr;
	
	if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath, 
										D3DXMESH_MANAGED, 
										m_pGraphicDev,
										m_pLoader, // HierarchyLoader
										NULL, 
										&m_pRootFrame, 
										&pAniCtrl)))	// AniCtrl
										return E_FAIL;

	m_pAniCtrl = CAniCtrl::Create(pAniCtrl);
	NULL_CHECK_RETURN(m_pAniCtrl, E_FAIL);

	Safe_Release(pAniCtrl);

	D3DXMatrixRotationY(&m_matParent, D3DXToRadian(180.f));
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame,&m_matParent);

	SetUp_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame);

	return S_OK;

}

void Engine::CDynamicMesh::Render_Meshes(void)
{
	
	for (auto& iter : m_MeshContainerList)
	{
		D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = iter;

		for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
			pDerivedMeshContainer->pRenderingMatrix[i] = pDerivedMeshContainer->pFrameOffSetMatrix[i] 
														* *pDerivedMeshContainer->ppCombinedTransformMatrix[i];
		
		void*			pSrcVtx = nullptr; // 고정 불변의 메쉬 정점 정보
		void*			pDestVtx = nullptr; // 애니메이션 적용에 따른 변환된 메쉬 정점 정보

		pDerivedMeshContainer->pOriMesh->LockVertexBuffer(0, &pSrcVtx);
		pDerivedMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVtx);

		//소프트웨어 스키닝을 수행하는 함수(스키닝뿐 아니라 애니메이션 변경 시, 뼈와 정점들의 정보도 동시에 변경해줌)
		pDerivedMeshContainer->pSkinInfo->UpdateSkinnedMesh(pDerivedMeshContainer->pRenderingMatrix, NULL, pSrcVtx, pDestVtx);
		
		for (_ulong i = 0; i < pDerivedMeshContainer->NumMaterials; ++i)
		{
			m_pGraphicDev->SetTexture(0, pDerivedMeshContainer->ppTexture[i]);
			pDerivedMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
		//pDerivedMeshContainer->MeshData.pMesh->GetNumVertices();

		pDerivedMeshContainer->pOriMesh->UnlockVertexBuffer();
		pDerivedMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	}
	
	
}

void CDynamicMesh::Render_Meshes(LPD3DXEFFECT & pEffect)
{
	for (auto& iter : m_MeshContainerList)
	{
		D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = iter;

		for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
			pDerivedMeshContainer->pRenderingMatrix[i] = pDerivedMeshContainer->pFrameOffSetMatrix[i]
			* *pDerivedMeshContainer->ppCombinedTransformMatrix[i];

		void*			pSrcVtx = nullptr; // 고정 불변의 메쉬 정점 정보
		void*			pDestVtx = nullptr; // 애니메이션 적용에 따른 변환된 메쉬 정점 정보

		pDerivedMeshContainer->pOriMesh->LockVertexBuffer(0, &pSrcVtx);
		pDerivedMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVtx);

		//소프트웨어 스키닝을 수행하는 함수(스키닝뿐 아니라 애니메이션 변경 시, 뼈와 정점들의 정보도 동시에 변경해줌)
		pDerivedMeshContainer->pSkinInfo->UpdateSkinnedMesh(pDerivedMeshContainer->pRenderingMatrix, NULL, pSrcVtx, pDestVtx);

		for (_ulong i = 0; i < pDerivedMeshContainer->NumMaterials; ++i)
		{
			pEffect->SetTexture("g_BaseTexture", pDerivedMeshContainer->ppTexture[i]);
			pEffect->CommitChanges();
			
			pDerivedMeshContainer->MeshData.pMesh->DrawSubset(i);
		}

		pDerivedMeshContainer->pOriMesh->UnlockVertexBuffer();
		pDerivedMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	}
}

void Engine::CDynamicMesh::Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix)
{
	if (nullptr == pFrame)
		return;

	pFrame->CombinedTransformMatrix = pFrame->TransformationMatrix * (*pParentMatrix);


	if (nullptr != pFrame->pFrameSibling)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pParentMatrix);

	if (nullptr != pFrame->pFrameFirstChild)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, &pFrame->CombinedTransformMatrix);
}

Engine::CDynamicMesh* Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CDynamicMesh*	pInstance = new CDynamicMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
		Safe_Release(pInstance);

	return pInstance;
}


Engine::CComponent* Engine::CDynamicMesh::Clone(void)
{
	return new CDynamicMesh(*this);
}

void Engine::CDynamicMesh::Free(void)
{
	Safe_Release(m_pAniCtrl);

	if (false == m_bClone)
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
		Safe_Release(m_pLoader);
	}

	CComponent::Free();
}

//디라이브드 컨테이너 받아서
//뼈 복사

void Engine::CDynamicMesh::SetUp_FrameMatrices(D3DXFRAME_DERIVED* pFrame)
{
	USES_CONVERSION;

	if (nullptr != pFrame->pMeshContainer)
	{

		D3DXMESHCONTAINER_DERIVED*	pDerivedMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;
		map<_ulong, const char*> mapBone;
		//wstring wstrBoneName;
		//전체 뼈 - > 순회횟수
		//뼈 위치 저장한 값 덩어리 -> 컴퓨트 버텍스 값 
		for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
		{
			const char* pBoneName = pDerivedMeshContainer->pSkinInfo->GetBoneName(i);
			//wstrBoneName = A2W(pBoneName);
			D3DXFRAME_DERIVED* pDerivedFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);

			pDerivedMeshContainer->ppCombinedTransformMatrix[i] = &pDerivedFrame->CombinedTransformMatrix;
			mapBone.emplace(i, pBoneName);
		}
		//wstrBoneName = A2W(pFrame->Name);
		m_mapBoneName.emplace(pFrame->Name, mapBone);
		m_MeshContainerList.push_back(pDerivedMeshContainer);
	
	}


	if (nullptr != pFrame->pFrameSibling)
		SetUp_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);

	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);
}