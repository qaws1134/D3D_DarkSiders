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


			/*	fDstRadi -= 200.f;
				fSrcRadi -= 200.f;*/
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
				fDstRadi += 300.f;
				fSrcRadi += 300.f;
				if (pDstCalcul->Collision_Sphere(pDstCol->Get_Center(), &fDstRadi, pSrcCol->Get_Center(), &fSrcRadi, eMesh))
				{
					iter_Dst->second->SetCol(true);
					iter_Src->second->SetCol(true);
				}
				else
				{
					iter_Dst->second->SetCol(false);
					iter_Src->second->SetCol(false);

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
	else if (eColCheck == CHECK_WEAPON)
	{
		_uint idx = 0;
		for (auto& pDst : _Dst)
		{	
			while (true)
			{
				auto& iter_Dst = find_if(pDst.second->GetColmap().begin(), pDst.second->GetColmap().end(), CTag_Finder((wstrDstTag + to_wstring(idx)).c_str()));
				if (iter_Dst == pDst.second->GetColmap().end())
					break;

				for (auto& pSrc : _Src)
				{
					//��Ʈ���°� �ƴҶ�
					if (!pSrc.second->GetHit())
					{
						//������ ����
						if (ColCheck(pSrc.second, L"Col_Left", iter_Dst->second, eMesh))
						{
							pSrc.second->TakeDmg(pDst.second->GetDmg());
						}
						else if (ColCheck(pSrc.second, L"Col_Right", iter_Dst->second, eMesh))
						{
							pSrc.second->TakeDmg(pDst.second->GetDmg());
						}
						else if (ColCheck(pSrc.second, L"Col_Front", iter_Dst->second, eMesh))
						{
							pSrc.second->TakeDmg(pDst.second->GetDmg());
						}
						else if (ColCheck(pSrc.second, L"Col_Back", iter_Dst->second, eMesh))
						{
							pSrc.second->TakeDmg(pDst.second->GetDmg());
						}
					}
				}
				idx++;
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
	case Engine::CHECK_END:
		break;
	default:
		break;
	}
}

_bool CColMgr::ColCheck(CGameObject* pSrcObj, wstring ColTag,CGameObject * pDstColObj,MESHTYPE eMesh)
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
		if (pDstCalcul->Collision_Sphere(pDstCol->Get_Center(), &fDstRadi, pSrcCol->Get_Center(), &fSrcRadi, eMesh))
		{
			iter_Src->second->SetCol(true);
			return true;
		}
	}
	return false;
}
