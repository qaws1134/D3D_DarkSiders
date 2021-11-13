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


	//�� 6�������� ������ �������� 
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
	//�ش� ����Ʈ�� ���콺�� �浹üũ�ؼ� �浹�� ������Ʈ�� �±׷� �˾ƺ��� ��ȣ�ۿ� ��? ��������?

	m_mapHead.emplace(UI::LISTKEY_WEAPONELEMENT,m_listWeaponElement);
}

//UIType�� �༭ �ش� ���� Ÿ������ ��� 
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


	//��ŷ�ؼ� ������Ʈ �±׿� ���� �Ű�����

	//Ŀ�� �߰�
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(GetCoretreePos(0), _vec2(fSelUISize, fSelUISize), 0, L"Proto_Texture_CoreTree_Sel", L"UI_CoreTree_Sel"), bActive);
	pGameObject->SetZPos(fSelZ, ID_DYNAMIC);
	m_listCoreTree.emplace_back(pGameObject);

	//��� �߰� 
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5), _float(WINCY*0.5)), _vec2(fBgUISize, fBgUISize), 3, L"Proto_Texture_CoreTree_Bg", L"UI_CoreTree_Bg"), bActive);
	pGameObject->SetZPos(fBgZ, ID_DYNAMIC);
	m_listCoreTree.emplace_back(pGameObject);


	//���̽� 
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5), _float(WINCY*0.5)-100.f), _vec2(fBaseUISize+300.f, fBaseUISize), 0, L"Proto_Texture_CoreTree_Base", L"UI_CoreTree_Base"), bActive);
	pGameObject->SetZPos(fBaseZ, ID_DYNAMIC);
	m_listCoreTree.emplace_back(pGameObject);

	//�� ĭ 
	for (_uint i = 0; i < 19; i++)
	{
		if (0 == i ||1 == i ||
			8 == i || 16 == i || 17 == i  )
		{
			//���
			pGameObject = CUI::Create(pGraphicDev, Set_UISET(GetCoretreePos(i), _vec2(fStoneUISize, fStoneUISize), 1, L"Proto_Texture_CoreTree_Core", L"UI_CoreTree_Core" + to_wstring(i)), bActive);
		}
		else
			pGameObject = CUI::Create(pGraphicDev, Set_UISET(GetCoretreePos(i), _vec2(fStoneUISize, fStoneUISize), 0, L"Proto_Texture_CoreTree_Core", L"UI_CoreTree_Core"+ to_wstring(i)), bActive);
		//���ǹ� ���� ���̴� ���� 
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

	//��� ���� 
	pGameObject = CUI::Create(pGraphicDev, 
		Set_UISET(_vec2(_float(WINCX*0.5), _float(WINCY*0.5)), _vec2(WINCX*1.2f, WINCY*1.2f),
			0, L"Proto_Texture_CoreTree_StoneList_Base", L"UI_CoreTree_StoneList_Bg"), bActive);
	pGameObject->SetZPos(fBgZ, ID_DYNAMIC);
	m_listCoreList.emplace_back(pGameObject);


	////���̽� ���
	pGameObject = CUI::Create(pGraphicDev,
		Set_UISET(_vec2(_float(WINCX*0.5), _float(WINCY*0.5)), _vec2(WINCX*0.98f, WINCY*0.8f),
			2, L"Proto_Texture_CoreTree_StoneList_Base", L"UI_CoreTree_StoneList_ListInfoBg"), bActive);
	pGameObject->SetZPos(fBaseZ, ID_DYNAMIC);
	m_listCoreList.emplace_back(pGameObject);

	//����Ʈ ���� ���
	//���������� ���� ����
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

	//�ʻ�ȭ
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(1120.f, 630.f), _vec2(fFortraitSize, fFortraitSize), 0, L"Proto_Texture_Portrait", L"UI_Info_Portrait"), bActive);
	pGameObject->SetZPos(fUIZ, ID_DYNAMIC);
	m_listPlayerInfo.emplace_back(pGameObject);


	//����
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(1000.f, 670.f), _vec2(fLineSize, 5.f), 0, L"Proto_Texture_UnderBar", L"UI_Info_UnderBar"), bActive);
	pGameObject->SetZPos(fUIZ, ID_DYNAMIC);
	m_listPlayerInfo.emplace_back(pGameObject);

	pGameObject = CUI::Create(pGraphicDev, UIFONT{ L"Font_L_Normal", L"W A R   P O W E R", _vec2(765.f, 615.f), D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f) }, bActive);
	dynamic_cast<CUI*>(pGameObject)->SetObjTag( L"PlayerInfo_Font_L_Normal" + to_wstring(10));
	m_listPlayerInfo.emplace_back(pGameObject);
	//����, �����, ü�� 
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

	//���� ��� �߰� 
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

	//���� ó�� 
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
			//���� ��Ʈ ��� 
		}
		InitStoreList(pGraphicDev, m_listStoreActiveList, (UI::ITEM)i);
	}
	m_iStoreIdx = 0;

	for (_uint i = (_uint)UI::ACTIVE_ITEM_END + 1; i < UI::ITEM_END; i++)
	{
		if (i == (_uint)UI::ACTIVE_ITEM_END + 1)
		{
			//���� ��Ʈ ���
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
	//����Ʈ�ڽ�
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5) + 300.f,150.f+(m_iStoreIdx*90.f)), _vec2(520.f, 80.f), 0, L"Proto_Texture_List", L"UI_Store_List"+ wstrListIdx), bActive);
	pGameObject->SetZPos(fListZ, ID_DYNAMIC);
	listStore.emplace_back(pGameObject);

	//��

	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5) + 90.f, 150.f + (m_iStoreIdx*90.f)), _vec2(100.f, 80.f), 0, L"Proto_Texture_Store_Sel", L"UI_Store_Sel" + wstrListIdx), bActive);
	pGameObject->SetZPos(fSelZ, ID_DYNAMIC);
	listStore.emplace_back(pGameObject);

	//����
	//pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5) + 300.f, 150.f + (m_iStoreIdx*90.f)), _vec2(520.f, 80.f), 0, L"Proto_Texture_Store_SelFill", L"UI_Store_SelFill" + wstrListIdx), bActive);
	//pGameObject->SetZPos(fSelFillZ, ID_DYNAMIC);
	//listStore.emplace_back(pGameObject);
	//
	
	////������
	//_bool  bActive = false;
	//pGameObject = CUI::Create(m_pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5) + 300.f, _float(WINCY*0.5)), _vec2(_float(WINCX)*0.5f, _float(WINCY)), 0, L"Proto_Texture_Store_Active", L"UI_Store_Active" + to_wstring(m_iStoreIdx)), bActive);
	//pGameObject->SetZPos(fSelZ, ID_DYNAMIC);
	//m_listStoreActiveList.emplace_back(pGameObject);

	////��Ʈ �̸�
	//pGameObject = CUI::Create(m_pGraphicDev, UIFONT{ L"Font_L_Normal", /*�̸� */L"", _vec2(860.f, 220.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f) }, bActive);
	//dynamic_cast<CUI*>(pGameObject)->SetObjTag(L"StoreActive_Font_L_Normal" + to_wstring(m_iStoreIdx));
	//m_listStoreActiveList.emplace_back(pGameObject);


	//����� ����Ʈ
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
	//������
	//�̸�
	//����
	//����
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
		//�̸� ��Ʈ
		iPrice = 400;
		tNameFont.wstrText = L"ȸ�� ����";
		tInfoFont.wstrText = L"���߿���                 ";
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
		tNameFont.wstrText = L"�� ����1 UP";
		tInfoFont.wstrText = L"     ���׷��̵�";

		tInfo.iTextureNum = 0;
		tInfo.vPos = _vec2(740.f + (30.f*listInfo.size()), 165.f + (m_iStoreIdx*90.f));
		tInfo.wstrObjTag = L"UI_Store_Active_Info" + wstrListIdx + to_wstring(listInfo.size());
		listInfo.emplace_back(tInfo);

	}
		break;
	case UI::ACTIVE_LIGHTATTACK2UP:
	{
		iPrice = 1200;
		tNameFont.wstrText = L"�� ����2 UP";
		tInfoFont.wstrText = L"            ���׷��̵�";

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
		tNameFont.wstrText = L"�� ����1 UP";
		tInfoFont.wstrText = L"    ���׷��̵�";

		tInfo.iTextureNum = 1;
		tInfo.vPos = _vec2(740.f + (30.f*listInfo.size()), 165.f + (m_iStoreIdx*90.f));
		tInfo.wstrObjTag = L"UI_Store_Active_Info" + wstrListIdx + to_wstring(listInfo.size());
		listInfo.emplace_back(tInfo);
		
	}
		break;
	case UI::ACTIVE_HEAVYATTACK2UP: {
		iPrice = 1600;
		tNameFont.wstrText = L"�� ����2 UP";
		tInfoFont.wstrText = L"            ���׷��̵�";

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
		tNameFont.wstrText = L"��� ����";
		tInfoFont.wstrText = L"       ��      ";


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
		tNameFont.wstrText = L"�ݰ�";
		tInfoFont.wstrText = L"       ��       ������ �ݰ�";


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
		//������
		iPrice = 1200;
		tNameFont.wstrText = L"�ݰ� ��ȭ";
		tInfoFont.wstrText = L"       ��       ���׷��̵�";

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
		tNameFont.wstrText = L"�������";
		tInfoFont.wstrText = L"       ��      ";

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
		tNameFont.wstrText = L"�Ӽ� ����";
		tInfoFont.wstrText = L"       ��      ";

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
		tNameFont.wstrText = L"�ϱ� ����";
		tInfoFont.wstrText = L"�ϱ� ������ ���ɴϴ� ";

	}
		break;
	case UI::ITEM_BOX2: 
	{
		iPrice = 500;
		tNameFont.wstrText = L"�߱� ����";
		tInfoFont.wstrText = L"�߱� ������ ���ɴϴ� ";
	}
		break;
	case UI::ITEM_BOX3: 
	{
		iPrice = 500;
		tNameFont.wstrText = L"��� ����";
		tInfoFont.wstrText = L"��� ������ ���ɴϴ� ";
	}
		break;
	case UI::ITEM_GRINNER: 
	{
		iPrice = 500;
		tNameFont.wstrText = L"�׸��� ����";
		tInfoFont.wstrText = L"�׸��� ������ ȹ���մϴ�";
	}
		break;
	case UI::ITEM_SKULLMAGE: 
	{
		iPrice = 500;
		tNameFont.wstrText = L"�ذ� ������ ����";
		tInfoFont.wstrText = L"�ذ� ������ ������ ȹ���մϴ�";
	}
		break;
	case UI::ITEM_BROODI: 
	{
		iPrice = 500;
		tNameFont.wstrText = L"���� ����";
		tInfoFont.wstrText = L"���� ������ ȹ���մϴ� ";
	}
		break;
	case UI::ITEM_BAT: 
	{
		iPrice = 500;
		tNameFont.wstrText = L"���� ����";
		tInfoFont.wstrText = L"���� ������ ȹ���մϴ� ";
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

	//������
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(685.f, 150.f + (m_iStoreIdx*90.f)), _vec2(fIconBSize, fIconBSize), m_iStoreIdx, wstrProtoTag.c_str(), L"UI_Store_Active_Icon" + wstrListIdx), bActive);
	pGameObject->SetZPos(fInfoZ, ID_DYNAMIC);
	listItemInsert.emplace_back(pGameObject);

	//�̸�
	pGameObject = CUI::Create(pGraphicDev, tNameFont, bActive);
	dynamic_cast<CUI*>(pGameObject)->SetObjTag(L"StoreActive_Font_L_Normal_Name" + wstrListIdx);
	listItemInsert.emplace_back(pGameObject);

	//���� ��Ʈ
	pGameObject = CUI::Create(pGraphicDev, tInfoFont, bActive);
	dynamic_cast<CUI*>(pGameObject)->SetObjTag(L"StoreActive_Font_L_Light_Info" + wstrListIdx);
	listItemInsert.emplace_back(pGameObject);


	//���� ������
	for (auto iter : listInfo)
	{
		iter.vSize = _vec2(fIconInfoSize, fIconInfoSize);
		iter.wstrTexture = L"Proto_Texture_Store_Active_Info";

		pGameObject = CUI::Create(pGraphicDev, iter, bActive);
		pGameObject->SetZPos(fInfoZ, ID_DYNAMIC);
		listItemInsert.emplace_back(pGameObject);
	}


	//�ҿ� ������
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

//ȹ�������� ������ �ִ��� Ȯ���ϰ� 
//-> �÷��̾�� vector enum���� �������� 
// ������ �߰��ϸ鼭 UI Create 
//ȹ���Ҷ����� UI ����
//����Ʈ ��ũ�ѿ��� ��Ƽ�� ���ְ� ��ũ�ѷ� ������ �ø��ų� ������
void CUIMgr::SetStoneInfoUI(LPDIRECT3DDEVICE9 pGraphicDev,STONE tStone) //������� �Ʒ��� ���� �������� ������ ���� 
{
	auto iter_find = m_mapStoneList.find(tStone.eCreature);
	if (iter_find != m_mapStoneList.end())
	{
		return;
	}

	//2. ȹ���Ҷ����� ����-> �̰� �´µ�?
	CGameObject* pGameObject = nullptr;

	_float fStoneSize = 90.f;
	_float fIconSize = 50.f;

	_float fSelZ = 0.2f;
	_float fStoneZ = 0.2f;
	_float fCoreZ = 0.19f;
	_bool bActive =false;
	

	//������ ���� ����Ʈ�� ���� ->��¦ ���̴� ���� ���� 
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(780.f, 180.f), _vec2(fStoneSize, fStoneSize), (_uint)tStone.eCreature, L"Proto_Texture_CoreTree_Creature", L"UI_Info_Stone" + to_wstring(tStone.eCreature)), bActive);
	pGameObject->SetZPos(fStoneZ, ID_DYNAMIC);
	dynamic_cast<CUI*>(pGameObject)->SetShade(UI::SHADE_UVMOVE);
	m_listStoneInfo.emplace_back(pGameObject);


	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(840.f, 225.f), _vec2(fIconSize, fIconSize), tStone.iElementType, L"Proto_Texture_CoreTree_StoneElement", L"UI_Info_Element" + to_wstring(tStone.eCreature)), bActive);
	pGameObject->SetZPos(fStoneZ, ID_DYNAMIC);
	m_listStoneInfo.emplace_back(pGameObject);


	//�빮¦��Ʈ
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
	//�̵���Ʈ
	pGameObject = CUI::Create(pGraphicDev, UIFONT{ L"Font_L_Normal", wstrStoneElement.c_str(), _vec2(860.f, 220.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f) }, bActive);
	dynamic_cast<CUI*>(pGameObject)->SetObjTag(L"StoneInfo_Font_L_Normal" + to_wstring(tStone.eCreature));
	m_listStoneInfo.emplace_back(pGameObject);

	//������Ʈ
	pGameObject = CUI::Create(pGraphicDev, UIFONT{ L"Font_L_Light", tStone.wstrInfo.c_str(), _vec2(830.f, 275.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f) }, bActive);
	dynamic_cast<CUI*>(pGameObject)->SetObjTag(L"StoneInfo_Font_L_Light" + to_wstring(tStone.eCreature));
	m_listStoneInfo.emplace_back(pGameObject);

	//�̸����� 


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
		//���� �ߺ��� -> �������? ��? 
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

	//�� bar
	pGameObject = CUI::Create(pGraphicDev,
		Set_UISET(_vec2(80.f, fPosy), _vec2(fIconSize, fIconSize),
			0, L"Proto_Texture_CoreTree_StoneList_Sel", L"UI_CoreTree_StoneList_SelBar"+to_wstring(tStone.eCreature)), bActive);
	pGameObject->SetZPos(fSelZ, ID_DYNAMIC);
	m_listStoneList.emplace_back(pGameObject);
	//Add_GameObject();

	//�̰� ���� ����Ʈ�� �������� �̾ƿ���
	//���� UI�� �̸� �������� ������ �̾ƿ��� 

	//ũ����
	pGameObject = CUI::Create(pGraphicDev,
		Set_UISET(_vec2(95.f, fPosy), _vec2(fIconSize, fIconSize),
			(_uint)tStone.eCreature, L"Proto_Texture_CoreTree_Creature", L"UI_CoreTree_StoneList_Creature" + to_wstring(tStone.eCreature)), bActive);
	pGameObject->SetZPos(fCoreZ, ID_DYNAMIC);
	m_listStoneList.emplace_back(pGameObject);

	//Ÿ��
	pGameObject = CUI::Create(pGraphicDev,
		Set_UISET(_vec2(600.f, fPosy), _vec2(fIconSize, fIconSize),
			tStone.iElementType*2, L"Proto_Texture_CoreTree_StoneElement", L"UI_CoreTree_StoneList_Element" + to_wstring(tStone.eCreature)), bActive);
	pGameObject->SetZPos(fCoreZ, ID_DYNAMIC);
	m_listStoneList.emplace_back(pGameObject);

	//�̵���Ʈ

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


