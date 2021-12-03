#include "stdafx.h"
#include "Effect_Trail.h"
#include "Define.h"
#include "Export_Function.h"
#include "GameMgr.h"

CEffect_Trail::CEffect_Trail(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CEffect_Trail::CEffect_Trail(const CEffect_Trail& rhs)
	: CGameObject(rhs)
	, m_pmatPlayerInfo(rhs.m_pmatPlayerInfo)
	, m_pmatWeaponRef(rhs.m_pmatWeaponRef)
	, m_fAccTime(rhs.m_fAccTime)
	,m_tTrailInfo(rhs.m_tTrailInfo)
{


}

CEffect_Trail::~CEffect_Trail(void)
{

}


HRESULT CEffect_Trail::Ready_Object(void)
{
	if (Engine::CGameObject::Ready_Object())
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;


	m_pTransformCom->Set_Scale(&_vec3(1.f, 1.f, 1.f));
	m_pTransformCom->Set_Pos(&_vec3(0.f, 0.f, 0.f));
	m_tTrailInfo.iTrailCount = 30;
	m_tTrailInfo.fTrailEmitRate = 0.0015f;
	m_tTrailInfo.fTrailSize = 400.f;
	m_tTrailInfo.vColor = { 0.2f,0.7f,0.2f,0.7f };


	return NOERROR;
}

_int CEffect_Trail::Update_Object(const _float & fTimeDelta)
{
	_vec3 vPos;
	m_pTransformCom->Get_INFO(INFO_POS, &vPos);
	Engine::CGameObject::Update_Object(fTimeDelta);

	if (m_bActive)
	{
		if (nullptr != m_pRendererCom)
			Add_RenderGroup(RENDER_EFFECT, this);
	}
	Compute_ViewZ(&vPos);
	Memorize_Point(fTimeDelta);

	m_bActive = false;
	return 0;

}

void CEffect_Trail::Render_Object(void)
{
	if (nullptr == m_pShaderCom || m_Pointlist.size() <= 3)
		return;
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return;

	_uint iPass;
	pEffect->AddRef();

	if (FAILED(Set_ConstantTable(pEffect)))
		return;

	pEffect->Begin(nullptr, 0);
	
	
	if (1==m_tTrailInfo.iTextureNum)
	{
		iPass = 3;
	}
	else
	{
		iPass = 2;
	}
	
	
	pEffect->BeginPass(iPass); // Pass1 (검은색 제거)
	
	




	D3DXVECTOR3*		pPoint = new D3DXVECTOR3[m_Pointlist.size()];

	POINTLIST::iterator		iter = m_Pointlist.begin();
	POINTLIST::iterator		iter_end = m_Pointlist.end();

	int		iIndex = 0;
	for (; iter != iter_end; ++iter)
	{
		pPoint[iIndex] = (*iter);
		++iIndex;
	}

	m_pBufferCom->SetVertexTrail(pPoint, m_Pointlist.size());
	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
	Engine::Safe_Delete_Array(pPoint);
}


HRESULT CEffect_Trail::Add_Component(void)
{
	CComponent*		pComponent = nullptr;


	// Shader
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Prototype(L"Proto_Shader_EffectTrail"));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	// Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(m_pRendererCom, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);


	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Prototype(L"Proto_Texture_Trail"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", pComponent);


	pComponent = m_pBufferCom = dynamic_cast<CTrail_Texture*>(Clone_Prototype(L"Proto_Buffer_Trail"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);




	return S_OK;

}

void CEffect_Trail::SetOption(void * pArg)
{
	if (pArg)
	{
		memcpy(&m_tTrailInfo, pArg, sizeof(TRAIL));
	}
	m_bActive = true;
	//텍스쳐?,
	//선택 -> 기본셋팅
	//공격하면서 유동적 변경


}

void CEffect_Trail::Memorize_Point(const float & fTimeDelta)
{
	while (m_Pointlist.size() >= (unsigned int)m_tTrailInfo.iTrailCount)
	{
		m_Pointlist.pop_front();
	}

	m_fAccTime += fTimeDelta;

	if (m_tTrailInfo.fTrailEmitRate < m_fAccTime)
	{
		D3DXVECTOR3		vPoint[2];

		float Point0 = -80.f ;
		float Point1 = -m_tTrailInfo.fTrailSize;

		vPoint[0] = D3DXVECTOR3(0.f, Point0, 0.f);
		vPoint[1] = D3DXVECTOR3(-0.f, Point1, 0.f);
		if (m_pmatWeaponRef&&m_pmatPlayerInfo)
		{
			for (_uint i = 0; i < 2; i++)
			{
				D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], m_pmatWeaponRef);
				D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], m_pmatPlayerInfo);
				m_Pointlist.push_back(vPoint[i]);

			}
		}

		m_fAccTime = 0.f;
	}

}

HRESULT CEffect_Trail::Set_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect
		|| nullptr == m_pTransformCom
		|| nullptr == m_pTextureCom)
		return E_FAIL;

	_matrix		matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", m_tTrailInfo.iTextureNum); // Textrue 번호
	m_pTextureCom->Set_Texture(pEffect, "g_BackTexture", 4);
	pEffect->SetVector("g_vColor", &m_tTrailInfo.vColor);

	return NOERROR;
}

CEffect_Trail* CEffect_Trail::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{	
	CEffect_Trail* pInstance = new CEffect_Trail(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		pInstance->Release();
	}
	return pInstance;
}

void CEffect_Trail::Free(void)
{
	CGameObject::Free();
}
