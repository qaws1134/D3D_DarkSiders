#include "stdafx.h"
#include "UIMgr.h"
#include "GameMgr.h"
#include "Export_Function.h"

IMPLEMENT_SINGLETON(CUIMgr)


CUIMgr::CUIMgr(void)
{

}
CUIMgr::~CUIMgr(void)
{

}


void CUIMgr::InitWeaponElement(LPDIRECT3DDEVICE9 pGraphicDev)
{

	CGameObject* pGameObject= nullptr;

	_float fUISize = 90.f;
	_float fBaseUISize = 600.f;

	_float fBaseZ = 0.3f;
	_float fArrowZ = 0.25f;
	_float fElementZ = 0.2f;

	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5), _float(WINCY*0.5)), _vec2(fBaseUISize, fBaseUISize), 0, L"Proto_Texture_Element_Base", L"UI_Element_Base"),false);
	pGameObject->SetZPos(fBaseZ, ID_DYNAMIC);
	m_listWeaponElement.emplace_back(pGameObject);



	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5), _float(WINCY*0.5)), _vec2(fBaseUISize, fBaseUISize), 0, L"Proto_Texture_Element_Arrow", L"UI_Element_Arrow"), false);
	pGameObject->SetZPos(fBaseZ, ID_DYNAMIC);
	m_listWeaponElement.emplace_back(pGameObject);
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5), _float(WINCY*0.5)), _vec2(fBaseUISize, fBaseUISize), 0, L"Proto_Texture_Element_Arrow_Bg", L"UI_Element_Arrow_Bg"), false);
	pGameObject->SetZPos(fBaseZ, ID_DYNAMIC);
	m_listWeaponElement.emplace_back(pGameObject);


	//원 6각형으로 나눠서 더해주자 
	_float fRadius = 240.f;

	_uint iIndex = 0;
	_float fAngle = 60.f*(_float)iIndex;

	_vec2 Dir = { cosf(D3DXToRadian(fAngle))*fRadius,-sinf(D3DXToRadian(fAngle))*fRadius };
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5)+ Dir.x, _float(WINCY*0.5)+ Dir.y), _vec2(fUISize, fUISize), 0,L"Proto_Texture_Element",L"UI_Elemet_Earth"), false);
	pGameObject->SetZPos(fElementZ, ID_DYNAMIC);
	m_listWeaponElement.emplace_back(pGameObject);
	
	iIndex++;
	fAngle = 60.f*(_float)iIndex;

	Dir = { cosf(D3DXToRadian(fAngle))*fRadius,-sinf(D3DXToRadian(fAngle))*fRadius };
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5)+ Dir.x, _float(WINCY*0.5)+ Dir.y), _vec2(fUISize, fUISize), 1, L"Proto_Texture_Element", L"UI_Elemet_Fire"), false);
	pGameObject->SetZPos(fElementZ, ID_DYNAMIC);
	m_listWeaponElement.emplace_back(pGameObject);
	
	iIndex++;
	fAngle = 60.f*(_float)iIndex;
	Dir = { cosf(D3DXToRadian(fAngle))*fRadius,-sinf(D3DXToRadian(fAngle))*fRadius };
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5)+ Dir.x, _float(WINCY*0.5)+ Dir.y), _vec2(fUISize, fUISize), 2, L"Proto_Texture_Element", L"UI_Elemet_Lightning"), false);
	pGameObject->SetZPos(fElementZ, ID_DYNAMIC);
	m_listWeaponElement.emplace_back(pGameObject);

	iIndex++;
	fAngle = 60.f*(_float)iIndex;
	Dir = { cosf(D3DXToRadian(fAngle))*fRadius,-sinf(D3DXToRadian(fAngle))*fRadius };
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5)+ Dir.x, _float(WINCY*0.5)+ Dir.y), _vec2(fUISize, fUISize), 3, L"Proto_Texture_Element", L"UI_Elemet_Death"), false);
	pGameObject->SetZPos(fElementZ, ID_DYNAMIC);
	m_listWeaponElement.emplace_back(pGameObject);

	iIndex++;
	fAngle = 60.f*(_float)iIndex;
	Dir = { cosf(D3DXToRadian(fAngle))*fRadius,-sinf(D3DXToRadian(fAngle))*fRadius };
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5)+ Dir.x, _float(WINCY*0.5)+ Dir.y), _vec2(fUISize, fUISize), 4, L"Proto_Texture_Element", L"UI_Elemet_Wind"), false);
	pGameObject->SetZPos(fElementZ, ID_DYNAMIC);
	m_listWeaponElement.emplace_back(pGameObject);
	
	iIndex++;
	fAngle = 60.f*(_float)iIndex;
	Dir = { cosf(D3DXToRadian(fAngle))*fRadius,-sinf(D3DXToRadian(fAngle))*fRadius };
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5)+ Dir.x, _float(WINCY*0.5)+ Dir.y), _vec2(fUISize, fUISize), 5, L"Proto_Texture_Element", L"UI_Elemet_Vamp"), false);
	pGameObject->SetZPos(fElementZ, ID_DYNAMIC);
	m_listWeaponElement.emplace_back(pGameObject);
	//해당 리스트랑 마우스랑 충돌체크해서 충돌한 오브젝트의 태그로 알아보고 상호작용 ㄱ? ㄱㅊ은데?

	m_mapHead.emplace(UI::LISTKEY_WEAPONELEMENT,m_listWeaponElement);
}

//UIType을 줘서 해당 랜더 타겟으로 출력 
void CUIMgr::InitCoretree(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGameObject* pGameObject = nullptr;

	_float fBgUISize = WINCX;
	_float fBaseUISize = 500.f;
	_float fSelUISize = 110.f;
	_float fStoneUISize = 90.f;
	_float fInfoUISize = 200.f;

	_float fBgZ = 0.6f;
	_float fBaseZ = 0.5f;
	_float fSelZ = 0.45f;
	_float fStoneBaseZ = 0.4f;
	_float fInfoZ = 0.2f;

	_bool  bActive = false;


	//피킹해서 오브젝트 태그에 셀을 옮겨주잦

	//커서 추가
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(GetCoretreePos(0), _vec2(fSelUISize, fSelUISize), 0, L"Proto_Texture_CoreTree_Sel", L"UI_CoreTree_Sel"), bActive);
	pGameObject->SetZPos(fSelZ, ID_DYNAMIC);
	m_listCoreTree.emplace_back(pGameObject);

	//배경 추가 
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5), _float(WINCY*0.5)), _vec2(fBgUISize, fBgUISize), 3, L"Proto_Texture_CoreTree_Bg", L"UI_CoreTree_Bg"), bActive);
	pGameObject->SetZPos(fBgZ, ID_DYNAMIC);
	m_listCoreTree.emplace_back(pGameObject);


	//베이스 
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5), _float(WINCY*0.5)-100.f), _vec2(fBaseUISize+300.f, fBaseUISize), 0, L"Proto_Texture_CoreTree_Base", L"UI_CoreTree_Base"), bActive);
	pGameObject->SetZPos(fBaseZ, ID_DYNAMIC);
	m_listCoreTree.emplace_back(pGameObject);

	//돌 칸 
	for (_uint i = 0; i < 19; i++)
	{
		if (0 == i ||1 == i ||
			8 == i || 16 == i || 17 == i  )
		{
			pGameObject = CUI::Create(pGraphicDev, Set_UISET(GetCoretreePos(i), _vec2(fStoneUISize, fStoneUISize), 1, L"Proto_Texture_CoreTree_Core", L"UI_CoreTree_Core" + to_wstring(i)), bActive);

		}
		else
			pGameObject = CUI::Create(pGraphicDev, Set_UISET(GetCoretreePos(i), _vec2(fStoneUISize, fStoneUISize), 0, L"Proto_Texture_CoreTree_Core", L"UI_CoreTree_Core"+ to_wstring(i)), bActive);
		//조건문 변경 쉐이더 변경 
		//dynamic_cast<CUI*>(pGameObject)->SetSubTex1(L"Proto_Texture_CoreTree_StoneElement",0);
		
		pGameObject->SetZPos(fStoneBaseZ, ID_DYNAMIC);
		m_listCoreTree.emplace_back(pGameObject);
	}


	m_mapHead.emplace(UI::LISTKEY_CORETREE,m_listCoreTree);
}

void CUIMgr::InitCoreList(LPDIRECT3DDEVICE9 pGraphicDev)
{

	CGameObject* pGameObject = nullptr;

	_float fBgSize = 600.f;
	_float fListSize = 450.f;
	_float fSize = 50.f;

	_float fBgZ = 0.6f;
	_float fBaseZ = 0.5f;
	_float flistBgZ = 0.4f;
	_float flistZ = 0.3f;
	_float fSelMaskZ = 0.25f;
	_float fSelZ = 0.2f;
	_float fCoreZ = 0.19f;

	_bool bActive = true;

	//배경 ㅇㅇ 
	pGameObject = CUI::Create(pGraphicDev, 
		Set_UISET(_vec2(_float(WINCX*0.5), _float(WINCY*0.5)), _vec2(WINCX*1.2f, WINCY*1.2f),
			0, L"Proto_Texture_CoreTree_StoneList_Base", L"UI_CoreTree_StoneList_Bg"), bActive);
	pGameObject->SetZPos(fBgZ, ID_DYNAMIC);
	m_listCoreList.emplace_back(pGameObject);


	////베이스 배경
	pGameObject = CUI::Create(pGraphicDev,
		Set_UISET(_vec2(_float(WINCX*0.5), _float(WINCY*0.5)), _vec2(WINCX*0.98f, WINCY*0.8f),
			2, L"Proto_Texture_CoreTree_StoneList_Base", L"UI_CoreTree_StoneList_ListInfoBg"), bActive);
	pGameObject->SetZPos(fBaseZ, ID_DYNAMIC);
	m_listCoreList.emplace_back(pGameObject);

	//리스트 인포 배경
	//내려갈수록 알파 옅게
	pGameObject = CUI::Create(pGraphicDev,
		Set_UISET(_vec2(940.f, 380.f), _vec2(fListSize, fListSize+130.f),
			0, L"Proto_Texture_CoreTree_StoneList_Base", L"UI_CoreTree_StoneList_ListBg"), bActive);
	pGameObject->SetZPos(flistBgZ, ID_DYNAMIC); 
	m_listCoreList.emplace_back(pGameObject);


	//////리스트 
	//pGameObject = CUI::Create(pGraphicDev,
	//	Set_UISET(_vec2(380.f, 180.f), _vec2(fSize + 600.f, fSize),
	//		0, L"Proto_Texture_List", L"UI_CoreTree_StoneList_SelList"), bActive);
	//pGameObject->SetZPos(fSelZ, ID_DYNAMIC);
	//m_listCoreList.emplace_back(pGameObject);


	////셀 
	////여기에 태두리 쉐이더 추가 
	//pGameObject = CUI::Create(pGraphicDev,
	//	Set_UISET(_vec2(380.f, 180.f), _vec2(fSize+600.f, fSize),
	//		1, L"Proto_Texture_CoreTree_StoneList_Sel", L"UI_CoreTree_StoneList_Sel1"), bActive);
	//pGameObject->SetZPos(fSelZ, ID_DYNAMIC);
	//m_listCoreList.emplace_back(pGameObject);



	////리스트를 열었을 때 
	////가지고 있는 애들을 출력해줘야하네 일단 출력 ㄱㄱ
	////아이콘?
	////아이템 먹으면 게임매니져에서 미리 생성해두고 해당 위치에 출

	////셀 왼쪽에 
	////선택되면 택스쳐 1로 변경 
	////렉트를 셀크기로 잡아줌 
	//pGameObject = CUI::Create(pGraphicDev,
	//	Set_UISET(_vec2(80.f, 180.f), _vec2(fSize, fSize),
	//		0, L"Proto_Texture_CoreTree_StoneList_Sel", L"UI_CoreTree_StoneList_SelBar"), bActive);
	//pGameObject->SetZPos(fSelZ, ID_DYNAMIC);
	//m_listCoreList.emplace_back(pGameObject);




	////이건 보석 리스트를 만들어놓고 뽑아오자
	////보석 UI를 미리 만들어놓고 정보들 뽑아오기 

	////크리쳐
	//pGameObject = CUI::Create(pGraphicDev,
	//	Set_UISET(_vec2(95.f, 180.f), _vec2(fSize, fSize),
	//		0, L"Proto_Texture_CoreTree_Creature", L"UI_CoreTree_StoneList_Creature"), bActive);
	//pGameObject->SetZPos(fCoreZ, ID_DYNAMIC);
	//m_listCoreList.emplace_back(pGameObject);

	////타입
	//pGameObject = CUI::Create(pGraphicDev,
	//	Set_UISET(_vec2(600.f, 180.f), _vec2(fSize, fSize),
	//		0, L"Proto_Texture_CoreTree_StoneElement", L"UI_CoreTree_StoneList_Element"), bActive);
	//pGameObject->SetZPos(fCoreZ, ID_DYNAMIC);
	//m_listCoreList.emplace_back(pGameObject);

	//이름까지 


	//오른쪽 리스트UI에 보석
	// 보석


	m_mapHead.emplace(UI::LISTKEY_CORELIST,m_listCoreList);
}

void CUIMgr::InitPlayerInfo(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGameObject* pGameObject = nullptr;

	_float fFortraitSize = 100.f;
	_float fLineSize = 500.f;
	_float fTypeSize = 40.f;
	_float fInfoUISize = 200.f;

	_float fUIZ = 0.18f;
	_float fInfoZ = 0.2f;
	_float fBgUIZ = 0.3f;
	_bool  bActive = true;

	//Bg 
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5), _float(WINCY*0.5) + 355.f), _vec2(_float(WINCX), _float(WINCY*0.4f)), 0, L"Proto_Texture_InfoBg", L"UI_Info_Bg"), bActive);
	pGameObject->SetZPos(fBgUIZ, ID_DYNAMIC);
	m_listPlayerInfo.emplace_back(pGameObject);

	//초상화
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(1120.f, 630.f), _vec2(fFortraitSize, fFortraitSize), 0, L"Proto_Texture_Portrait", L"UI_Info_Portrait"), bActive);
	pGameObject->SetZPos(fUIZ, ID_DYNAMIC);
	m_listPlayerInfo.emplace_back(pGameObject);


	//라인
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(1000.f, 670.f), _vec2(fLineSize, 5.f), 0, L"Proto_Texture_UnderBar", L"UI_Info_UnderBar"), bActive);
	pGameObject->SetZPos(fUIZ, ID_DYNAMIC);
	m_listPlayerInfo.emplace_back(pGameObject);

	//공격, 진노력, 체력 
	for (_uint i = 0; i < 3; i++)
	{
		pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(780.f, 690.f+(40.f*i)), _vec2(fTypeSize, fTypeSize), (i*2)+1, L"Proto_Texture_CoreTree_StoneElement", L"UI_Info_Element"+ to_wstring(i)), bActive);
		pGameObject->SetZPos(fUIZ, ID_DYNAMIC);
		m_listPlayerInfo.emplace_back(pGameObject);
	}

	//설명 추가 
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5) - 220.f, _float(WINCY*0.5) + 305.f), _vec2(_float(WINCX*0.6f), fInfoUISize), 0, L"Proto_Texture_CoreTree_Info", L"UI_CoreTree_Info"), bActive);
	pGameObject->SetZPos(fInfoZ, ID_DYNAMIC);
	m_listPlayerInfo.emplace_back(pGameObject);

	m_mapHead.emplace(UI::LISTKEY_PLAYERINFO,m_listPlayerInfo);
}

void CUIMgr::InitStoneList(LPDIRECT3DDEVICE9 pGraphicDev)
{
	vector<STONE> vecStone;
	vecStone = CGameMgr::GetInstance()->GetStoneVec();
	for (auto iter : vecStone)
	{
		//SetStoneInfoUI(pGraphicDev, iter);
		//SetStoneListUI(pGraphicDev, iter.eCreature);
	}
}

//획득했을때 스톤이 있는지 확인하고 
//-> 플레이어에서 vector enum으로 저장하자 
// 없으면 추가하면서 UI Create 
//획득할때마다 UI 생성
//리스트 스크롤에서 액티브 해주고 스크롤로 포지션 올리거나 내리기
void CUIMgr::SetStoneInfoUI(LPDIRECT3DDEVICE9 pGraphicDev,STONE tStone) //순서대로 아래로 생성 열때마다 맨위로 조정 
{
	//2. 획득할때마다 셋팅-> 이게 맞는듯?
	CGameObject* pGameObject = nullptr;

	_float fStoneSize = 90.f;
	_float fIconSize = 50.f;


	_float fSelZ = 0.2f;
	_float fStoneZ = 0.2f;
	_float fCoreZ = 0.19f;
	_bool bActive =true;
	


	//오른쪽 설명 리스트에 스톤 ->빤짝 쉐이더 먹인 스톤 
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(780.f, 150.f), _vec2(fStoneSize, fStoneSize), (_uint)tStone.eCreature, L"Proto_Texture_CoreTree_Creature", L"UI_Info_Element" + to_wstring(m_StoneListIdx)), bActive);
	pGameObject->SetZPos(fStoneZ, ID_DYNAMIC);
	dynamic_cast<CUI*>(pGameObject)->SetShade(UI::SHADE_UVMOVE);

	m_listStoneInfo.emplace_back(pGameObject);
	

	//대문짝폰트
	//미들폰트

	//스몰폰트

	//이름까지 


	for (auto iter : m_listStoneInfo)
	{
		Add_GameObject(L"UI", dynamic_cast<CUI*>(iter)->GetObjTag().c_str(), iter);
	}

	m_mapStoneList.emplace(m_StoneListIdx, m_listStoneInfo);
	m_listStoneInfo.clear();
}

void CUIMgr::SetStoneListUI(LPDIRECT3DDEVICE9 pGraphicDev, UI::STONE eStone)
{
	CGameObject* pGameObject = nullptr;

	_float fStoneSize = 90.f;
	_float fIconSize = 50.f;

	_float fPosy = 180.f + (60.f*m_StoneListIdx);
	_float fSelZ = 0.2f;
	_float fStoneZ = 0.2f;
	_float fCoreZ = 0.19f;
	_bool bActive = true;

	//셀 bar
	pGameObject = CUI::Create(pGraphicDev,
		Set_UISET(_vec2(80.f, fPosy), _vec2(fIconSize, fIconSize),
			0, L"Proto_Texture_CoreTree_StoneList_Sel", L"UI_CoreTree_StoneList_SelBar"+to_wstring(m_StoneListIdx)), bActive);
	pGameObject->SetZPos(fSelZ, ID_DYNAMIC);
	m_listStoneList.emplace_back(pGameObject);
	//Add_GameObject();

	//이건 보석 리스트를 만들어놓고 뽑아오자
	//보석 UI를 미리 만들어놓고 정보들 뽑아오기 

	//크리쳐
	pGameObject = CUI::Create(pGraphicDev,
		Set_UISET(_vec2(95.f, fPosy), _vec2(fIconSize, fIconSize),
			(_uint)eStone, L"Proto_Texture_CoreTree_Creature", L"UI_CoreTree_StoneList_Creature" + to_wstring(m_StoneListIdx)), bActive);
	pGameObject->SetZPos(fCoreZ, ID_DYNAMIC);
	m_listStoneList.emplace_back(pGameObject);

	//타입
	pGameObject = CUI::Create(pGraphicDev,
		Set_UISET(_vec2(600.f, fPosy), _vec2(fIconSize, fIconSize),
			0, L"Proto_Texture_CoreTree_StoneElement", L"UI_CoreTree_StoneList_Element" + to_wstring(m_StoneListIdx)), bActive);
	pGameObject->SetZPos(fCoreZ, ID_DYNAMIC);
	m_listStoneList.emplace_back(pGameObject);

	for (auto iter : m_listStoneList)
	{
		Add_GameObject(L"UI", dynamic_cast<CUI*>(iter)->GetObjTag().c_str(), iter);
	}

	m_mapStoneList.emplace(m_StoneListIdx, m_listStoneList);
	m_listStoneList.clear();
	m_StoneListIdx++;
}




map<UI::LISTKEY,list<CGameObject*>> CUIMgr::InitCreateUI(LPDIRECT3DDEVICE9 pGraphicDev)
{
	InitWeaponElement(pGraphicDev);
	InitCoretree(pGraphicDev);
	InitCoreList(pGraphicDev);
	InitPlayerInfo(pGraphicDev);
	InitStoneList(pGraphicDev);
	return m_mapHead;
}

UISET CUIMgr::Set_UISET(_vec2 vPos, _vec2 vSize, _uint iTextureNum, wstring wstrTexture, wstring wstrObjTag)
{
	UISET tInfo;
	tInfo.vPos = vPos;
	tInfo.vSize = vSize;
	tInfo.iTextureNum = iTextureNum;
	tInfo.wstrTexture = wstrTexture.c_str();
	tInfo.wstrObjTag = wstrObjTag.c_str();
	return tInfo;
}


void CUIMgr::SetActiveElementUI(_bool bActive)
{
	for (auto iter : m_listWeaponElement)
	{
		dynamic_cast<CUI*>(iter)->SetActive(bActive);
	}
}

void CUIMgr::SetActiveCoreTreeUI(_bool bActive)
{
	for (auto iter : m_listCoreTree)
	{
		dynamic_cast<CUI*>(iter)->SetActive(bActive);
	}
	for (auto iter : m_listPlayerInfo)
	{
		dynamic_cast<CUI*>(iter)->SetActive(bActive);
	}
}

_bool CUIMgr::GetElemetUIActive()
{
	return m_listWeaponElement.front()->GetActive();
}

_bool CUIMgr::GetCoreTreeUIActive()
{
	return m_listCoreTree.front()->GetActive();
}


void CUIMgr::Free(void)
{
	for (auto iter : m_listWeaponElement)
		Safe_Release(iter);
	for (auto iter : m_listCoreTree)
		Safe_Release(iter);
	for (auto iter : m_listPlayerInfo)
		Safe_Release(iter);
	for (auto iter : m_listStoneInfo)
		Safe_Release(iter);

	for (auto iter : m_mapStoneList)
	{
		for (auto iter_second : iter.second)
		{
			Safe_Release(iter_second);
		}
	}
	for (auto iter : m_mapStoneInfo)
	{
		for (auto iter_second : iter.second)
		{
			Safe_Release(iter_second);
		}
	}
}


_vec2 CUIMgr::GetCoretreePos(_uint iIdx)
{
	_vec2 vPos;
	switch (iIdx)
	{
	case 0:
		vPos.x = 430.f;
		vPos.y = 45.f;
		break;
	case 1:
		vPos.x = 770.f;
		vPos.y = 45.f;
		break;
	case 2:
		vPos.x = 230.f;
		vPos.y = 180.f;
		break;
	case 3:
		vPos.x = 430.f;
		vPos.y = 165.f;
		break;
	case 4:
		vPos.x = 770.f;
		vPos.y = 165.f;
		break;
	case 5:
		vPos.x = 970.f;
		vPos.y = 180.f;
		break;
	case 6:
		vPos.x = 230.f;
		vPos.y = 295.f;
		break;
	case 7:
		vPos.x = 430.f;
		vPos.y = 295.f;
		break;
	case 8:
		vPos.x = 600.f;
		vPos.y = 295.f;

		break;
	case 9:
		vPos.x = 770.f;
		vPos.y = 295.f;
		break;
	case 10:
		vPos.x = 970.f;
		vPos.y = 295.f;
		break;
	case 11:
		vPos.x = 230.f;
		vPos.y = 430.f;
		break;
	case 12:
		vPos.x = 430.f;
		vPos.y = 430.f;
		break;
	case 13:
		vPos.x = 770.f;
		vPos.y = 430.f;
		break;
	case 14:
		vPos.x = 970.f;
		vPos.y = 430.f;
		break;
	case 15:
		vPos.x = 230.f;
		vPos.y = 540.f;
		break;
	case 16:
		vPos.x = 430.f;
		vPos.y = 550.f;
		break;
	case 17:
		vPos.x = 770.f;
		vPos.y = 550.f;
		break;
	case 18:
		vPos.x = 970.f;
		vPos.y = 540.f;
		break;
	default:
		break;
	}

	return vPos;
}
