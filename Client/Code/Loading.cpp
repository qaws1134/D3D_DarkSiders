#include "stdafx.h"
#include "Loading.h"
#include "LoadMgr.h"
#include "Export_Function.h"
#include "GameMgr.h"


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
	//FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_UI", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/hpbar.png", TEX_NORMAL, 1)), E_FAIL);


#pragma region UI Texture

	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_List", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/List.png", TEX_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_UnderBar", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/UnderBar.png", TEX_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_ToastBox", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/ToastBox.png", TEX_NORMAL, 1)), E_FAIL);


	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Store_Base", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Store/Bg_0%d.png", TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Store_Sel", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Store/Sel_0%d.png", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Store_SelFill", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Store/SelFill.png", TEX_NORMAL, 1)), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Store_Stone", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Store/Item_Stone_0%d.png", TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Store_Active", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Store/Item_Active_0%d.png", TEX_NORMAL, 11)), E_FAIL);
	
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Store_Active_Info", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Store/Active_Info_0%d.png", TEX_NORMAL, 5)), E_FAIL);
	
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Store_Soul", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Store/Soul.png", TEX_NORMAL, 1)), E_FAIL);


	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_InfoBg", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/InfoBg.tga", TEX_NORMAL, 1)), E_FAIL);

	//FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_FillBg", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Element/Element_0%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Portrait", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Portrait/Portrait_0%d.png", TEX_NORMAL, 4)), E_FAIL);


	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Element",				CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Element/Element_0%d.png", TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Element_Base",			CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Element/Base_0%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Element_Arrow",			CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Element/Arrow_Element_01.png", TEX_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Element_Arrow_Bg",		CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Element/Arrow_Element_00.png", TEX_NORMAL, 1)), E_FAIL);
		
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Skill",					CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Skill/Skill_0%d.png", TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Skill_Base",			CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Skill/Base.png", TEX_NORMAL,1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Skill_Select",			CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Skill/Select.png", TEX_NORMAL, 1)), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_StatStone_Health",		CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/StatStone/Health_0%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_StatStone_Skill",		CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/StatStone/SkillGage_0%d.png", TEX_NORMAL, 4)), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_CoreTree_Bg",			CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/CoreTree/Bg_0%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_CoreTree_Base",			CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/CoreTree/Base.png", TEX_NORMAL,1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_CoreTree_Core",			CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/CoreTree/Core_0%d.png", TEX_NORMAL, 2)), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_CoreTree_Sel",			CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/CoreTree/Select.png", TEX_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_CoreTree_Info",			CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/CoreTree/Info.png", TEX_NORMAL, 1)), E_FAIL);



	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_CoreTree_StoneList_Base",CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Stone/ListBase_0%d.png",TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_CoreTree_StoneList_Sel", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Stone/Sel_0%d.png", TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_CoreTree_StoneList_Seg", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Stone/Seg_0%d.png", TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_CoreTree_StoneList_Icon",CTexture::Create(m_pGraphicDev,L"../../Resource/Texture/UI/Stone/Icon_0%d.png", TEX_NORMAL, 2)), E_FAIL);

	
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_CoreTree_StoneBase",	CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Stone/Base_0%d.png", TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_CoreTree_StoneElement", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Stone/StoneElement_0%d.png", TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_CoreTree_Creature",		CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Stone/Creature_0%d.png", TEX_NORMAL, 22)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_CoreTree_StoneEffect",  CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/UI/Stone/StoneEffect.png", TEX_NORMAL, 1)), E_FAIL);


	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Texture_Effect_Lightning", CTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Effect/VFX_gen_bolts.tga", TEX_NORMAL, 1)), E_FAIL);



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
	FAILED_CHECK_RETURN(Ready_Prototype(L"WaterBoss", CDynamicMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/DynamicMesh/WaterBoss/", L"WaterBoss.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Chest", CDynamicMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/DynamicMesh/Chest/", L"Chest.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"PlayerBarrier", CDynamicMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/DynamicMesh/PlayerBarrier/", L"PlayerBarrier.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Grinner", CDynamicMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/DynamicMesh/Grinner/", L"Grinner.X")), E_FAIL);

	
	FAILED_CHECK_RETURN(Ready_Prototype(L"Goblin0", CDynamicMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/DynamicMesh/Goblin/", L"Goblin.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Goblin1", CDynamicMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/DynamicMesh/Goblin/", L"Goblin.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Goblin2", CDynamicMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/DynamicMesh/Goblin/", L"Goblin.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Goblin3", CDynamicMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/DynamicMesh/Goblin/", L"Goblin.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Goblin4", CDynamicMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/DynamicMesh/Goblin/", L"Goblin.X")), E_FAIL);

	
	FAILED_CHECK_RETURN(Ready_Prototype(L"Grinner0", CDynamicMesh::Create(CGameMgr::GetInstance()->GetDevice(), L"../../Resource/Mesh/DynamicMesh/Grinner/", L"Grinner.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Grinner1", CDynamicMesh::Create(CGameMgr::GetInstance()->GetDevice(), L"../../Resource/Mesh/DynamicMesh/Grinner/", L"Grinner.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Grinner2", CDynamicMesh::Create(CGameMgr::GetInstance()->GetDevice(), L"../../Resource/Mesh/DynamicMesh/Grinner/", L"Grinner.X")), E_FAIL);
#pragma endregion DYNAMICMESH

#pragma region STATICMESH	
	//FAILED_CHECK_RETURN(Ready_Prototype(L"TombStone", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/StaticMesh/TombStone/", L"TombStone.x")), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Prototype(L"Tree01", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/StaticMesh/Tree/", L"Tree01.X")), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Prototype(L"WaterBoss_Orb", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/StaticMesh/WaterBoss_Orb/", L"Orb.X")), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Prototype(L"WaterBoss_Bullet", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/StaticMesh/WaterBoss_Bullet/", L"WaterBoss_Bullet.X")), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Prototype(L"GoblinSpear", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/StaticMesh/GoblinSpear/", L"GoblinSpear.X")), E_FAIL);

	//맵
	for (_uint i = 0; i < 24; i++)
	{
		wstring wstrProto = L"Eden" + to_wstring(i);
		wstring wsrFile = L"Eden" + to_wstring(i) + L".X";
		const _tchar* pConvProtoTag = W2BSTR(wstrProto.c_str());
		const _tchar* pConvFileTag = W2BSTR(wsrFile.c_str());
		Ready_Prototype(pConvProtoTag, CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/StaticMesh/Eden/", pConvFileTag));
	}
	for (_uint i = 0; i < 2; i++)
	{
		wstring wstrProto = L"Eden_decal" + to_wstring(i);
		wstring wsrFile = L"Eden_decal" + to_wstring(i) + L".X";
		const _tchar* pConvProtoTag = W2BSTR(wstrProto.c_str());
		const _tchar* pConvFileTag = W2BSTR(wsrFile.c_str());
		Ready_Prototype(pConvProtoTag, CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/StaticMesh/Eden/", pConvFileTag));
	}
	Ready_Prototype(L"Eden_Edge", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/StaticMesh/Eden/", L"Eden_Edge.X"));

	for (_uint i = 0; i < 5; i++)
	{
		wstring wstrProto = L"Eden_Prop" + to_wstring(i);
		wstring wsrFile = L"Eden_Prop" + to_wstring(i) + L".X";
		const _tchar* pConvProtoTag = W2BSTR(wstrProto.c_str());
		const _tchar* pConvFileTag = W2BSTR(wsrFile.c_str());
		Ready_Prototype(pConvProtoTag, CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/StaticMesh/Eden_Prop/", pConvFileTag));
	}
	for (_uint i = 0; i < 2; i++)
	{
		wstring wstrProto = L"Eden_WaterFall" + to_wstring(i);
		wstring wsrFile = L"Eden_WaterFall" + to_wstring(i) + L".X";
		const _tchar* pConvProtoTag = W2BSTR(wstrProto.c_str());
		const _tchar* pConvFileTag = W2BSTR(wsrFile.c_str());
		Ready_Prototype(pConvProtoTag, CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/StaticMesh/WaterFall/", pConvFileTag));
	}

	

	//이펙트 

	for (_uint i = 0; i < 6; i++)
	{
		wstring wstrProto = L"Effect_Bolt" + to_wstring(i);
		wstring wsrFile = L"Effect_Bolt" + to_wstring(i) + L".X";
		const _tchar* pConvProtoTag = W2BSTR(wstrProto.c_str());
		const _tchar* pConvFileTag = W2BSTR(wsrFile.c_str());
		Ready_Prototype(pConvProtoTag, CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/StaticMesh/Effect/", pConvFileTag));
	}
	for (_uint i = 0; i < 3; i++)
	{
		wstring wstrProto = L"Ramp" + to_wstring(i);
		wstring wsrFile = L"Ramp" + to_wstring(i) + L".X";
		const _tchar* pConvProtoTag = W2BSTR(wstrProto.c_str());
		const _tchar* pConvFileTag = W2BSTR(wsrFile.c_str());
		Ready_Prototype(pConvProtoTag, CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/StaticMesh/Effect/", pConvFileTag));
	}
	for (_uint i = 0; i < 3; i++)
	{
		wstring wstrProto = L"StoneCluster" + to_wstring(i);
		wstring wsrFile = L"StoneCluster" + to_wstring(i) + L".X";
		const _tchar* pConvProtoTag = W2BSTR(wstrProto.c_str());
		const _tchar* pConvFileTag = W2BSTR(wsrFile.c_str());
		Ready_Prototype(pConvProtoTag, CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/StaticMesh/Effect/", pConvFileTag));
	}
	for (_uint i = 0; i < 2; i++)
	{
		wstring wstrProto = L"StoneSingle" + to_wstring(i);
		wstring wsrFile = L"StoneSingle" + to_wstring(i) + L".X";
		const _tchar* pConvProtoTag = W2BSTR(wstrProto.c_str());
		const _tchar* pConvFileTag = W2BSTR(wsrFile.c_str());
		Ready_Prototype(pConvProtoTag, CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/StaticMesh/Effect/", pConvFileTag));
	}


	for (_uint i = 0; i < 2; i++)
	{
		wstring wstrProto = L"Ghost" + to_wstring(i);
		wstring wsrFile = L"Ghost" + to_wstring(i) + L".X";
		const _tchar* pConvProtoTag = W2BSTR(wstrProto.c_str());
		const _tchar* pConvFileTag = W2BSTR(wsrFile.c_str());
		Ready_Prototype(pConvProtoTag, CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/StaticMesh/Effect/", pConvFileTag));
	}

	FAILED_CHECK_RETURN(Ready_Prototype(L"WaterFloor", CStaticMesh::Create(m_pGraphicDev, L"../../Resource/Mesh/StaticMesh/Effect/", L"WaterFloor.X")), E_FAIL);






#pragma region STATICMESH



	//오브젝트 풀 생성 
	//CGameMgr::GetInstance()->InitObjPool();
	CLoadMgr::GetInstance()->LoadData(L"../../Data/");
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

