#include "stdafx.h"
#include "Angel.h"
#include "Enum.h"
#include "Export_Function.h"
#include "GameMgr.h"


CAngel::CAngel(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CAngel::CAngel(const CAngel& rhs)
	: CGameObject(rhs)
{

}

CAngel::~CAngel(void)
{

}

HRESULT CAngel::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);

	m_pTransformCom->Update_Component(0.f);
	m_pMeshCom->Set_AnimationIndex(Angel::Angel_Atk_Dragon_End);
	m_fPatternTimer = 1.f;
	m_fPatternSpeed = 0.f;
	m_eCurAniState = Angel::Angel_Atk_Dragon_End;
	m_iPatternNum = 0;
	m_fAngleSpeed = 360.f;

	m_bActive = true;

	m_eMachineState = Angel::STATE_SPAWN_IDLE;
	m_fHitSpeed = 1.0f;
	m_fHitTime = 0.1f;
	m_fAttackMoveSpeed = 5.f;
	SetCharInfo(50.f, 1.f);

	return S_OK;
}

void CAngel::Late_Ready_Object()
{
}


_int CAngel::Update_Object(const _float& fTimeDelta)
{
	m_fFrameSpeed = fTimeDelta;
	_int iExit = CGameObject::Update_Object(fTimeDelta);
	if (!m_pNavi)
	{
		m_pNavi = dynamic_cast<CNaviMesh*>(Clone_Prototype(L"Proto_Navi"));
		m_mapComponent[ID_STATIC].emplace(L"Com_Navi", m_pNavi);
	}


	DeadCheck();
	StateChange();
	StateActor(fTimeDelta);
	StateLinker(fTimeDelta);

	if(m_bActive)
		Add_RenderGroup(RENDER_NONALPHA, this);
	return iExit;
}

CAngel* CAngel::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAngel*	pInstance = new CAngel(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

CAngel * CAngel::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoTag)
{
	CAngel*	pInstance = new CAngel(pGraphicDev);
	pInstance->SetProtoTag(ProtoTag);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CAngel::Free(void)
{
	CGameObject::Free();
}


HRESULT CAngel::Add_Component()
{
	USES_CONVERSION;
	CComponent*		pComponent = nullptr;

	const _tchar* pConvProtoTag = W2BSTR(m_wstrProtoTag.c_str());

	// Mesh
	if(m_wstrProtoTag == L"")
		pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Prototype(L"Angel"));
	else
		pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Prototype(pConvProtoTag));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(m_pRendererCom, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Clone_Prototype(L"Proto_Shader_Mesh"));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);


	return S_OK;
}

void CAngel::Render_Object(void)
{
	_float fTimeDelta = Get_TimeDelta(L"Timer_Immediate");
	m_pMeshCom->Set_AnimationIndex(m_eCurAniState, m_bBlend);
	m_pMeshCom->Play_Animation(m_fFrameSpeed);

	LPD3DXEFFECT	 pEffect = m_pShaderCom->Get_EffectHandle();
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iMaxPass = 0;

	pEffect->Begin(&iMaxPass, NULL);		// 1인자 : 현재 쉐이더 파일이 반환하는 pass의 최대 개수
											// 2인자 : 시작하는 방식을 묻는 FLAG
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();
	pEffect->End();


	Safe_Release(pEffect);
}



void CAngel::StateChange()
{

	//기본 상태 시 무기상태 false 

	for (_uint i = 0; i < 2; i++)
	{
		wstring wstrWeaponColKey = L"Col_Weapon" + to_wstring(i);
		auto iter_find = find_if(m_mapColider.begin(), m_mapColider.end(), CTag_Finder(wstrWeaponColKey.c_str()));
		if (iter_find != m_mapColider.end())
			iter_find->second->SetActive(false);
	}


	//플레이어 상태 전환 시 
	if (m_ePreMachineState != m_eMachineState)
	{
		switch (m_eMachineState)
		{
		case Angel::STATE_SPAWN_IDLE:
			m_eCurAniState = Angel::Angel_Idle;
			break;
		case Angel::STATE_SPAWN:
			m_bTurnEnd = false;
			m_bTurnStartCross = m_bTurnCross;
			m_eCurAniState = Angel::Angel_Atk_Dragon_End;
			m_bActive = true;
			break;
		case Angel::STATE_IDLE:
		{
			m_eCurAniState = Angel::Angel_Idle;
			m_fPatternSpeed = 0.f;

			if (m_ePreMachineState == Angel::STATE_HIT)
				m_fPatternTimer = 1.f;
			else
				m_fPatternTimer = 0.5f;
		}
			break;
		case Angel::STATE_ATK:
			m_iPatternNum = RandNext(0, 5);
			SetAtkPattern();
			break;
		case Angel::STATE_TURN:
			m_fTurnAngle = m_fAngle;
			m_bTurnEnd = false;
			m_fPatternSpeed = 0.f;
			m_fPatternTimer = 0.2f;
			m_bTurnStartCross = m_bTurnCross;
			if (m_fAngle > 180.f)
			{
				if (m_fTurnAngle < 270.f)
				{
					m_eCurAniState = Angel::Angel_Turn_90_L;
				}
				else if (m_fTurnAngle < 360.f)
				{
					m_eCurAniState = Angel::Angel_Turn_180_L;
				}
				break;
			}
			else if (m_fTurnAngle > 30.f)
			{
				if (m_fTurnAngle < 90.f)
				{
					m_eCurAniState = Angel::Angel_Turn_90_R;

				}
				else if (m_fTurnAngle < 180.f)
				{
					m_eCurAniState = Angel::Angel_Turn_180_R;
				}
				break;
			}
			break;
		case Angel::STATE_MOVE:
			break;
		case Angel::STATE_CHASE:

			break;
		case Angel::STATE_HIT:
			m_bHitEnd = false;
			break;
		case Angel::STATE_DEAD:
		{
			m_eCurAniState = Angel::Angel_Death_War;
			CGameObject* pObj;
			_vec3 vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			for (_uint i = 0; i < RandNext(10, 20); i++)
			{
				pObj = CGameMgr::GetInstance()->GetItem(DROPITEM::ITEM_SOUL);
				pObj->SetPos(vPos, ID_DYNAMIC);
				dynamic_cast<CNaviMesh*>(pObj->Get_Component(L"Com_Navi", ID_STATIC))->Set_CellIndex(29);

			}
			if (RandNext(0, 2) == 0)
				pObj = CGameMgr::GetInstance()->GetItem(DROPITEM::ITEM_STONE);
		}
			break;
		case Angel::STATE_END:
			break;
		default:
			break;
		}

		m_ePreMachineState = m_eMachineState;
	}


	if (m_ePreAniState != m_eCurAniState)
	{
		switch (m_eCurAniState)
		{
		case Angel::Angel_Atk_Combo:
			m_fInitAttackMoveSpeed = 20.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bAttackMoveEnd = false;
			m_bNexAni = false;
			m_bBlend = true;
			break;
		case Angel::Angel_Atk_Dash:
			m_fInitAttackMoveSpeed = 30.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bAttackMoveEnd = false;
			m_bNexAni = false;
			m_bBlend = true;
			break;
		case Angel::Angel_Atk_Dragon_Start:
			m_fDragonSpeed = 0.f;
			break;
		case Angel::Angel_Atk_Dragon_Hold:
			m_bActive = false;
			break;
		case Angel::Angel_Atk_Dragon_End:
			m_bActive = true;
			break;
		case Angel::Angel_Atk_Swipe_L:
		case Angel::Angel_Atk_Swipe_R:
			m_fInitAttackMoveSpeed = 15.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bAttackMoveEnd = false;
			m_bNexAni = false;
			m_bBlend = true;
			break;
		case Angel::Angel_Fly_F_Start:
			break;
		case Angel::Angel_Fly_F:
			break;
		case Angel::Angel_Fly_Idle:
			break;
		case Angel::Angel_Death_War:
			break;
		case Angel::Angel_DeathPose_War:
			break;
		case Angel::Angel_Idle:
			break;
		case Angel::Angel_Impact_F:
		case Angel::Angel_Impact_L:
		case Angel::Angel_Impact_R:
			m_fInitAttackMoveSpeed = 8.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			break;
		case Angel::Angel_Impact_Flinch_F:
		case Angel::Angel_Impact_Flinch_L:
		case Angel::Angel_Impact_Flinch_R:
			m_fInitAttackMoveSpeed = 5.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bBlend = false;
			break;
		case Angel::Angel_Knock_B_Start:
			m_fInitAttackMoveSpeed = 5.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bBlend = true;
			m_bAttackMoveEnd = false;
			break;
		case Angel::Angel_Knock_B_Apex:
		case Angel::Angel_Knock_B_Fall:
		case Angel::Angel_Knock_B_Land:
		case Angel::Angel_Knock_B_Idle:
			m_bAttackMoveEnd = false;
			m_bBlend = true;
			break;
		case Angel::Angel_Run_F:
			break;
		case Angel::Angel_Turn_90_L:
		case Angel::Angel_Turn_90_R:
		case Angel::Angel_Turn_180_L:
		case Angel::Angel_Turn_180_R:
			m_bBlend = true;
			break;
		case Angel::Angel_Walk_B:
			break;
		case Angel::End:
			break;
		default:
			break;
		}
		m_ePreAniState = m_eCurAniState;
	}
}
void CAngel::StateActor(_float fDeltaTime)
{

	_vec3	vPos;
	_vec3 vInitPos;
	m_fMoveSpeed = 5.f;
	m_pTransformCom->Get_INFO(INFO_POS, &vInitPos);
	if(!m_bSpawnEnd)
		m_pNavi->Set_CellIndex(m_iNaviIdx);
	vPos = m_pNavi->MoveOn_NaviMesh(&vInitPos, &m_vDir, m_fMoveSpeed, fDeltaTime, m_pCalculatorCom);
	m_fNaviY = vPos.y;
	DirSet(fDeltaTime, m_fAngleSpeed,false);

	switch (m_eMachineState)
	{
	case Angel::STATE_SPAWN_IDLE:
		m_bActive = false;
		break;
	case Angel::STATE_SPAWN:
		if (m_bTurnStartCross != m_bTurnCross)
		{
			m_bSpawnDir = true;
			//if (vInitPos.y <= m_fNaviY)
			//{
			//	m_bSpawnEnd = true;
			//}
		}
		if (m_bNexAni)
		{
			if (!m_bSpawnEnd)
			{
				//m_fSpawnSpeed = m_fInitSpawnSpeed;
				_vec3 vPos;
				m_pTransformCom->Get_INFO(INFO_POS, &vPos);
				if (vPos.y <= m_fNaviY)
				{
					m_bSpawnEnd = true;
				}
			}
		}
		break;
	case Angel::STATE_IDLE:
	{
		if (Pattern_Timer(fDeltaTime))
		{
			auto iter_find = find_if(m_mapColider.begin(), m_mapColider.end(), CTag_Finder(L"Col_Body"));
			if (iter_find != m_mapColider.end())
			{
				if (iter_find->second->GetCol())
				{
					m_eMachineState = Angel::STATE_CHASE;
				}
			}
		}
	}
	break;
	case Angel::STATE_ATK:
		if (m_bAttackMoveEnd)
		{
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed ;
			m_bAttackMoveEnd = false;
		}
		break;
	case Angel::STATE_MOVE:
		break;
	case Angel::STATE_TURN:
		if (m_bTurnStartCross != m_bTurnCross)
		{
			m_eCurAniState = Angel::Angel_Idle;
			m_bTurnEnd = true;
		}

		if (m_bTurnEnd)
		{
			if (m_eCurAniState == Angel::Angel_Idle)
			{
				if (Pattern_Timer(fDeltaTime))
				{
					m_eMachineState = Angel::STATE_IDLE;
				}
				break;
			}
		}
		break;

	case Angel::STATE_CHASE:
	{
		if (m_fAngle > 30.f&&m_fAngle<330.f)
		{
			m_eMachineState = Angel::STATE_TURN;
			break;
		}

		//패턴 셋팅
		_vec3 vPos;
		_vec3 vTargetPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		dynamic_cast<CTransform*> (m_pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_INFO(INFO_POS, &vTargetPos);
		_float fDis = D3DXVec3Length(&(vTargetPos - vPos));

		if (fDis < 7.f)
		{
			m_eMachineState = Angel::STATE_ATK;
			m_bChasingEnd = true;
		}
		else
		{
			m_eCurAniState = Angel::Angel_Run_F;
		}
	}
	break;
	case Angel::STATE_HIT:

		if (m_bHitAniEnd)
		{
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bHitAniEnd = false;
		}

		if (m_bHitStart)
		{
			_vec3 vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			if (vPos.y <= m_fNaviY)
			{
				m_bHitEnd = true;
			}

		}
		//USES_CONVERSION;
		if (m_bHit||m_bHitStart)
			break;
		for (auto iter : m_mapColider)
		{
			//레이어는 뒤에 메쉬 명 붙여서
			//const _tchar* pConvLayerTag = W2BSTR(LayerTag.c_str());

			wstring ColKey = iter.first;
			if (iter.second->GetCol())
			{
				if (m_tCharInfo.fDmg < 2.f)
				{
					if (L"Col_Front" == ColKey)
					{
						m_eCurAniState = Angel::Angel_Impact_Flinch_F;
						iter.second->SetCol(false);
						m_fHitSpeed = 0.f;
						m_tCharInfo.fDmg = 0.f;
						m_fHitTime = 0.1f;
						break;
					}
					else if (L"Col_Back" == ColKey)
					{
						m_eCurAniState = Angel::Angel_Impact_Flinch_F;
						iter.second->SetCol(false);
						m_fHitSpeed = 0.f;
						m_tCharInfo.fDmg = 0.f;
						m_fHitTime = 0.1f;
						break;
					}
					else if (L"Col_Left" == ColKey)
					{
						m_eCurAniState = Angel::Angel_Impact_Flinch_L;
						iter.second->SetCol(false);
						m_fHitSpeed = 0.f;
						m_tCharInfo.fDmg = 0.f;
						m_fHitTime = 0.1f;
						break;
					}
					else if (L"Col_Right" == ColKey)
					{
						m_eCurAniState = Angel::Angel_Impact_Flinch_R;
						iter.second->SetCol(false);
						m_fHitSpeed = 0.f;
						m_tCharInfo.fDmg = 0.f;
						m_fHitTime = 0.1f;
						break;
					}
				}
				else if (m_tCharInfo.fDmg < 3.f)
				{
					if (L"Col_Front" == iter.first)
					{
						m_eCurAniState = Angel::Angel_Impact_F;
						iter.second->SetCol(false);
						m_fHitSpeed = 0.f;
						m_tCharInfo.fDmg = 0.f;
						m_fHitTime = 0.1f;
						break;
					}
					else if (L"Col_Back" == iter.first)
					{
						m_eCurAniState = Angel::Angel_Impact_F;
						iter.second->SetCol(false);
						m_fHitSpeed = 0.f;
						m_tCharInfo.fDmg = 0.f;
						m_fHitTime = 0.1f;
						break;
					}
					else if (L"Col_Left" == iter.first)
					{
						m_eCurAniState = Angel::Angel_Impact_L;
						iter.second->SetCol(false);
						m_fHitSpeed = 0.f;
						m_tCharInfo.fDmg = 0.f;
						m_fHitTime = 0.1f;
						break;
					}
					else if (L"Col_Right" == iter.first)
					{
						m_eCurAniState = Angel::Angel_Impact_R;
						iter.second->SetCol(false);
						m_fHitSpeed = 0.f;
						m_tCharInfo.fDmg = 0.f;
						m_fHitTime = 0.1f;
						break;
					}
				}
				else
				{
					if (L"Col_Front" == ColKey ||
						L"Col_Right" == ColKey ||
						L"Col_Left" == ColKey ||
						L"Col_Back" == ColKey)
					{
						m_eCurAniState = Angel::Angel_Knock_B_Start;
						iter.second->SetCol(false);
						m_fHitSpeed = 0.f;
						m_tCharInfo.fDmg = 0.f;
						m_fHitTime = 1.f;
						m_bHitStart = true;
						break;
					}
				}
			}
		}
		break;
	case Angel::STATE_DEAD:
		break;
	case Angel::STATE_END:
		break;
	default:
		break;
	}


	switch (m_eCurAniState)
	{
	case Angel::Angel_Atk_Combo:
		m_tCharInfo.fAtk = 2.f;

		AtkColActive(0.2, 0.4, 0);
		AtkColActive(0.3, 0.5, 0);
		AtkColActive(0.4, 0.6, 0);

		if (m_pMeshCom->Is_Animationset(0.4))
		{
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 50.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));
			m_pTransformCom->Set_PosY(m_fNaviY);
		}

		//if (!m_pNavi)
		//	break;

		//if (m_pMeshCom->Is_Animationset(0.5))
		//{
		//	if (!m_bNexAni)
		//	{
		//		m_bAttackMoveEnd = true;
		//		m_bNexAni = true;
		//	}
		//	_vec3	vPos;
		//	m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		//	m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 20.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));
		//	m_pTransformCom->Set_PosY(m_fNaviY);
		//}
		//else if (m_pMeshCom->Is_Animationset(0.3))
		//{
		//	_vec3	vPos;
		//	m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		//	m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 15.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));
		//	m_pTransformCom->Set_PosY(m_fNaviY);
		//}

		break;
	case Angel::Angel_Atk_Dash:
		m_tCharInfo.fAtk = 2.f;

		AtkColActive(0.4, 0.6, 0);


		if (!m_pNavi)
			break;
		if (m_pMeshCom->Is_Animationset(0.4))
		{
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 50.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));
			m_pTransformCom->Set_PosY(m_fNaviY);
		}

		break;
	case Angel::Angel_Atk_Dragon_Start:
	{
		if (!m_pNavi)
			break;
		_vec3	vPos;
		m_fMoveSpeed = 1.f;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(&m_pNavi->MoveOn_NaviMesh(&vPos, &m_vDir, m_fMoveSpeed, fDeltaTime, m_pCalculatorCom));
		m_pTransformCom->Set_PosY(m_fNaviY);
	}
		break;
	case Angel::Angel_Atk_Dragon_Hold:
		break;
	case Angel::Angel_Atk_Dragon_End:

		break;
	case Angel::Angel_Atk_Swipe_L:
		m_tCharInfo.fAtk = 2.f;

		AtkColActive(0.4, 0.6, 1);
		if (!m_pNavi)
			break;
		if (m_pMeshCom->Is_Animationset(0.4))
		{
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 50.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));
			m_pTransformCom->Set_PosY(m_fNaviY);
		}

		break;
	case Angel::Angel_Atk_Swipe_R:
		m_tCharInfo.fAtk = 2.f;
		AtkColActive(0.4, 0.6, 0);
		if (!m_pNavi)
			break;
		if (m_pMeshCom->Is_Animationset(0.4))
		{
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 50.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));
			m_pTransformCom->Set_PosY(m_fNaviY);
		}

		break;
	case Angel::Angel_Fly_F_Start:
		break;
	case Angel::Angel_Fly_F:
		break;
	case Angel::Angel_Fly_Idle:
		break;
	case Angel::Angel_Death_War:
		break;
	case Angel::Angel_DeathPose_War:
		break;
	case Angel::Angel_Impact_F:
		break;
	case Angel::Angel_Idle:
		break;
	case Angel::Angel_Impact_L:
	case Angel::Angel_Impact_R:
	case Angel::Angel_Impact_Flinch_F:
	case Angel::Angel_Impact_Flinch_L:
	case Angel::Angel_Impact_Flinch_R:
	{
		_vec3	vPos;

		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 15.f, 0.f, &-m_vTargetDir, fDeltaTime, m_pCalculatorCom));
		m_pTransformCom->Set_PosY(m_fNaviY);
		break;
	}
	case Angel::Angel_Knock_B_Start:
		if (!m_bHitEnd)
		{
			DirSet(fDeltaTime, m_fAngleSpeed, true);
			if (!m_bNexAni)
			{
				m_bHitAniEnd = true;
				m_bNexAni = true;
			}
			m_pTransformCom->MoveStep(MOVETYPE_BREAK, &m_fAttackMoveSpeed, 300.f, 0.f, &_vec3(0.f, 1.f, 0.f), fDeltaTime);
			m_pTransformCom->Move_Pos(&-m_vTargetDir, m_fMoveSpeed*1.5f, fDeltaTime);
		}
		break;
	case Angel::Angel_Knock_B_Idle:
		break;
	case Angel::Angel_Knock_B_Land:
		m_pTransformCom->Set_PosY(m_fNaviY);
		break;
	case Angel::Angel_Knock_B_Fall:
		if (!m_bHitEnd)
		{

			m_pTransformCom->MoveStep(MOVETYPE_ACC, &m_fAttackMoveSpeed, 150.f, 200.f, &_vec3(0.f, -1.f, 0.f), fDeltaTime);
			m_pTransformCom->Move_Pos(&-m_vTargetDir, m_fMoveSpeed*1.5f, fDeltaTime);
		}
		break;
	case Angel::Angel_Knock_B_Apex:
		if (!m_bHitEnd)
		{
			m_pTransformCom->MoveStep(MOVETYPE_BREAK, &m_fAttackMoveSpeed, 300.f, 0.f, &_vec3(0.f, 1.f, 0.f), fDeltaTime);
			m_pTransformCom->Move_Pos(&-m_vTargetDir, m_fMoveSpeed*1.5f, fDeltaTime);
		}
		break;
	case Angel::Angel_Run_F:
	{
		if (!m_pNavi)
			break;
		_vec3	vPos;
		m_fMoveSpeed = 5.f;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(&m_pNavi->MoveOn_NaviMesh(&vPos, &m_vDir, m_fMoveSpeed, fDeltaTime, m_pCalculatorCom));
		m_pTransformCom->Set_PosY(m_fNaviY);
		//m_pTransformCom->Move_Pos(&m_vDir, m_fMoveSpeed);
	}
		break;
	case Angel::Angel_Turn_90_L:
	case Angel::Angel_Turn_90_R:
	case Angel::Angel_Turn_180_L:
	case Angel::Angel_Turn_180_R:
		if (m_pMeshCom->Is_Animationset(0.3))
		{
			DirSet(fDeltaTime, m_fAngleSpeed, true);
		}
		break;
	case Angel::Angel_Walk_B:
	{
		if (!m_pNavi)
			break;
		_vec3	vPos;
		m_fMoveSpeed = 3.f;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(&m_pNavi->MoveOn_NaviMesh(&vPos, &m_vDir, m_fMoveSpeed, fDeltaTime, m_pCalculatorCom));
		m_pTransformCom->Set_PosY(m_fNaviY);
	}
		break;
	case Angel::End:
		break;
	default:
		break;
	}

}
//다음 동작으로 자동으로 연결 
void CAngel::StateLinker(_float fDeltaTime)
{

	switch (m_eCurAniState)
	{
	case Angel::Angel_Atk_Combo:
	case Angel::Angel_Atk_Dash:
	case Angel::Angel_Atk_Swipe_L:
	case Angel::Angel_Atk_Swipe_R:
	case Angel::Angel_Impact_F:
	case Angel::Angel_Impact_L:
	case Angel::Angel_Impact_R:
	case Angel::Angel_Impact_Flinch_F:
	case Angel::Angel_Impact_Flinch_L:
	case Angel::Angel_Impact_Flinch_R:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eMachineState = Angel::STATE_IDLE;
		}
		break;
	case Angel::Angel_Atk_Dragon_Start:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eCurAniState = Angel::Angel_Atk_Dragon_End;

		}
		break;
	case Angel::Angel_Atk_Dragon_End:
		if(m_eMachineState != Angel::STATE_SPAWN_IDLE)
			m_fDragonSpeed += fDeltaTime;
		if (m_fDragonSpeed < m_fDragonTime)
		{
			m_fFrameSpeed = 0.f;
		}
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eMachineState = Angel::STATE_IDLE;
		}
		break;
	case Angel::Angel_Knock_B_Idle:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_bHitStart = false;
			m_eMachineState = Angel::STATE_IDLE;
		}
		break;
	case Angel::Angel_Fly_F_Start:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eCurAniState = Angel::Angel_Fly_Idle;
		}
		break;
	case Angel::Angel_Fly_F:
		break;
	case Angel::Angel_Fly_Idle:
		break;
	case Angel::Angel_Death_War:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eCurAniState = Angel::Angel_DeathPose_War;
		}
		break;
	case Angel::Angel_DeathPose_War:

		break;
	case Angel::Angel_Idle:
		break;
	case Angel::Angel_Knock_B_Start:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eCurAniState = Angel::Angel_Knock_B_Apex;
		}
		break;
	case Angel::Angel_Knock_B_Land:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eCurAniState = Angel::Angel_Knock_B_Idle;
		}
		break;
	case Angel::Angel_Knock_B_Fall:
		if (m_bHitEnd)
		{
			m_eCurAniState = Angel::Angel_Knock_B_Land;
		}
		break;
	case Angel::Angel_Knock_B_Apex:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eCurAniState = Angel::Angel_Knock_B_Fall;
		}
		break;
	case Angel::Angel_Walk_B:
	case Angel::Angel_Run_F:
		break;
	case Angel::Angel_Turn_90_L:
	case Angel::Angel_Turn_90_R:
	case Angel::Angel_Turn_180_L:
	case Angel::Angel_Turn_180_R:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eCurAniState = Angel::Angel_Idle;
			m_bTurnEnd = true;
		}
		break;

	case Angel::End:
		break;
	default:
		break;
	}

}



HRESULT CAngel::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
{
	_matrix		matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	D3DMATERIAL9		tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrl.Power = 10.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&tMtrl.Diffuse);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&tMtrl.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&tMtrl.Ambient);

	pEffect->SetFloat("g_fPower", tMtrl.Power);

	const D3DLIGHT9*		pLightInfo = Get_Light();
	NULL_CHECK_RETURN(pLightInfo, E_FAIL);

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));

	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPos", (_vec4*)&matView._41);

	return S_OK;
}

_bool CAngel::Pattern_Timer(_float fDeltaTime)
{
	m_fPatternSpeed += fDeltaTime;
	if (m_fPatternSpeed > m_fPatternTimer)
	{
		m_fPatternSpeed = 0.f;
		return true;
	}
	return false;
}

void CAngel::SetAtkPattern()
{
	switch (m_iPatternNum)
	{
	case 0:
		m_eCurAniState = Angel::Angel_Atk_Combo;
		break;
	case 1:
		m_eCurAniState = Angel::Angel_Atk_Dash;
		break;
	case 2:
		m_eCurAniState = Angel::Angel_Atk_Dragon_Start;
		break;
	case 3:
		m_eCurAniState = Angel::Angel_Atk_Swipe_L;
		break;
	case 4:
		m_eCurAniState = Angel::Angel_Atk_Swipe_R;
		break;
	case 5:
		m_eCurAniState = Angel::Angel_Atk_Dragon_Start;
		break;
	}	

}


void CAngel::TakeDmg(_float fDmg)
{
	m_tCharInfo.fDmg = fDmg;
	m_tCharInfo.fHp -= fDmg;
	m_eMachineState = Angel::STATE_HIT;
	//m_bHit = true;
}

void CAngel::AtkColActive(double dStart, double dEnd, _uint iWeaponIdx)
{
	auto iter_find = find_if(m_mapColider.begin(), m_mapColider.end(), CTag_Finder((L"Col_Weapon" + to_wstring(iWeaponIdx)).c_str()));
	if (iter_find == m_mapColider.end())
		return;

	if (m_pMeshCom->Is_Animationset(dEnd))
	{
		iter_find->second->SetActive(false);
	}
	else if (m_pMeshCom->Is_Animationset(dStart))
	{
		iter_find->second->SetActive(true);
	}
}

void CAngel::DirSet(_float fDeltaTime, _float fAngleSpeed,_bool bAngleSet)
{
	if (!m_pTarget)
	{
		m_pTarget = CGameMgr::GetInstance()->GetPlayer();
	}
	_vec3 vPos;
	_vec3 vTargetPos;
	m_pTransformCom->Get_INFO(INFO_POS, &vPos);
	dynamic_cast<CTransform*> (m_pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_INFO(INFO_POS,&vTargetPos);

	m_vTargetDir = vTargetPos - vPos;
	D3DXVec3Normalize(&m_vTargetDir, &m_vTargetDir);
	_vec3 vRight;
	m_pTransformCom->Get_INFO(INFO_RIGHT, &vRight);
	_vec3 vCross;
	D3DXVec3Cross(&vCross, &vRight, &m_vTargetDir);
	D3DXVec3Normalize(&vRight, &vRight);
	m_vDir = vRight;
	_float fCos = D3DXVec3Dot(&vRight, &m_vTargetDir);
	m_fAngle = D3DXToDegree(acosf(fCos));


	if (vCross.y < 0.f)
	{
		m_fAngle = 360.f - m_fAngle;
		m_bTurnCross = true;
	}
	else
	{
		m_bTurnCross = false;
	}


	if (bAngleSet)
	{
		if (vCross.y < 0.f)
		{
			m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-fAngleSpeed*fDeltaTime));
		}
		else
		{
			m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(fAngleSpeed*fDeltaTime));
		}
	}
}



void CAngel::DeadCheck()
{
	if (m_tCharInfo.fHp <= 0.f) {
		m_eMachineState = Angel::STATE_DEAD;
		m_bDead = true;
			
	}
}

void CAngel::EnemyTurn(_float fAngle)
{

}

void CAngel::SetOption(void * pArg)
{
	//if (pArg)
	//{
	//	memcpy(&m_eSpawnType, pArg, sizeof(_uint));
	//}

	m_eMachineState = Angel::STATE_SPAWN;
	m_pNavi->Set_CellIndex(m_iNaviIdx);
	//m_bActive = true;
}

