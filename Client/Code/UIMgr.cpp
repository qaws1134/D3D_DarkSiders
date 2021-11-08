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

list<list<CGameObject*>> CUIMgr::InitCreateUI(LPDIRECT3DDEVICE9 pGraphicDev)
{
	InitWeaponElement(pGraphicDev);

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

_bool CUIMgr::GetElemetUIActive()
{
	return m_listWeaponElement.front()->GetActive();
}


void CUIMgr::Free(void)
{
	for (auto iter : m_listWeaponElement)
		Safe_Release(iter);
}
