#include "stdafx.h"
#include "Loading.h"

#include "Export_Function.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(m_szLoading, sizeof(_tchar) * 256);
}

CLoading::~CLoading(void)
{

}

unsigned int CALLBACK CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;
	
	_uint		iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;

	case LOADING_BOSS:
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());

	//_endthreadex(0);

	return iFlag;
}

HRESULT CLoading::Ready_Loading(LOADINGID eLoading)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_eLoading = eLoading;
	
	return 0;
}

Engine::_uint CLoading::Loading_ForStage(void)
{
	lstrcpy(m_szLoading, L"Loading Buffer...............");
	
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Buffer_TerrainTex", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Buffer_CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
		
	lstrcpy(m_szLoading, L"Loading Texture...............");
	
	//FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Terrain", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Terrain/Grass_%d.tga", TEX_NORMAL, 2)), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_SkyBox", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/SkyBox/burger%d.dds", TEX_CUBE, 4)), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Effect", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Explosion/Explosion%d.png", TEX_NORMAL, 90)), E_FAIL);
//	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_UI", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/hpbar.png", TEX_NORMAL, 1)), E_FAIL);


#pragma region UI Texture
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Element",				CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Element/Element_0%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Element_Base",			CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI//Element/Base_0%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Skill",					CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Skill/Skill_0%d.png", TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Skill_Base",			CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Skill/Base.png", TEX_NORMAL,1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Skill_Select",			CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Skill/Select.png", TEX_NORMAL, 1)), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_StatStone_Health",		CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/StatStone/Health_0%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_StatStone_Skill",		CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/StatStone/SkillGage_0%d.png", TEX_NORMAL, 4)), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_CoreTree_Base",			CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/CoreTree/Base_0%d.png", TEX_NORMAL,3)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_COreTree_Select",		CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/CoreTree/Select.png", TEX_NORMAL, 1)), E_FAIL);
	
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_CoreTree_StoneBase",	CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Stone/Base_0%d.png", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_CoreTree_StoneElement", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Stone/StoneElement_0%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_CoreTree_Creature",		CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Stone/Creature_0%d.png", TEX_NORMAL, 12)), E_FAIL);


#pragma endregion UI Texture



	lstrcpy(m_szLoading, L"Loading Component...............");
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Optimization", COptimization::Create(m_pGraphicDev, true, VTXCNTX, VTXCNTZ)), E_FAIL);

	// 원본을 추가하더라도 충돌체마다 정점의 위치 값이 다를 수 있고, 버텍스의 개수와 간격 또한 다를 수 있기 때문에 복제를 하는 것이 무의미할 수도 있다.
	// FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Collider", CCollider::Create(m_pGraphicDev, nullptr, 0, 0)), E_FAIL);

	lstrcpy(m_szLoading, L"Loading Mesh...............");

	//FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Mesh_Stone", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/StaticMesh/TombStone/", L"TombStone.x")), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Mesh_Navi", CNaviMesh::Create(m_pGraphicDev)), E_FAIL);
//	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Mesh_Player", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/PlayerXfile/", L"Player.x")), E_FAIL);
//	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Mesh_Sword", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/Sword/", L"Sword.x")), E_FAIL);
#pragma region DYNAMICMESH
	FAILED_CHECK_RETURN(Ready_Prototype(L"War", CDynamicMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/DynamicMesh/War/", L"War.X")), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Prototype(L"WaterBoss", CDynamicMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/DynamicMesh/WaterBoss/", L"WaterBoss.X")), E_FAIL);
#pragma endregion DYNAMICMESH

#pragma region STATICMESH	
	//FAILED_CHECK_RETURN(Ready_Prototype(L"TombStone", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/StaticMesh/TombStone/", L"TombStone.x")), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Prototype(L"Tree01", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/StaticMesh/Tree/", L"Tree01.X")), E_FAIL);
#pragma region STATICMESH

	lstrcpy(m_szLoading, L"Loading Complete!!!!");

	m_bFinish = true;

	return 0;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading*	pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eID)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLoading::Free(void)
{
	Safe_Release(m_pGraphicDev);
	
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);
}

