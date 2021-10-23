#include "Transform.h"

USING(Engine)

Engine::CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	:	CComponent(pGraphicDev)
	, m_vAngle(0.f, 0.f, 0.f)
	, m_vScale(1.f, 1.f, 1.f)
{
	D3DXMatrixIdentity(&m_matWorld);
}

Engine::CTransform::CTransform(const CTransform& rhs)
	: CComponent(rhs)
	, m_vScale(rhs.m_vScale)
	, m_vAngle(rhs.m_vAngle)
{
	for (_uint i = 0; i < INFO_END; ++i)
		m_vInfo[i] = rhs.m_vInfo[i];

	m_matWorld = rhs.m_matWorld;
}

Engine::CTransform::~CTransform(void)
{

}

HRESULT Engine::CTransform::Ready_Transform(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	return S_OK;
}

Engine::_int Engine::CTransform::Update_Component(const _float& fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_uint i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	// 크기적용
	for (_uint i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);	
	}

	// 회전적용
	_matrix		matRot[ROT_END];
	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		for (_uint j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}

	// 위치 변환은 Set_Pos 함수를 통해서 직접 대입하기 때문에 다른 벡터들을 가공할 이유가 없다

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	return 0;
}

void CTransform::Get_WorldMatrix(_matrix * pMatrix) const
{
	*pMatrix = m_matWorld;
}

void Engine::CTransform::Set_Scale(const _float& fX, const _float& fY, const _float& fZ)
{
	m_vScale.x = fX;
	m_vScale.y = fY;
	m_vScale.z = fZ;
}

void CTransform::Set_Scale(const _vec3 * vScale)
{
	m_vScale = *vScale;
}

void CTransform::Set_Pos(const _float & fX, const _float & fY, const _float & fZ)
{
	m_vInfo[INFO_POS].x = fX;
	m_vInfo[INFO_POS].y = fY;
	m_vInfo[INFO_POS].z = fZ;
}

void CTransform::Set_Pos(const _vec3* pPos)
{
	m_vInfo[INFO_POS] = *pPos;
}

void CTransform::Set_Rot(const _vec3 * vAngle)
{
	m_vAngle = *vAngle;
}

void CTransform::Set_Rot(const _float & fX, const _float & fY, const _float & fZ)
{
	m_vAngle.x = fX;
	m_vAngle.y = fY;
	m_vAngle.z = fZ;

}

void Engine::CTransform::Set_WorldMatrix(const _matrix* pWorld)
{
	m_matWorld = *pWorld;
}

void Engine::CTransform::Move_Pos(const _vec3* pDir, const _float& fSpeed, const _float& fTimeDelta)
{
	m_vInfo[INFO_POS] += *pDir * fSpeed * fTimeDelta;
}

void Engine::CTransform::Rotation(ROTATION eType, const _float& fAngle)
{
	*(((_float*)&m_vAngle) + eType) += fAngle;
}

void Engine::CTransform::Get_INFO(INFO eType, _vec3* pInfo)
{
	memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3));
}



Engine::CTransform* Engine::CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform*		pInstance = new CTransform(pGraphicDev);

	if (FAILED(pInstance->Ready_Transform()))
		Safe_Release(pInstance);

	return pInstance;
}


void Engine::CTransform::Free(void)
{
	CComponent::Free();
}

Engine::CComponent* Engine::CTransform::Clone(void)
{
	return new CTransform(*this);
}

