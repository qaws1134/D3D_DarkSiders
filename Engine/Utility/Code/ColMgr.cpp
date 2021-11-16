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



				//===========================�о�� ====================================
				if (pDstCalcul->Collision_Sphere(pDstCol->Get_Center(), &fDstRadi, pSrcCol->Get_Center(), &fSrcRadi, eMesh))
				{
					_float fRadi = (*pDstCol->Get_Radius() - *pSrcCol->Get_Radius());
					fRadi *= 0.01f;
					_float fTimer = pDstTransform->Get_TransTimer();
					_vec3 vDstDir = vSrcPos - vDstPos;
					_vec3 vSrcDir = vDstPos - vSrcPos;

					_float fAccel = pDstTransform->Get_Accel()*0.9f;
					pDstTransform->Move_Pos(&vDstDir, fRadi*fAccel, 0.001f);
					pDstTransform->Reset_Accel();
					//pSrcTransform->Move_Pos(&vSrcDir, fRadi, 0.001f);
					//���� �浹 �� 
					iter_Dst->second->SetCol(true);
					iter_Src->second->SetCol(true);

				}
				else
				{
					pDstTransform->Reset_Accel();
				}
	
				//===========================��ȣ�ۿ� ====================================
				fDstRadi += 100.f;
				fSrcRadi += 100.f;
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

	else if (eColCheck == CHECK_WEAPONE_BODY)
	{
		for (auto& pDst : _Dst)
		{
			auto& iter_Dst = find_if(pDst.second->GetColmap().begin(), pDst.second->GetColmap().end(), CTag_Finder(wstrDstTag.c_str()));
			if (iter_Dst == pDst.second->GetColmap().end())
				continue;
			CColliderSphere* pDstCol = dynamic_cast<CColliderSphere*>(iter_Dst->second->Get_Component(L"Com_Collider", ID_STATIC));
			CCalculator* pDstCalcul = dynamic_cast<CCalculator*>(iter_Dst->second->Get_Component(L"Com_Calculator", ID_STATIC));
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

				//===========================���� �浹 ====================================
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
		break;
	case Engine::CHECK_END:
		break;
	default:
		break;
	}
}
