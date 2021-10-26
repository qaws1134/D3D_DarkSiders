#include "NaviMesh.h"
#include "RayPickManager.h"
USING(Engine)

Engine::CNaviMesh::CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_dwIndex(0)
{

}

Engine::CNaviMesh::CNaviMesh(const CNaviMesh& rhs)
	: CComponent(rhs.m_pGraphicDev)
	, m_dwIndex(rhs.m_dwIndex)
	, m_vecCell(rhs.m_vecCell)
{
	for (auto& iter : m_vecCell)
		iter->AddRef();
}

Engine::CNaviMesh::~CNaviMesh(void)
{

}

void CNaviMesh::Set_CellUpdate(_ulong dwIndex, _vec3 vPosA, _vec3 vPosB, _vec3 vPosC)
{
	m_vecCell[dwIndex]->Set_Line(&vPosA, &vPosB, &vPosC);
}

void CNaviMesh::Add_Cell( _vec3 vPosA, _vec3 vPosB, _vec3 vPosC)
{
	CCell*		pCell = nullptr;
	pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &vPosA, &vPosB, &vPosC);
	NULL_CHECK_RETURN(pCell, );
	m_vecCell.push_back(pCell);
	FAILED_CHECK_RETURN(Link_Cell(), );
}

HRESULT Engine::CNaviMesh::Ready_NaviMesh(void)
{

	//m_vecCell.reserve(4);

	//CCell*		pCell = nullptr;

	//// 0
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(0.f, 0.f, 2.f), &_vec3(2.f, 0.f, 0.f), &_vec3(0.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//// 1
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(0.f, 0.f, 2.f), &_vec3(2.f, 0.f, 2.f), &_vec3(2.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//// 2
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(0.f, 0.f, 4.f), &_vec3(2.f, 0.f, 2.f), &_vec3(0.f, 0.f, 2.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//// 3
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(2.f, 0.f, 2.f), &_vec3(4.f, 0.f, 0.f), &_vec3(2.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//FAILED_CHECK_RETURN(Link_Cell(), E_FAIL);

	return S_OK;
}
HRESULT Engine::CNaviMesh::Ready_NaviMesh(map<_ulong, map<_ulong, CCell*>> mapNavi)
{

	m_vecCell.reserve(mapNavi.size());

	CCell*		pCell = nullptr;


	for (auto iter : mapNavi)
	{
		for (auto iter_Cell : iter.second)
		{
			m_vecCell.emplace_back(iter_Cell.second);
		}
	}
	//// 0
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(0.f, 0.f, 2.f), &_vec3(2.f, 0.f, 0.f), &_vec3(0.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//// 1
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(0.f, 0.f, 2.f), &_vec3(2.f, 0.f, 2.f), &_vec3(2.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//// 2
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(0.f, 0.f, 4.f), &_vec3(2.f, 0.f, 2.f), &_vec3(0.f, 0.f, 2.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//// 3
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(2.f, 0.f, 2.f), &_vec3(4.f, 0.f, 0.f), &_vec3(2.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);
	FAILED_CHECK_RETURN(Link_Cell(), E_FAIL);


	return S_OK;
}
void CNaviMesh::Render_NaviMesh(void)
{
	for (auto& iter : m_vecCell)
		iter->Render_Cell();
}

_vec3 CNaviMesh::MoveOn_NaviMesh(const _vec3 * pTargetPos, const _vec3 * pTargetDir, 
								const _float & fSpeed, const _float & fTimeDelta)
{
	_vec3		vEndPos = *pTargetPos + (*pTargetDir * fSpeed * fTimeDelta);

	if (CCell::COMPARE_MOVE == m_vecCell[m_dwIndex]->Compare_Position(&vEndPos, &m_dwIndex))
		return vEndPos;

	else if (CCell::COMPARE_STOP == m_vecCell[m_dwIndex]->Compare_Position(&vEndPos, &m_dwIndex))
		return *pTargetPos;

	return _vec3();
}

//_bool CNaviMesh::PickOn_NaviMesh(const _vec2  vMousPos, const _vec2  vWindowSize)
//{
//	RAY rayMouse = CRayPickManager::GetInstance()->Create_MouseRay(_vec2((_float)vMousPos.x, (_float)vMousPos.y), _vec2((_float)vWindowSize.x, (_float)vWindowSize.y));
//	_vec3 vPos = rayMouse.vRayPos;
//
//	return false;
//
//}
HRESULT Engine::CNaviMesh::Link_Cell(void)
{
	for (_ulong i = 0; i < m_vecCell.size(); ++i)
	{
		for (_ulong j = 0; j < m_vecCell.size(); ++j)
		{
			if (i == j)
				continue;

			// 1. 얻어온 이웃 셀의 메모리 정보가 null인 경우
			// 2. 얻어온 이웃 셀의 두 점이 같은 두 점을 공유하는 경우
			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_AB) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_A),
													m_vecCell[i]->Get_Point(CCell::POINT_B),
													m_vecCell[i]))
				{
					m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_AB, m_vecCell[j]);
					continue;
				}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_BC) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_B),
													m_vecCell[i]->Get_Point(CCell::POINT_C),
													m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_BC, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_CA) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_C),
													m_vecCell[i]->Get_Point(CCell::POINT_A),
													m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_CA, m_vecCell[j]);
				continue;
			}
		}
	}

	return S_OK;
}

Engine::CNaviMesh* Engine::CNaviMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNaviMesh*	pInstance = new CNaviMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_NaviMesh()))
		Safe_Release(pInstance);

	return pInstance;
}
//Engine::CNaviMesh* Engine::CNaviMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev,map<_ulong,map<_ulong, CCell*>> m_mapCell)
//{
//	CNaviMesh*	pInstance = new CNaviMesh(pGraphicDev);
//
//	if (FAILED(pInstance->Ready_NaviMesh(m_mapCell)))
//		Safe_Release(pInstance);
//
//	return pInstance;
//}

Engine::CComponent* Engine::CNaviMesh::Clone(void)
{
	return new CNaviMesh(*this);
}

void Engine::CNaviMesh::Free(void)
{
	for_each(m_vecCell.begin(), m_vecCell.end(), CDeleteObj());
	m_vecCell.clear();

	CComponent::Free();
}

