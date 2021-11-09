#include "stdafx.h"
#include "UIMgr.h"

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

	_float fBaseUISize = 500.f;
	_float fBaseZ = 0.3f;

	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5), _float(WINCY*0.5)), _vec2(fBaseUISize, fBaseUISize), 0, L"Proto_Texture_Element_Base", L"UI_Element_Base"),false);
	pGameObject->SetZPos(fBaseZ, ID_DYNAMIC);
	m_listWeaponElement.emplace_back(pGameObject);

	_float fElementZ = 0.2f;
	//UISET tInfo;
	_float fUISize = 120.f;
	//원 6각형으로 나눠서 더해주자 
	_float fRadius = 210.f;

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

	m_listHead.emplace_back(m_listWeaponElement);
}

//UIType을 줘서 해당 랜더 타겟으로 출력 
void CUIMgr::InitCoretreeElement(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGameObject* pGameObject = nullptr;

	_float fBaseUISize = 500.f;
	_float fStoneUISize = 90.f;
	_float fBaseZ = 0.5f;
	_float fStoneBaseZ = 0.4f;
	_bool  bActive = false;


	//배경 추가 


	//베이스 
	pGameObject = CUI::Create(pGraphicDev, Set_UISET(_vec2(_float(WINCX*0.5), _float(WINCY*0.5)-100.f), _vec2(fBaseUISize+300.f, fBaseUISize), 0, L"Proto_Texture_CoreTree_Base", L"UI_CoreTree_Base"), bActive);
	pGameObject->SetZPos(fBaseZ, ID_DYNAMIC);
	m_listCoreTree.emplace_back(pGameObject);
	
	//돌 칸 
	for (_uint i = 0; i < 19; i++)
	{
		pGameObject = CUI::Create(pGraphicDev, Set_UISET(GetCoretreePos(i), _vec2(fStoneUISize, fStoneUISize), 1, L"Proto_Texture_CoreTree_Base", L"UI_CoreTree_StoneBase"+i), bActive);
		//조건문 변경 쉐이더 변경 
		//dynamic_cast<CUI*>(pGameObject)->SetSubTex1(L"Proto_Texture_CoreTree_StoneElement",0);
		
		pGameObject->SetZPos(fStoneBaseZ, ID_DYNAMIC);
		m_listCoreTree.emplace_back(pGameObject);
	}

	//설명 추가 



	m_listHead.emplace_back(m_listCoreTree);
}

list<list<CGameObject*>> CUIMgr::InitCreateUI(LPDIRECT3DDEVICE9 pGraphicDev)
{
	InitWeaponElement(pGraphicDev);
	InitCoretreeElement(pGraphicDev);

	return m_listHead;
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