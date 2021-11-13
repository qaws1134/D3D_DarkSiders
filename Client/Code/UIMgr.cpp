#include "stdafx.h"
#include "UIMgr.h"
#include "GameMgr.h"
#include "Export_Function.h"

IMPLEMENT_SINGLETON(CUIMgr)


CUIMgr::CUIMgr(void)
{
	m_fPreWheel = g_fWheel;
}
CUIMgr::~CUIMgr(void)
{
	Free();
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
			//레어돌
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

	_bool bActive = false;

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
	_bool  bActive = false;

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

	pGameObject = CUI::Create(pGraphicDev, UIFONT{ L"Font_L_Normal", L"W A R   P O W E R", _vec2(765.f, 615.f), D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f) }, bActive);
	dynamic_cast<CUI*>(pGameObject)->SetObjTag( L"PlayerInfo_Font_L_Normal" + to_wstring(10));
	m_listPlayerInfo.emplace_back(pGameObject);
	//공격, 진노력, 체력 
	for (_uint i = 0; i < 3; i++)
	{
		pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(780.f, 690.f+(40.f*i)), _vec2(fTypeSize, fTypeSize), (i*2), L"Proto_Texture_CoreTree_StoneElement", L"UI_Info_Element"+ to_wstring(i)), bActive);
		pGameObject->SetZPos(fUIZ, ID_DYNAMIC);
		m_listPlayerInfo.emplace_back(pGameObject);
		wstring wstrType;
		switch (i)
		{
		case 0:
			wstrType = L"ATTACK";
			break;
		case 1:	
			wstrType = L"WRATH";
			break;
		case 2:
			wstrType = L"HEALTH";
		default:
			break;
		}

		pGameObject = CUI::Create(pGraphicDev, UIFONT{ L"Font_L_Normal", wstrType.c_str(), _vec2(820.f,  680.f + (40.f*i)), D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f) }, bActive);
		dynamic_cast<CUI*>(pGameObject)->SetObjTag(L"PlayerInfo_Font_L_Normal" + to_wstring(i));
		m_listPlayerInfo.emplace_back(pGameObject);


	}

	//설명 배경 추가 
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5) - 220.f, _float(WINCY*0.5) + 305.f), _vec2(_float(WINCX*0.6f), fInfoUISize), 0, L"Proto_Texture_CoreTree_Info", L"UI_CoreTree_Info"), bActive);
	pGameObject->SetZPos(fInfoZ, ID_DYNAMIC);
	m_listPlayerInfo.emplace_back(pGameObject);


	m_mapHead.emplace(UI::LISTKEY_PLAYERINFO,m_listPlayerInfo);
}



void CUIMgr::InitStore(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGameObject* pGameObject = nullptr;
	_float fBaseZ = 0.5f;
	_float fInfoZ = 0.45f;

	_bool  bActive = false;

	//블랜드 처리 
	//Bg 
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5)+300.f, _float(WINCY*0.5)), _vec2(_float(WINCX)*0.5f, _float(WINCY)), 0, L"Proto_Texture_Store_Base", L"UI_Store_Base"), bActive);
	pGameObject->SetZPos(fBaseZ, ID_DYNAMIC);
	m_listStoreBase.emplace_back(pGameObject);

	//Info
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5) + 300.f, _float(WINCY*0.5)+300.f), _vec2(_float(WINCX)*0.5f-50.f, _float(WINCY)*0.3f-100.f), 2, L"Proto_Texture_Store_Base", L"UI_Store_Info"), bActive);
	pGameObject->SetZPos(fInfoZ, ID_DYNAMIC);
	m_listStoreBase.emplace_back(pGameObject);


	for (_uint i = 0; i < UI::ACTIVE_ITEM_END; i++)
	{
		if (i == 0)
		{
			//상점 폰트 출력 
		}
		InitStoreList(pGraphicDev, m_listStoreActiveList, (UI::ITEM)i);
	}
	m_iStoreIdx = 0;

	for (_uint i = (_uint)UI::ACTIVE_ITEM_END + 1; i < UI::ITEM_END; i++)
	{
		if (i == (_uint)UI::ACTIVE_ITEM_END + 1)
		{
			//상점 폰트 출력
		}
		InitStoreList(pGraphicDev, m_listStoreStoneList, (UI::ITEM) i);
	}
	for (auto iter : m_listStoreBase)
	{
		Add_GameObject(L"UI", dynamic_cast<CUI*>(iter)->GetObjTag().c_str(), iter);
	}

	m_mapHead.emplace(UI::LISTKEY_STOREBASE,m_listStoreBase);
}

void CUIMgr::InitStoreList(LPDIRECT3DDEVICE9 pGraphicDev,list<CGameObject*> listStore, UI::ITEM eItemIdx)
{
	CGameObject* pGameObject = nullptr;

	_float fListZ = 0.4f;
	_float fSelZ = 0.395f;
	_float fSelFillZ = 0.39f;


	_bool  bActive = false;
	wstring wstrListIdx= to_wstring(m_iStoreIdx)+to_wstring(eItemIdx) ;
	//리스트박스
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5) + 300.f,150.f+(m_iStoreIdx*90.f)), _vec2(520.f, 80.f), 0, L"Proto_Texture_List", L"UI_Store_List"+ wstrListIdx), bActive);
	pGameObject->SetZPos(fListZ, ID_DYNAMIC);
	listStore.emplace_back(pGameObject);

	//셀

	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5) + 90.f, 150.f + (m_iStoreIdx*90.f)), _vec2(100.f, 80.f), 0, L"Proto_Texture_Store_Sel", L"UI_Store_Sel" + wstrListIdx), bActive);
	pGameObject->SetZPos(fSelZ, ID_DYNAMIC);
	listStore.emplace_back(pGameObject);

	//셀필
	//pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5) + 300.f, 150.f + (m_iStoreIdx*90.f)), _vec2(520.f, 80.f), 0, L"Proto_Texture_Store_SelFill", L"UI_Store_SelFill" + wstrListIdx), bActive);
	//pGameObject->SetZPos(fSelFillZ, ID_DYNAMIC);
	//listStore.emplace_back(pGameObject);
	//
	
	////아이콘
	//_bool  bActive = false;
	//pGameObject = CUI::Create(m_pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5) + 300.f, _float(WINCY*0.5)), _vec2(_float(WINCX)*0.5f, _float(WINCY)), 0, L"Proto_Texture_Store_Active", L"UI_Store_Active" + to_wstring(m_iStoreIdx)), bActive);
	//pGameObject->SetZPos(fSelZ, ID_DYNAMIC);
	//m_listStoreActiveList.emplace_back(pGameObject);

	////폰트 이름
	//pGameObject = CUI::Create(m_pGraphicDev, UIFONT{ L"Font_L_Normal", /*이름 */L"", _vec2(860.f, 220.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f) }, bActive);
	//dynamic_cast<CUI*>(pGameObject)->SetObjTag(L"StoreActive_Font_L_Normal" + to_wstring(m_iStoreIdx));
	//m_listStoreActiveList.emplace_back(pGameObject);


	//설명덩어리 리스트
	SetItemInfoList(pGraphicDev,eItemIdx, listStore);

	for (auto iter : listStore)
	{
		Add_GameObject(L"UI", dynamic_cast<CUI*>(iter)->GetObjTag().c_str(), iter);
	}

	if (eItemIdx < UI::ACTIVE_ITEM_END)
	{
		m_mapStoreActiveList.emplace(m_iStoreIdx, listStore);
	}
	else if (eItemIdx < UI::ITEM_END)
	{
		m_mapStoreStoneList.emplace(m_iStoreIdx, listStore);
	}

	m_iStoreIdx++;
	listStore.clear();
}



void CUIMgr::SetItemInfoList(LPDIRECT3DDEVICE9 pGraphicDev, UI::ITEM eItemIdx, list<CGameObject*>& listItemInsert)
{
	CGameObject* pGameObject = nullptr;
	_bool  bActive = false;
	wstring wstrListIdx = to_wstring(m_iStoreIdx) + to_wstring(eItemIdx);
	//아이콘
	//이름
	//내용
	//가격
	_float fIconBSize = 80.f;
	_float fIconInfoSize = 40.f;
	_float fInfoZ = 0.38f;
	
	wstring wstrProtoTag;
	UIFONT tNameFont;
	UIFONT tInfoFont;
	_uint iPrice;


	UISET tInfo;

	list<UISET> listInfo;
	list<UIFONT>listFont;

	tNameFont.wstrFont = L"Font_L_Heavy_Small";
	tNameFont.vPos = _vec2(730.f, 120.f + (m_iStoreIdx*90.f));
	tNameFont.vColor = _vec4(1.f, 1.f, 1.f, 1.f);

	tInfoFont.wstrFont = L"Font_L_Normal_Small";
	tInfoFont.vPos = _vec2(730.f, 160.f + (m_iStoreIdx*90.f));
	tInfoFont.vColor = _vec4(1.f, 1.f, 1.f, 1.f);


	switch (eItemIdx)
	{
	case UI::ACTIVE_AIR_SPIN: 
	{	
		//이름 폰트
		iPrice = 400;
		tNameFont.wstrText = L"회전 베기";
		tInfoFont.wstrText = L"공중에서                 ";
		for (_uint i = 0; i < 3; i++)
		{
			tInfo.iTextureNum = 0;
			tInfo.vPos = _vec2(810.f + (30.f*listInfo.size()), 165.f + (m_iStoreIdx*90.f));
			tInfo.wstrObjTag = L"UI_Store_Active_Info" + wstrListIdx + to_wstring(listInfo.size());
			listInfo.emplace_back(tInfo);
		}
	}
		break;

	case UI::ACTIVE_LIGHTATTACK1UP:
	{
		iPrice = 800;
		tNameFont.wstrText = L"약 베기1 UP";
		tInfoFont.wstrText = L"     업그레이드";

		tInfo.iTextureNum = 0;
		tInfo.vPos = _vec2(740.f + (30.f*listInfo.size()), 165.f + (m_iStoreIdx*90.f));
		tInfo.wstrObjTag = L"UI_Store_Active_Info" + wstrListIdx + to_wstring(listInfo.size());
		listInfo.emplace_back(tInfo);

	}
		break;
	case UI::ACTIVE_LIGHTATTACK2UP:
	{
		iPrice = 1200;
		tNameFont.wstrText = L"약 베기2 UP";
		tInfoFont.wstrText = L"            업그레이드";

		for (_uint i = 0; i < 2; i++)
		{
			tInfo.iTextureNum = 0;
			tInfo.vPos = _vec2(740.f + (30.f*listInfo.size()), 165.f + (m_iStoreIdx*90.f));
			tInfo.wstrObjTag = L"UI_Store_Active_Info" + wstrListIdx + to_wstring(listInfo.size());
			listInfo.emplace_back(tInfo);
		}
	}
		break;
	case UI::ACTIVE_HEAVYATTACK1UP:
	{
		iPrice = 1300;
		tNameFont.wstrText = L"강 베기1 UP";
		tInfoFont.wstrText = L"    업그레이드";

		tInfo.iTextureNum = 1;
		tInfo.vPos = _vec2(740.f + (30.f*listInfo.size()), 165.f + (m_iStoreIdx*90.f));
		tInfo.wstrObjTag = L"UI_Store_Active_Info" + wstrListIdx + to_wstring(listInfo.size());
		listInfo.emplace_back(tInfo);
		
	}
		break;
	case UI::ACTIVE_HEAVYATTACK2UP: {
		iPrice = 1600;
		tNameFont.wstrText = L"강 베기2 UP";
		tInfoFont.wstrText = L"            업그레이드";

		for (_uint i = 0; i < 2; i++)
		{
			tInfo.iTextureNum = 1;
			tInfo.vPos = _vec2(740.f + (30.f*listInfo.size()), 165.f + (m_iStoreIdx*90.f));
			tInfo.wstrObjTag = L"UI_Store_Active_Info" + wstrListIdx + to_wstring(listInfo.size());
			listInfo.emplace_back(tInfo);
		}
	}
		break;
	case UI::ACTIVE_DASHATTACK: {

		iPrice = 1300;
		tNameFont.wstrText = L"대시 공격";
		tInfoFont.wstrText = L"       후      ";


		tInfo.iTextureNum = 3;
		tInfo.vPos = _vec2(740.f + (30.f*listInfo.size()*2.f), 165.f + (m_iStoreIdx*90.f));
		tInfo.wstrObjTag = L"UI_Store_Active_Info" + wstrListIdx + to_wstring(listInfo.size());
		listInfo.emplace_back(tInfo);

		tInfo.iTextureNum = 0;
		tInfo.vPos = _vec2(740.f + (30.f*listInfo.size()*2.f), 165.f + (m_iStoreIdx*90.f));
		tInfo.wstrObjTag = L"UI_Store_Active_Info" + wstrListIdx + to_wstring(listInfo.size());
		listInfo.emplace_back(tInfo);

	}
		break;
	case UI::ACTIVE_REFLECT: {

		iPrice = 1300;
		tNameFont.wstrText = L"반격";
		tInfoFont.wstrText = L"       후       성공시 반격";


		tInfo.iTextureNum = 2;
		tInfo.vPos = _vec2(740.f + (30.f*listInfo.size()*2.f), 165.f + (m_iStoreIdx*90.f));
		tInfo.wstrObjTag = L"UI_Store_Active_Info" + wstrListIdx + to_wstring(listInfo.size());
		listInfo.emplace_back(tInfo);

		tInfo.iTextureNum = 0;
		tInfo.vPos = _vec2(740.f + (30.f*listInfo.size()*2.f), 165.f + (m_iStoreIdx*90.f));
		tInfo.wstrObjTag = L"UI_Store_Active_Info" + wstrListIdx + to_wstring(listInfo.size());
		listInfo.emplace_back(tInfo);

	}
		break;
	case UI::ACTIVE_REFLECTUP: {
		//아이콘
		iPrice = 1200;
		tNameFont.wstrText = L"반격 강화";
		tInfoFont.wstrText = L"       후       업그레이드";

		tInfo.iTextureNum = 2;
		tInfo.vPos = _vec2(740.f + (30.f*listInfo.size()*2.f), 165.f + (m_iStoreIdx*90.f));
		tInfo.wstrObjTag = L"UI_Store_Active_Info" + wstrListIdx + to_wstring(listInfo.size());
		listInfo.emplace_back(tInfo);

		tInfo.iTextureNum = 0;
		tInfo.vPos = _vec2(740.f + (30.f*listInfo.size()*2.f), 165.f + (m_iStoreIdx*90.f));
		tInfo.wstrObjTag = L"UI_Store_Active_Info" + wstrListIdx + to_wstring(listInfo.size());
		listInfo.emplace_back(tInfo);
	}
		break;
	case UI::ACTIVE_AIR_DASH: {
		iPrice = 1200;
		tNameFont.wstrText = L"내려찍기";
		tInfoFont.wstrText = L"       후      ";

		tInfo.iTextureNum = 4;
		tInfo.vPos = _vec2(740.f + (30.f*listInfo.size()*2.f), 165.f + (m_iStoreIdx*90.f));
		tInfo.wstrObjTag = L"UI_Store_Active_Info" + wstrListIdx + to_wstring(listInfo.size());
		listInfo.emplace_back(tInfo);

		tInfo.iTextureNum = 3;
		tInfo.vPos = _vec2(740.f + (30.f*listInfo.size()*2.f), 165.f + (m_iStoreIdx*90.f));
		tInfo.wstrObjTag = L"UI_Store_Active_Info" + wstrListIdx + to_wstring(listInfo.size());
		listInfo.emplace_back(tInfo);
	}
		break;
	case UI::ACTIVE_COMBO_ELEMENT: {
		iPrice = 1700;
		tNameFont.wstrText = L"속성 공격";
		tInfoFont.wstrText = L"       후      ";

		tInfo.iTextureNum = 0;
		tInfo.vPos = _vec2(740.f + (30.f*listInfo.size()), 165.f + (m_iStoreIdx*90.f));
		tInfo.wstrObjTag = L"UI_Store_Active_Info" + wstrListIdx + to_wstring(listInfo.size());
		listInfo.emplace_back(tInfo);

		tInfo.iTextureNum = 1;
		tInfo.vPos = _vec2(740.f + (30.f*listInfo.size()), 165.f + (m_iStoreIdx*90.f));
		tInfo.wstrObjTag = L"UI_Store_Active_Info" + wstrListIdx + to_wstring(listInfo.size());
		listInfo.emplace_back(tInfo);

	}
		break;
	case UI::ITEM_BOX1:
	{
		iPrice = 500;
		tNameFont.wstrText = L"하급 상자";
		tInfoFont.wstrText = L"하급 스톤이 나옵니다 ";

	}
		break;
	case UI::ITEM_BOX2: 
	{
		iPrice = 500;
		tNameFont.wstrText = L"중급 상자";
		tInfoFont.wstrText = L"중급 스톤이 나옵니다 ";
	}
		break;
	case UI::ITEM_BOX3: 
	{
		iPrice = 500;
		tNameFont.wstrText = L"고급 상자";
		tInfoFont.wstrText = L"고급 스톤이 나옵니다 ";
	}
		break;
	case UI::ITEM_GRINNER: 
	{
		iPrice = 500;
		tNameFont.wstrText = L"그리너 스톤";
		tInfoFont.wstrText = L"그리너 스톤을 획득합니다";
	}
		break;
	case UI::ITEM_SKULLMAGE: 
	{
		iPrice = 500;
		tNameFont.wstrText = L"해골 마법사 스톤";
		tInfoFont.wstrText = L"해골 마법사 스톤을 획득합니다";
	}
		break;
	case UI::ITEM_BROODI: 
	{
		iPrice = 500;
		tNameFont.wstrText = L"브루디 스톤";
		tInfoFont.wstrText = L"브루디 스톤을 획득합니다 ";
	}
		break;
	case UI::ITEM_BAT: 
	{
		iPrice = 500;
		tNameFont.wstrText = L"박쥐 스톤";
		tInfoFont.wstrText = L"박쥐 스톤을 획득합니다 ";
	}
		break;
	case UI::ITEM_END:
		break;
	}

	if (eItemIdx < UI::ACTIVE_ITEM_END)
	{
		wstrProtoTag = L"Proto_Texture_Store_Active";
	}
	else
	{
		wstrProtoTag = L"Proto_Texture_Store_Stone";
	}

	//아이콘
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(685.f, 150.f + (m_iStoreIdx*90.f)), _vec2(fIconBSize, fIconBSize), m_iStoreIdx, wstrProtoTag.c_str(), L"UI_Store_Active_Icon" + wstrListIdx), bActive);
	pGameObject->SetZPos(fInfoZ, ID_DYNAMIC);
	listItemInsert.emplace_back(pGameObject);

	//이름
	pGameObject = CUI::Create(pGraphicDev, tNameFont, bActive);
	dynamic_cast<CUI*>(pGameObject)->SetObjTag(L"StoreActive_Font_L_Normal_Name" + wstrListIdx);
	listItemInsert.emplace_back(pGameObject);

	//내용 폰트
	pGameObject = CUI::Create(pGraphicDev, tInfoFont, bActive);
	dynamic_cast<CUI*>(pGameObject)->SetObjTag(L"StoreActive_Font_L_Light_Info" + wstrListIdx);
	listItemInsert.emplace_back(pGameObject);


	//내용 아이콘
	for (auto iter : listInfo)
	{
		iter.vSize = _vec2(fIconInfoSize, fIconInfoSize);
		iter.wstrTexture = L"Proto_Texture_Store_Active_Info";

		pGameObject = CUI::Create(pGraphicDev, iter, bActive);
		pGameObject->SetZPos(fInfoZ, ID_DYNAMIC);
		listItemInsert.emplace_back(pGameObject);
	}


	//소울 아이콘
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(1050.f, 165.f + (m_iStoreIdx*90.f)), _vec2(fIconInfoSize, fIconInfoSize), 0, L"Proto_Texture_Store_Soul", L"UI_Store_Soul" + wstrListIdx), bActive);
	pGameObject->SetZPos(fInfoZ, ID_DYNAMIC);
	listItemInsert.emplace_back(pGameObject);

	pGameObject = CUI::Create(pGraphicDev, UIFONT{ L"Font_L_Normal_Small",to_wstring(iPrice), _vec2(1065, 160.f + (m_iStoreIdx*90.f)), D3DXCOLOR(1.f, 1.f, 1.f, 1.f) }, bActive);
	dynamic_cast<CUI*>(pGameObject)->SetObjTag(L"StoreActive_Font_L_Light_Price" + wstrListIdx);
	listItemInsert.emplace_back(pGameObject);





}

void CUIMgr::MoveStoneList(_float fTimeDelta,_float fSpeed)
{
	for (auto iter : m_mapStoneList)
	{
		for (auto iter_second : iter.second)
		{
			_float fListCount = 7.f;
			_vec2 vPos = dynamic_cast<CUI*>(iter_second)->GetUIPos();
			if (fSpeed > 0.f)
			{
				if (dynamic_cast<CUI*>(iter_second)->GetInitPos().y+ fListCount*60.f < vPos.y + m_StoneListIdx*60.f)
					vPos.y -= fSpeed*fTimeDelta;
			}
			else
			{
				if (dynamic_cast<CUI*>(iter_second)->GetInitPos().y > vPos.y)
					vPos.y -= fSpeed*fTimeDelta;
			}
			dynamic_cast<CUI*>(iter_second)->SetUIPos(vPos);
		}
	}
}

//획득했을때 스톤이 있는지 확인하고 
//-> 플레이어에서 vector enum으로 저장하자 
// 없으면 추가하면서 UI Create 
//획득할때마다 UI 생성
//리스트 스크롤에서 액티브 해주고 스크롤로 포지션 올리거나 내리기
void CUIMgr::SetStoneInfoUI(LPDIRECT3DDEVICE9 pGraphicDev,STONE tStone) //순서대로 아래로 생성 열때마다 맨위로 조정 
{
	auto iter_find = m_mapStoneList.find(tStone.eCreature);
	if (iter_find != m_mapStoneList.end())
	{
		return;
	}

	//2. 획득할때마다 셋팅-> 이게 맞는듯?
	CGameObject* pGameObject = nullptr;

	_float fStoneSize = 90.f;
	_float fIconSize = 50.f;

	_float fSelZ = 0.2f;
	_float fStoneZ = 0.2f;
	_float fCoreZ = 0.19f;
	_bool bActive =false;
	

	//오른쪽 설명 리스트에 스톤 ->빤짝 쉐이더 먹인 스톤 
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(780.f, 180.f), _vec2(fStoneSize, fStoneSize), (_uint)tStone.eCreature, L"Proto_Texture_CoreTree_Creature", L"UI_Info_Stone" + to_wstring(tStone.eCreature)), bActive);
	pGameObject->SetZPos(fStoneZ, ID_DYNAMIC);
	dynamic_cast<CUI*>(pGameObject)->SetShade(UI::SHADE_UVMOVE);
	m_listStoneInfo.emplace_back(pGameObject);


	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(840.f, 225.f), _vec2(fIconSize, fIconSize), tStone.iElementType, L"Proto_Texture_CoreTree_StoneElement", L"UI_Info_Element" + to_wstring(tStone.eCreature)), bActive);
	pGameObject->SetZPos(fStoneZ, ID_DYNAMIC);
	m_listStoneInfo.emplace_back(pGameObject);


	//대문짝폰트
	pGameObject = CUI::Create(pGraphicDev, UIFONT{ L"Font_L_Heavy", tStone.wstrName.c_str(), _vec2(830.f, 150.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f) },bActive);
	dynamic_cast<CUI*>(pGameObject)->SetObjTag(L"StoneInfo_Font_L_Heavy" + to_wstring(tStone.eCreature));
	m_listStoneInfo.emplace_back(pGameObject);

	wstring wstrStoneElement;

	switch (tStone.iElementType)
	{
	case UI::STONE_ALL:
		break;
	case UI::STONE_SKILL:
		wstrStoneElement = L"SKILL TYPE";
		break;
	case UI::STONE_ATK:
		wstrStoneElement = L"ATK TYPE";
		break;
	case UI::STONE_HEALTH:
		wstrStoneElement = L"HEALTH TYPE";
		break;
	case UI::STONE_ELEMENT_END:
		break;
	default:
		break;
	}
	//미들폰트
	pGameObject = CUI::Create(pGraphicDev, UIFONT{ L"Font_L_Normal", wstrStoneElement.c_str(), _vec2(860.f, 220.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f) }, bActive);
	dynamic_cast<CUI*>(pGameObject)->SetObjTag(L"StoneInfo_Font_L_Normal" + to_wstring(tStone.eCreature));
	m_listStoneInfo.emplace_back(pGameObject);

	//스몰폰트
	pGameObject = CUI::Create(pGraphicDev, UIFONT{ L"Font_L_Light", tStone.wstrInfo.c_str(), _vec2(830.f, 275.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f) }, bActive);
	dynamic_cast<CUI*>(pGameObject)->SetObjTag(L"StoneInfo_Font_L_Light" + to_wstring(tStone.eCreature));
	m_listStoneInfo.emplace_back(pGameObject);

	//이름까지 


	for (auto iter : m_listStoneInfo)
	{
		Add_GameObject(L"UI", dynamic_cast<CUI*>(iter)->GetObjTag().c_str(), iter);
	}

	m_mapStoneInfo.emplace(tStone.eCreature, m_listStoneInfo);
	m_listStoneInfo.clear();
}

void CUIMgr::SetStoneListUI(LPDIRECT3DDEVICE9 pGraphicDev, STONE tStone)
{
	auto iter_find = m_mapStoneList.find(tStone.eCreature);
	if (iter_find != m_mapStoneList.end())
	{
		//돌이 중복됨 -> 레벨기능? ㄱ? 
		return;
	}
	CGameObject* pGameObject = nullptr;

	_float fStoneSize = 90.f;
	_float fIconSize = 50.f;

	_float fPosy = 180.f + (60.f*m_StoneListIdx);
	_float fSelZ = 0.2f;
	_float fStoneZ = 0.2f;
	_float fCoreZ = 0.19f;
	_bool bActive = false;

	//셀 bar
	pGameObject = CUI::Create(pGraphicDev,
		Set_UISET(_vec2(80.f, fPosy), _vec2(fIconSize, fIconSize),
			0, L"Proto_Texture_CoreTree_StoneList_Sel", L"UI_CoreTree_StoneList_SelBar"+to_wstring(tStone.eCreature)), bActive);
	pGameObject->SetZPos(fSelZ, ID_DYNAMIC);
	m_listStoneList.emplace_back(pGameObject);
	//Add_GameObject();

	//이건 보석 리스트를 만들어놓고 뽑아오자
	//보석 UI를 미리 만들어놓고 정보들 뽑아오기 

	//크리쳐
	pGameObject = CUI::Create(pGraphicDev,
		Set_UISET(_vec2(95.f, fPosy), _vec2(fIconSize, fIconSize),
			(_uint)tStone.eCreature, L"Proto_Texture_CoreTree_Creature", L"UI_CoreTree_StoneList_Creature" + to_wstring(tStone.eCreature)), bActive);
	pGameObject->SetZPos(fCoreZ, ID_DYNAMIC);
	m_listStoneList.emplace_back(pGameObject);

	//타입
	pGameObject = CUI::Create(pGraphicDev,
		Set_UISET(_vec2(600.f, fPosy), _vec2(fIconSize, fIconSize),
			tStone.iElementType*2, L"Proto_Texture_CoreTree_StoneElement", L"UI_CoreTree_StoneList_Element" + to_wstring(tStone.eCreature)), bActive);
	pGameObject->SetZPos(fCoreZ, ID_DYNAMIC);
	m_listStoneList.emplace_back(pGameObject);

	//미들폰트

	pGameObject = CUI::Create(pGraphicDev, UIFONT{ L"Font_L_Normal",tStone.wstrName.c_str() , _vec2(145.f, fPosy-10.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f) }, bActive);
	dynamic_cast<CUI*>(pGameObject)->SetObjTag(L"StoneList_Font_L_Normal" + to_wstring(tStone.eCreature));
	m_listStoneList.emplace_back(pGameObject);

	for (auto iter : m_listStoneList)
	{
		Add_GameObject(L"UI", dynamic_cast<CUI*>(iter)->GetObjTag().c_str(), iter);
	}

	m_mapStoneList.emplace(tStone.eCreature, m_listStoneList);
	m_listStoneList.clear();
	m_StoneListIdx++;
}





map<UI::LISTKEY,list<CGameObject*>> CUIMgr::InitCreateUI(LPDIRECT3DDEVICE9 pGraphicDev)
{

	InitWeaponElement(pGraphicDev);
	InitCoretree(pGraphicDev);
	InitCoreList(pGraphicDev);
	InitPlayerInfo(pGraphicDev);

	//InitStore(pGraphicDev);


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

}

void CUIMgr::SetActivePlayerInfo(_bool bActive)
{
	for (auto iter : m_listPlayerInfo)
	{
		dynamic_cast<CUI*>(iter)->SetActive(bActive);
	}
}

void CUIMgr::SetActiveStoneListUI(_bool bActive)
{
	for (auto iter : m_listCoreList)
	{
		dynamic_cast<CUI*>(iter)->SetActive(bActive);
	}

	for (auto iter : m_mapStoneList)
	{
		for(auto iter_second : iter.second)
			dynamic_cast<CUI*>(iter_second)->SetActive(bActive);
	}

}

void CUIMgr::SetActiveStoneInfoUI(_bool bActive, _uint iStoneIdx)
{
	auto iter_find = m_mapStoneInfo.find(iStoneIdx);

	for (auto iter : iter_find->second)
			dynamic_cast<CUI*>(iter)->SetActive(bActive);
	
}

void CUIMgr::SetActiveStoreStoneUI(_bool bActive)
{
	for (auto iter : m_listStoreBase)
	{
		dynamic_cast<CUI*>(iter)->SetActive(bActive);
	}
	for (auto iter : m_mapStoreStoneList)
	{
		for (auto iter_second : iter.second)
			dynamic_cast<CUI*>(iter_second)->SetActive(bActive);
	}

}

void CUIMgr::SetActiveStoreActiveUI(_bool bActive)
{
	for (auto iter : m_listStoreBase)
	{
		dynamic_cast<CUI*>(iter)->SetActive(bActive);
	}
	for (auto iter : m_mapStoreActiveList)
	{
		for (auto iter_second : iter.second)
			dynamic_cast<CUI*>(iter_second)->SetActive(bActive);
	}
}

list<CGameObject*> CUIMgr::GetStoneSelIdxList(wstring wstrObjtag, _uint* iStoneIdx)
{
	for (auto iter : m_mapStoneList)
	{
		for (auto iter_second : iter.second)
		{
			if (wstrObjtag == dynamic_cast<CUI*>(iter_second)->GetObjTag())
			{
				*iStoneIdx = iter.first;
				return iter.second;
			}
		}
	}
	return list<CGameObject*>();
}

list<CGameObject*> CUIMgr::GetItemActiveSelIdxList(wstring wstrObjtag, _uint * iSelIdx)
{
	for (auto iter : m_mapStoreActiveList)
	{
		for (auto iter_second : iter.second)
		{
			if (wstrObjtag == dynamic_cast<CUI*>(iter_second)->GetObjTag())
			{
				*iSelIdx = iter.first;
				return iter.second;
			}
		}
	}
	return list<CGameObject*>();
}

list<CGameObject*> CUIMgr::GetItemStoneSelIdxList(wstring wstrObjtag, _uint * iSelIdx)
{
	for (auto iter : m_mapStoreStoneList)
	{
		for (auto iter_second : iter.second)
		{
			if (wstrObjtag == dynamic_cast<CUI*>(iter_second)->GetObjTag())
			{
				*iSelIdx = iter.first;
				return iter.second;
			}
		}
	}
	return list<CGameObject*>();
}

_uint CUIMgr::GetWheelMove()
{
	if (m_fPreWheel < g_fWheel)
	{
		m_fPreWheel = g_fWheel;
		return WHEEL::MOVE_UP;
	}
	else if(m_fPreWheel > g_fWheel)
	{
		m_fPreWheel = g_fWheel;
		return WHEEL::MOVE_DOWN;
	}
	return WHEEL::MOVE_POS;
}

_bool CUIMgr::GetElemetUIActive()
{
	return m_listWeaponElement.front()->GetActive();
}

_bool CUIMgr::GetCoreTreeUIActive()
{
	return m_listCoreTree.front()->GetActive();
}

_bool CUIMgr::GetStoneInfoUIActive(_uint iStoneIdx)
{
	auto iter_find = m_mapStoneInfo.find(iStoneIdx);

	return iter_find->second.front()->GetActive();

}

_bool CUIMgr::GetStoreUIActive()
{
	return m_listStoreBase.front()->GetActive();
}



void CUIMgr::Free(void)
{

	//for (auto iter : m_listWeaponElement)
	//	Safe_Release(iter);
	//for (auto iter : m_listCoreTree)
	//	Safe_Release(iter);
	//for (auto iter : m_listPlayerInfo)
	//	Safe_Release(iter);
	//for (auto iter : m_listStoneInfo)
	//	Safe_Release(iter);

	//for (auto iter : m_listStoreBase)
	//	Safe_Release(iter);

	//for (auto iter : m_mapStoneList)
	//{
	//	for (auto iter_second : iter.second)
	//	{
	//		Safe_Release(iter_second);
	//	}
	//}
	//for (auto iter : m_mapStoneInfo)
	//{
	//	for (auto iter_second : iter.second)
	//	{
	//		Safe_Release(iter_second);
	//	}
	//}

	//for (auto iter : m_mapStoreActiveList)
	//{
	//	for (auto iter_second : iter.second)
	//	{
	//		Safe_Release(iter_second);
	//	}
	//}
	//for (auto iter : m_mapStoreStoneList)
	//{
	//	for (auto iter_second : iter.second)
	//	{
	//		Safe_Release(iter_second);
	//	}
	//}

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


