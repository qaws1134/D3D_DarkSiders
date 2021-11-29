#include "ColMgr.h"
#include "GameObject.h"

#include "Transform.h"
#include "Calculator.h"
#include "ColliderSphere.h"
USING(Engine)

CColMgr::CColMgr()
{
}


CColMgr::~CColMgr()
{
}

//���� �浹 -> �̵��Ұ� ->Ʈ�������� �����ؼ� �浹�� Ʈ�� -> �̵������� �ݴ�� �ѹ� �� ������ 

//�÷��̾� , �� 
void CColMgr::Col_Body(COLCHECK eColCheck,map<const _tchar* ,CGameObject*> _Dst, map<const _tchar* ,CGameObject*> _Src, MESHTYPE eMesh)
{
	wstring wstrDstTag;
	wstring wstrSrcTag;

	SetColType(eColCheck, &wstrDstTag, &wstrSrcTag);

	if (eColCheck == CHECK_BODY_BODY)
	{
		for (auto& pDst : _Dst)
		{
			//������ �浹ü		
			auto& iter_Dst = find_if(pDst.second->GetColmap().begin(), pDst.second->GetColmap().end(), CTag_Finder(wstrDstTag.c_str()));
			if (iter_Dst == pDst.second->GetColmap().end())
				continue;

			//�浹ü ���� 
			CColliderSphere* pDstCol = dynamic_cast<CColliderSphere*>(iter_Dst->second->Get_Component(L"Com_Collider", ID_STATIC));
			CCalculator* pDstCalcul = dynamic_cast<CCalculator*>(iter_Dst->second->Get_Component(L"Com_Calculator", ID_STATIC));

			//�÷��̾� Ʈ������
			CTransform* pDstTransform = dynamic_cast<CTransform*>(pDst.second->Get_Component(L"Com_Transform", ID_DYNAMIC));
			_vec3 vDstPos;
			pDstTransform->Get_INFO(INFO_POS, &vDstPos);

			for (auto& pSrc : _Src)
			{
				auto& iter_Src = find_if(pSrc.second->GetColmap().begin(), pSrc.second->GetColmap().end(), CTag_Finder(wstrSrcTag.c_str()));
				if (iter_Src == pSrc.second->GetColmap().end())
					continue;
				CColliderSphere* pSrcCol = dynamic_cast<CColliderSphere*>(iter_Src->second->Get_Component(L"Com_Collider", ID_STATIC));
				CCalculator* pSrcCal = dynamic_cast<CCalculator*>(iter_Src->second->Get_Component(L"Com_Calculator", ID_STATIC));
				CTransform* pSrcTransform = dynamic_cast<CTransform*>(pSrc.second->Get_Component(L"Com_Transform", ID_DYNAMIC));
				_vec3 vSrcPos;
				pSrcTransform->Get_INFO(INFO_POS, &vSrcPos);

				_float fDstRadi = *pDstCol->Get_Radius();
				_float fSrcRadi = *pSrcCol->Get_Radius();


				fDstRadi -= 150.f;
				
				//fSrcRadi -= 200.f;
				//===========================�о�� ====================================
				if (pDstCalcul->Collision_Sphere(pDstCol->Get_Center(), &fDstRadi, pSrcCol->Get_Center(), &fSrcRadi, eMesh))
				{
					//_float fRadi = (*pDstCol->Get_Radius() - *pSrcCol->Get_Radius());
					//fRadi *= 0.01f;
					_float fTimer = pDstTransform->Get_TransTimer();
					_vec3 vDstDir = vSrcPos - vDstPos;
					_vec3 vSrcDir = vDstPos - vSrcPos;

					_float fAccel = pDstTransform->Get_Accel();
					if (fAccel == 1.f)
					{
						fAccel *= 5.f;
					}
					_float floorAccel = floor(fAccel * 1.f + 0.5f) / 1.f;

					D3DXVec3Normalize(&vSrcDir, &vSrcDir);
					pDstTransform->Move_Pos(&vSrcDir, floorAccel, fTimer);
					//pDstTransform->Reset_Accel();
					//pSrcTransform->Move_Pos(&vSrcDir, fRadi, 0.001f);
					//���� �浹 �� 
					//iter_Dst->second->SetCol(true);
					//iter_Src->second->SetCol(true);

				}
				else
				{
					pDstTransform->Reset_Accel();
				}
	
				//===========================��ȣ�ۿ� ====================================
				fDstRadi += 400.f;
				fSrcRadi += 400.f;
				if (pDstCalcul->Collision_Sphere(pDstCol->Get_Center(), &fDstRadi, pSrcCol->Get_Center(), &fSrcRadi, eMesh))
				{
					iter_Dst->second->SetCol(true);
					iter_Src->second->SetCol(true);
				}
				else
				{
					iter_Dst->second->SetCol(false);
					//���ʹ� ü�̽��� ��Ǯ������ 
					//iter_Src->second->SetCol(false);

				}
				
			}
		}
	}

	//else if (eColCheck == CHECK_WEAPONE_BODY)
	//{
	//	for (auto& pDst : _Dst)
	//	{
	//		auto& iter_Dst = find_if(pDst.second->GetColmap().begin(), pDst.second->GetColmap().end(), CTag_Finder(wstrDstTag.c_str()));
	//		if (iter_Dst == pDst.second->GetColmap().end())
	//			continue;
	//		CColliderSphere* pDstCol = dynamic_cast<CColliderSphere*>(iter_Dst->second->Get_Component(L"Com_Collider", ID_STATIC));
	//		CCalculator* pDstCalcul = dynamic_cast<CCalculator*>(iter_Dst->second->Get_Component(L"Com_Calculator", ID_STATIC));
	//		CTransform* pDstTransform = dynamic_cast<CTransform*>(pDst.second->Get_Component(L"Com_Transform", ID_DYNAMIC));
	//		_vec3 vDstPos;
	//		pDstTransform->Get_INFO(INFO_POS, &vDstPos);

	//		for (auto& pSrc : _Src)
	//		{
	//			auto& iter_Src = find_if(pSrc.second->GetColmap().begin(), pSrc.second->GetColmap().end(), CTag_Finder(wstrSrcTag.c_str()));
	//			if (iter_Src == pSrc.second->GetColmap().end())
	//				continue;
	//			CColliderSphere* pSrcCol = dynamic_cast<CColliderSphere*>(iter_Src->second->Get_Component(L"Com_Collider", ID_STATIC));
	//			CCalculator* pSrcCal = dynamic_cast<CCalculator*>(iter_Src->second->Get_Component(L"Com_Calculator", ID_STATIC));
	//			CTransform* pSrcTransform = dynamic_cast<CTransform*>(pSrc.second->Get_Component(L"Com_Transform", ID_DYNAMIC));
	//			_vec3 vSrcPos;
	//			pSrcTransform->Get_INFO(INFO_POS, &vSrcPos);

	//			_float fDstRadi = *pDstCol->Get_Radius();
	//			_float fSrcRadi = *pSrcCol->Get_Radius();

	//			//===========================���� �浹 ====================================
	//			if (pDstCalcul->Collision_Sphere(pDstCol->Get_Center(), &fDstRadi, pSrcCol->Get_Center(), &fSrcRadi, eMesh))
	//			{
	//				
	//				iter_Dst->second->SetCol(true);
	//				iter_Src->second->SetCol(true);
	//			}
	//			else
	//			{
	//				iter_Dst->second->SetCol(false);
	//				//iter_Src->second->SetCol(false);
	//			}

	//		}
	//	}
	//}
	else if (eColCheck == CHECK_BULLET)
	{
		for (auto& pDst : _Dst)
		{	
			if(!pDst.second->GetActive())
				continue;
			for (auto& pSrc : _Src)
			{
				//������ ����
				if (ColCheckBullet(pSrc.second, L"Col_Left", pDst.second, eMesh))
				{
					pSrc.second->TakeDmg(pDst.second->GetAtk());
				}
				else if (ColCheckBullet(pSrc.second, L"Col_Right", pDst.second, eMesh))
				{
					pSrc.second->TakeDmg(pDst.second->GetAtk());
				}
				else if (ColCheckBullet(pSrc.second, L"Col_Front", pDst.second, eMesh))
				{
					pSrc.second->TakeDmg(pDst.second->GetAtk());
				}
				else if (ColCheckBullet(pSrc.second, L"Col_Back", pDst.second, eMesh))
				{
					pSrc.second->TakeDmg(pDst.second->GetAtk());
				}

			}
		
		}
	}
	else if (eColCheck == CHECK_WEAPON)
	{
		for (auto& pDst : _Dst)
		{	
			_uint idx = 0;
			wstring CheckKey = pDst.first;
			if (L"Col" == CheckKey.substr(0, 3))
				continue;
			while (true)
			{

				auto& iter_Dst = find_if(pDst.second->GetColmap().begin(), pDst.second->GetColmap().end(), CTag_Finder((wstrDstTag + to_wstring(idx)).c_str()));
				if (iter_Dst == pDst.second->GetColmap().end())
					break;

				for (auto& pSrc : _Src)
				{
					wstring CheckKey = pSrc.first;
					 if(L"Col"==CheckKey.substr(0, 3))
						 continue;
					
						//������ ����
					if (ColCheckWeapon(pSrc.second, L"Col_Left", iter_Dst->second, eMesh))
					{
						pSrc.second->TakeDmg(pDst.second->GetAtk());
					}
					else if (ColCheckWeapon(pSrc.second, L"Col_Right", iter_Dst->second, eMesh))
					{
						pSrc.second->TakeDmg(pDst.second->GetAtk());
					}
					else if (ColCheckWeapon(pSrc.second, L"Col_Front", iter_Dst->second, eMesh))
					{
						pSrc.second->TakeDmg(pDst.second->GetAtk());
					}
					else if (ColCheckWeapon(pSrc.second, L"Col_Back", iter_Dst->second, eMesh))
					{
						pSrc.second->TakeDmg(pDst.second->GetAtk());
					}
					
				}
				idx++;
			}
		}
	}
	else if (eColCheck == CHECK_ITEM)
	{
		for (auto& pDst : _Dst)
		{
			//������ �浹ü
			auto& iter_Dst = find_if(pDst.second->GetColmap().begin(), pDst.second->GetColmap().end(), CTag_Finder(wstrDstTag.c_str()));
			if (iter_Dst == pDst.second->GetColmap().end())
				continue;

			//�浹ü ���� 
			CColliderSphere* pDstCol = dynamic_cast<CColliderSphere*>(iter_Dst->second->Get_Component(L"Com_Collider", ID_STATIC));
			CCalculator* pDstCalcul = dynamic_cast<CCalculator*>(iter_Dst->second->Get_Component(L"Com_Calculator", ID_STATIC));

			for (auto& pSrc : _Src)
			{
				if(!pSrc.second->GetActive())
					continue;
				CColliderSphere* pSrcCol = dynamic_cast<CColliderSphere*>(pSrc.second->Get_Component(L"Com_Collider", ID_STATIC));
				CCalculator* pSrcCal = dynamic_cast<CCalculator*>(pSrc.second->Get_Component(L"Com_Calculator", ID_STATIC));
	

				_float fDstRadi = *pDstCol->Get_Radius();
				_float fSrcRadi = *pSrcCol->Get_Radius();

				//��ȣ�ۿ�
				//fSrcRadi += 100.f;
				if (pDstCalcul->Collision_Sphere(pDstCol->Get_Center(), &fDstRadi, pSrcCol->Get_Center(), &fSrcRadi, eMesh))
				{
					//iter_Dst->second->SetCol(true);
					pSrc.second->SetCol(true);
				}
			}
		}
	}
	else if (eColCheck == CHECK_INTERACTION)
	{
		for (auto& pDst : _Dst)
		{
			//������ �浹ü
			auto& iter_Dst = find_if(pDst.second->GetColmap().begin(), pDst.second->GetColmap().end(), CTag_Finder(wstrDstTag.c_str()));
			if (iter_Dst == pDst.second->GetColmap().end())
				continue;

			//�浹ü ���� 
			CColliderSphere* pDstCol = dynamic_cast<CColliderSphere*>(iter_Dst->second->Get_Component(L"Com_Collider", ID_STATIC));
			CCalculator* pDstCalcul = dynamic_cast<CCalculator*>(iter_Dst->second->Get_Component(L"Com_Calculator", ID_STATIC));

			for (auto& pSrc : _Src)
			{


				auto& iter_Src = find_if(pSrc.second->GetColmap().begin(), pSrc.second->GetColmap().end(), CTag_Finder(wstrSrcTag.c_str()));
				if (iter_Src == pSrc.second->GetColmap().end())
					continue;

				CColliderSphere* pSrcCol = dynamic_cast<CColliderSphere*>(iter_Src->second->Get_Component(L"Com_Collider", ID_STATIC));
				CCalculator* pSrcCal = dynamic_cast<CCalculator*>(iter_Src->second->Get_Component(L"Com_Calculator", ID_STATIC));


				_float fDstRadi = *pDstCol->Get_Radius();
				_float fSrcRadi = *pSrcCol->Get_Radius();

				if (pDstCalcul->Collision_Sphere(pDstCol->Get_Center(), &fDstRadi, pSrcCol->Get_Center(), &fSrcRadi, eMesh))
				{
					//iter_Dst->second->SetCol(true);
					iter_Src->second->SetCol(true);
				}
				else
					iter_Src->second->SetCol(false);
			}
		}
	}
	else if (eColCheck == CHECK_JUMPBAll)
	{
		for (auto& pDst : _Dst)
		{
			//������ �浹ü
			auto& iter_Dst = find_if(pDst.second->GetColmap().begin(), pDst.second->GetColmap().end(), CTag_Finder(wstrDstTag.c_str()));
			if (iter_Dst == pDst.second->GetColmap().end())
				continue;

			//�浹ü ���� 
			CColliderSphere* pDstCol = dynamic_cast<CColliderSphere*>(iter_Dst->second->Get_Component(L"Com_Collider", ID_STATIC));
			CCalculator* pDstCalcul = dynamic_cast<CCalculator*>(iter_Dst->second->Get_Component(L"Com_Calculator", ID_STATIC));

			for (auto& pSrc : _Src)
			{
				if (!pSrc.second->GetActive())
					continue;
				CColliderSphere* pSrcCol = dynamic_cast<CColliderSphere*>(pSrc.second->Get_Component(L"Com_Collider", ID_STATIC));
				CCalculator* pSrcCal = dynamic_cast<CCalculator*>(pSrc.second->Get_Component(L"Com_Calculator", ID_STATIC));


				_float fDstRadi = *pDstCol->Get_Radius();
				_float fSrcRadi = *pSrcCol->Get_Radius();

				//��ȣ�ۿ�
				//fSrcRadi += 100.f;
				if (pDstCalcul->Collision_Sphere(pDstCol->Get_Center(), &fDstRadi, pSrcCol->Get_Center(), &fSrcRadi, eMesh))
				{
					//iter_Dst->second->SetCol(true);
					pSrc.second->SetCol(true);
				}
				else {
					pSrc.second->SetCol(false);
				}
			}
		}
	}
}

void CColMgr::SetColType(COLCHECK eColCheck, wstring* pDstTag, wstring* pSrcTag)
{
	switch (eColCheck)
	{
	case Engine::CHECK_BODY_BODY:
		*pDstTag = L"Col_Body";
		*pSrcTag = L"Col_Body";
		break;
	case Engine::CHECK_WEAPONE_BODY:
		*pDstTag = L"Col_Weapon";
		*pSrcTag = L"Col_Body";
		break;
	case Engine::CHECK_BODY_BOSSTENTA:
		*pDstTag = L"Col_Body";
		*pSrcTag = L"Col_Body";		//�̰͸� ���� ��ŸŬ�� ���� 
		//����
		break;
	case Engine::CHECK_WEAPON:
		*pDstTag = L"Col_Weapon";
		*pSrcTag = L"";
		break;
	case Engine::CHECK_BULLET:
		break;
	case Engine::CHECK_ITEM:
		*pDstTag = L"Col_Body";
		*pSrcTag = L"";
		break;
	case Engine::CHECK_INTERACTION:
		*pDstTag = L"Col_Body";
		*pSrcTag = L"Col_Body";
		break;
	case Engine::CHECK_JUMPBAll:
		*pDstTag = L"Col_Body";
		*pSrcTag = L"";
		break;
	case Engine::CHECK_END:
		break;
	default:
		break;
	}
}

//���� Ȱ��ȭ�϶�
//���� ��Ʈ�� false��
_bool CColMgr::ColCheckWeapon(CGameObject* pSrcObj, wstring ColTag,CGameObject * pDstColObj,MESHTYPE eMesh)
{

	CColliderSphere* pDstCol = dynamic_cast<CColliderSphere*>(pDstColObj->Get_Component(L"Com_Collider", ID_STATIC));
	CCalculator* pDstCalcul = dynamic_cast<CCalculator*>(pDstColObj->Get_Component(L"Com_Calculator", ID_STATIC));

	auto& iter_Src = find_if(pSrcObj->GetColmap().begin(), pSrcObj->GetColmap().end(), CTag_Finder(ColTag.c_str()));
	if (iter_Src == pSrcObj->GetColmap().end())
		return false;

	CColliderSphere* pSrcCol = dynamic_cast<CColliderSphere*>(iter_Src->second->Get_Component(L"Com_Collider", ID_STATIC));
	CCalculator* pSrcCal = dynamic_cast<CCalculator*>(iter_Src->second->Get_Component(L"Com_Calculator", ID_STATIC));

	_float fDstRadi = *pDstCol->Get_Radius();
	_float fSrcRadi = *pSrcCol->Get_Radius();

	//===========================���� �浹 ====================================
	if(pDstColObj->GetActive())
	{
		if (!pSrcObj->GetHit())
		{
			if (pDstCalcul->Collision_Sphere(pDstCol->Get_Center(), &fDstRadi, pSrcCol->Get_Center(), &fSrcRadi, eMesh))
			{
				pSrcObj->SetColTarget(pDstColObj);
				//iter_Src->second->SetColTarget(pDstColObj);
				iter_Src->second->SetCol(true);
				return true;
			}
		}
	}
	return false;
}

_bool CColMgr::ColCheckBullet(CGameObject * pSrcObj, wstring ColTag, CGameObject * pDstColObj, MESHTYPE eMesh)
{
	CColliderSphere* pDstCol = dynamic_cast<CColliderSphere*>(pDstColObj->Get_Component(L"Com_Collider", ID_STATIC));
	CCalculator* pDstCalcul = dynamic_cast<CCalculator*>(pDstColObj->Get_Component(L"Com_Calculator", ID_STATIC));

	auto& iter_Src = find_if(pSrcObj->GetColmap().begin(), pSrcObj->GetColmap().end(), CTag_Finder(ColTag.c_str()));
	if (iter_Src == pSrcObj->GetColmap().end())
		return false;

	CColliderSphere* pSrcCol = dynamic_cast<CColliderSphere*>(iter_Src->second->Get_Component(L"Com_Collider", ID_STATIC));
	CCalculator* pSrcCal = dynamic_cast<CCalculator*>(iter_Src->second->Get_Component(L"Com_Calculator", ID_STATIC));

	_float fDstRadi = pDstColObj->GetColShpereRadius();
	_float fSrcRadi = *pSrcCol->Get_Radius();

	_vec3 vDstPos;
	dynamic_cast<CTransform*>(pDstColObj->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_INFO(INFO_POS,&vDstPos);
	//===========================���� �浹 ====================================
	if (pDstColObj->GetActive())
	{
		if (!pSrcObj->GetHit())
		{
			if (pDstCalcul->Collision_Sphere(&vDstPos, &fDstRadi, pSrcCol->Get_Center(), &fSrcRadi, eMesh))
			{
				pSrcObj->SetColTarget(pDstColObj);
				iter_Src->second->SetCol(true);
				return true;
			}
		}
	}
	return false;
}
