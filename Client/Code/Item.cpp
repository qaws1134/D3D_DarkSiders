#include "stdafx.h"
#include "Item.h"
#include "Export_Function.h"
#include "GameMgr.h"
#include "UIMgr.h"
#include "SoundMgr.h"
CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CItem::CItem(const CItem& rhs)
	: CGameObject(rhs)
{

}

CItem::~CItem(void)
{
}

HRESULT CItem::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_pTransformCom->Set_Pos(5.f, 23.f,5.f);
	m_pTransformCom->Update_Component(0.f);

	return S_OK;
}

_int CItem::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);
	if (m_bActive)
	{
		//LifeTime(fTimeDelta);
	
		BillBord();
		_vec3	vPos;
		_vec3 vInitPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vInitPos);
		m_pColliderCom->Set_Center(vInitPos);

		if (!m_bMoveEnd)
		{
			if (m_pNavi)
			{
				D3DXVec3Normalize(&m_vDir, &m_vDir);
				vPos = m_pNavi->MoveOn_NaviMesh(&vInitPos, &m_vDir, m_fSpeed, fTimeDelta, m_pCalculatorCom);
				m_fNaviY = vPos.y+2.f;

				if (!m_bApexEnd)
				{
					if (m_pTransformCom->MoveStep(MOVETYPE_BREAK, &m_fSpeed, 20.f, 0.f, &_vec3(0.f, 1.f, 0.f), fTimeDelta))
					{
						m_bApexEnd = true;
					}
				}
				else
				{
					m_pTransformCom->MoveStep(MOVETYPE_ACC, &m_fSpeed, 80.f, 150.f, &_vec3(0.f, -1.f, 0.f), fTimeDelta);

					if (vInitPos.y <= m_fNaviY)
					{
						//리턴
						m_bMoveEnd = true;
					}
				}
				
			}
			m_pTransformCom->Move_Pos(&m_vDir, m_fMoveSpeed);
		}
		else
		{
			//이동이 true일떄 

			_vec3 vTargetPos;
			dynamic_cast<CTransform*>(CGameMgr::GetInstance()->GetPlayer()->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_INFO(INFO_POS, &vTargetPos);
			m_vDir = vTargetPos - vInitPos;

			_float fDis= D3DXVec3Length(&m_vDir);
			if (fDis < 5.f)
			{
				D3DXVec3Normalize(&m_vDir, &m_vDir);
				m_fChaseSpeed += fTimeDelta;
				m_pTransformCom->Move_Pos(&m_vDir, m_fChaseSpeed);
			}
	

			if (fDis < 1.f)
			{
				if (m_bCol)
				{
					//아이템 판단
					switch (m_tItem.eType)
					{
					case DROPITEM::ITEM_STONE:
						CUIMgr::GetInstance()->SetStoneInfoUI(m_pGraphicDev, CGameMgr::GetInstance()->GetStone(m_tStone.eCreature));
						CUIMgr::GetInstance()->SetStoneListUI(m_pGraphicDev, CGameMgr::GetInstance()->GetStone(m_tStone.eCreature));
						m_bActive = true;
						break;
					case DROPITEM::ITEM_SOUL:
						CGameMgr::GetInstance()->TakeSoul(RandNext(5, 20));
						m_bActive = true;
						break;
					default:
						break;

					}
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_ITEM);
					CSoundMgr::Get_Instance()->PlaySound(L"prop_pickup_orb_health_01.ogg", CSoundMgr::CHANNEL_ITEM);
					CGameMgr::GetInstance()->RetunItem(this);

				}
			}
		}
		Add_RenderGroup(RENDER_EFFECT, this);
	}
	return iExit;
}

void CItem::Render_Object(void)
{

	LPD3DXEFFECT	 pItem = m_pShaderCom->Get_EffectHandle();
	pItem->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pItem), );

	_uint iMaxPass = 0;

	pItem->Begin(&iMaxPass, NULL);

	m_pColliderCom->Render_Collider(COLTYPE(m_bCol), m_pTransformCom->Get_WorldMatrix());


	pItem->BeginPass(m_iPassIdx);

	m_pBufferCom->Render_Buffer();

	pItem->EndPass();

	pItem->End();
	Safe_Release(pItem);
}

void CItem::SetOption(void * pArg)
{
	CComponent*		pComponent = nullptr;
	if (pArg)
	{
		memcpy(&m_tItem, pArg, sizeof(DROPITEMINFO));
	}	

	switch (m_tItem.eType)
	{
	case DROPITEM::ITEM_STONE:
		//m_wstrTexture = L"Proto_Texture_Stone";
		m_tStone = (STONE)CGameMgr::GetInstance()->GetStone((UI::STONE)Engine::RandNext(0,22));
		m_wstrTexture = L"Proto_Texture_CoreTree_Creature";
		m_iTextureNum = m_tStone.eCreature;
		m_bActive = true;
		m_bMoveEnd = false;
		m_fAngle = GetRandomFloat(0.f, 360.f);
		m_fSpeed = GetRandomFloat(5.f, 10.f);
		m_fMoveSpeed = GetRandomFloat(0.05f, 0.15f);
		m_fLifeSpeed = 0.f;

		break;
	case DROPITEM::ITEM_SOUL:
		m_wstrTexture = L"Proto_Texture_Store_Soul";
		m_iTextureNum = 0;
		m_bActive = true;
		m_bMoveEnd = false;
		m_pTransformCom->Set_Scale(0.5f, 0.5f, 0.5f);
		m_fAngle = GetRandomFloat(0.f, 360.f);
		m_fSpeed = GetRandomFloat(5.f, 10.f);
		m_fMoveSpeed = GetRandomFloat(0.05f, 0.15f);
		m_fLifeSpeed = 0.f;
		break;
	case DROPITEM::ITEM_END:
		m_wstrTexture = L"";
		m_iTextureNum = 0;
		m_fAngle = 0.f;
		m_fSpeed = 0.f;
		m_fMoveSpeed = 0.f;
		m_bActive = false;
		m_bMoveEnd = true;
		m_bCol = false;
		m_bApexEnd = false;
		m_fChaseSpeed = 0.f;
		m_fLifeSpeed = 10.f;
		m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
		m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);

		return;
	default:
		break;
	}

	_float fRadius = 0.3f;
	m_vDir.x = fRadius*cosf(m_fAngle) - fRadius*sinf(m_fAngle);
	m_vDir.z = fRadius*sinf(m_fAngle) + fRadius*cosf(m_fAngle);

	if (m_wstrTexture == L"")
		return;

	USES_CONVERSION;
	const _tchar* pConvComponentTag = W2BSTR((m_wstrTexture).c_str());
	
	auto& iter_find = find_if(m_mapComponent[ID_STATIC].begin(), m_mapComponent[ID_STATIC].end(), CTag_Finder(pConvComponentTag));

	if (iter_find == m_mapComponent[ID_STATIC].end())
	{
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Prototype(m_wstrTexture.c_str()));
		NULL_CHECK_RETURN(m_pTextureCom, );
		m_mapComponent[ID_STATIC].emplace(pConvComponentTag, pComponent);
	}
	else
	{
		m_pTextureCom = dynamic_cast<CTexture*>(iter_find->second);
	}

	//iter_find = find_if(m_mapComponent[ID_STATIC].begin(), m_mapComponent[ID_STATIC].end(), CTag_Finder(L"Proto_Navi"));
	//if (iter_find == m_mapComponent[ID_STATIC].end())
	//{
	//	pComponent = m_pNavi = dynamic_cast<CNaviMesh*>(Clone_Prototype(L"Proto_Navi"));
	//	NULL_CHECK_RETURN(m_pNavi, );
	//	m_mapComponent[ID_STATIC].emplace(L"Com_Navi", pComponent);
	//}
	//else
	//{
	//	m_pNavi = dynamic_cast<CNaviMesh*>(iter_find->second);
	//}

}

CItem* CItem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CItem*	pInstance = new CItem(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CItem::Free(void)
{
	if(m_bActive)
		CGameMgr::GetInstance()->RetunItem(this);

	CGameObject::Free();
}


void CItem::BillBord()
{


	CTransform* pCamTrans = dynamic_cast<CTransform*>(CGameMgr::GetInstance()->GetCamera()->Get_Component(L"Com_Transform", ID_DYNAMIC));

	_matrix *pWorld= pCamTrans->Get_WorldMatrix();

	_vec3 vPos;
	m_pTransformCom->Get_INFO(INFO_POS,&vPos);

	pWorld->_41 = vPos.x;
	pWorld->_42 = vPos.y;
	pWorld->_43 = vPos.z;


	m_pTransformCom->Set_WorldMatrix(pWorld);


}



void CItem::LifeTime(_float fTimeDelta)
{
	m_fLifeSpeed += fTimeDelta;

	if (m_fLifeSpeed > m_fLifeTime)
	{
		m_bActive = false;
	}
}

HRESULT CItem::Add_Component(void)
{
	CComponent*		pComponent = nullptr;
	
	// buffer
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Prototype(L"Proto_Buffer_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Prototype(L"Proto_Shader_Sample"));
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

	pComponent = m_pColliderCom = CColliderSphere::Create(m_pGraphicDev, &_vec3(0.f,0.f,0.f), 1.f);
	NULL_CHECK_RETURN(m_pColliderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Collider", pComponent);
	

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Calculator", pComponent);

	pComponent=	m_pNavi = dynamic_cast<CNaviMesh*>(Clone_Prototype(L"Proto_Navi"));
	NULL_CHECK_RETURN(m_pNavi,E_FAIL );
	m_mapComponent[ID_STATIC].emplace(L"Com_Navi", pComponent);
	//m_pNavi->Set_CellIndex();
	return S_OK;
}




HRESULT CItem::SetUp_ConstantTable(LPD3DXEFFECT& pItem)
{

	_matrix		matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pItem->SetMatrix("g_matWorld", &matWorld);
	pItem->SetMatrix("g_matView", &matView);
	pItem->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_Texture(pItem, "g_BaseTexture", m_iTextureNum);

	return S_OK;
}


_float CItem::GetRandomFloat(_float lowBound, _float highBound)
{
	if (lowBound >= highBound) // bad input
		return lowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}