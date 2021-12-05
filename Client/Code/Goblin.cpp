#include "stdafx.h"
#include "Goblin.h"
#include "Enum.h"
#include "Export_Function.h"
#include "GameMgr.h"
#include "Bullet.h"
#include "SoundMgr.h"
#include "Item.h"
CGoblin::CGoblin(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CGoblin::CGoblin(const CGoblin& rhs)
	: CGameObject(rhs)
{

}

CGoblin::~CGoblin(void)
{

}

HRESULT CGoblin::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	//m_pTransformCom->Set_Rot(0.f, D3DXToRadian(90.f), 0.f);	//파싱하면서 바꿀꺼임 
	m_pTransformCom->Update_Component(0.f);
	m_pMeshCom->Set_AnimationIndex(Goblin::Goblin_Idle);
	m_fPatternTimer = 1.f;
	m_fPatternSpeed = 0.f;
	m_eCurAniState = Goblin::Goblin_Idle;
	m_iPatternNum = 0;
	m_fAngleSpeed = 360.f;
	m_eMachineState = Goblin::STATE_IDLE;
	//m_eMachineState = Goblin::STATE_SPAWN_IDLE;
	//m_eSpawnType = Goblin::SPAWN_POTRAL;
	m_fHitSpeed = .0f;
	m_fHitTime = 0.05f;
	m_fAttackMoveSpeed = 5.f;
	SetCharInfo(15.f, 1.f);

	return S_OK;
}

void CGoblin::Late_Ready_Object()
{
}


_int CGoblin::Update_Object(const _float& fTimeDelta)
{

	if (m_bDead)
		return 0;

	_int iExit = CGameObject::Update_Object(fTimeDelta);
	if (!m_pNavi)
	{
		m_pNavi = dynamic_cast<CNaviMesh*>(Clone_Prototype(L"Proto_Navi"));
		m_mapComponent[ID_STATIC].emplace(L"Com_Navi", m_pNavi);
	}


	if (Key_Down(KEY_7))
	{
		m_eMachineState = Goblin::STATE_SPAWN;
	}


	DeadCheck();
	StateChange();
	StateActor(fTimeDelta);
	StateLinker(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);
	return iExit;
}

CGoblin* CGoblin::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGoblin*	pInstance = new CGoblin(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

CGoblin * CGoblin::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoTag)
{
	CGoblin*	pInstance = new CGoblin(pGraphicDev);
	pInstance->SetProtoTag(ProtoTag);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CGoblin::Free(void)
{
	CGameObject::Free();
}


HRESULT CGoblin::Add_Component()
{
	USES_CONVERSION;
	CComponent*		pComponent = nullptr;

	const _tchar* pConvProtoTag = W2BSTR(m_wstrProtoTag.c_str());

	// Mesh
	if(m_wstrProtoTag == L"")
		pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Prototype(L"Goblin"));
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


	pComponent = m_pDissolveCom = dynamic_cast<CTexture*>(Clone_Prototype(L"Proto_Texture_Dissolve"));
	NULL_CHECK_RETURN(m_pDissolveCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Dissolve", pComponent);


	return S_OK;
}

void CGoblin::Render_Object(void)
{
	_float fTimeDelta = Get_TimeDelta(L"Timer_Immediate");

	m_pMeshCom->Set_AnimationIndex(m_eCurAniState, m_bBlend);
	m_pMeshCom->Play_Animation(fTimeDelta);
	for (auto iter : m_mapColider)
	{
		iter.second->Update_Object(fTimeDelta);
	}

	LPD3DXEFFECT	 pEffect = m_pShaderCom->Get_EffectHandle();
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint iMaxPass = 0;

	pEffect->Begin(&iMaxPass, NULL);		// 1인자 : 현재 쉐이더 파일이 반환하는 pass의 최대 개수
											// 2인자 : 시작하는 방식을 묻는 FLAG
	if (m_eCurAniState != Goblin::Goblin_DeathPose)
		pEffect->BeginPass(0);
	else
	{
		m_fDissolveAmount += fTimeDelta*0.4f;
		pEffect->SetFloat("g_DissolveAmount", m_fDissolveAmount);
		m_pDissolveCom->Set_Texture(pEffect, "g_DissolveTexture", 1);

		pEffect->BeginPass(4);
	}


	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();
	pEffect->End();


	Safe_Release(pEffect);
}



void CGoblin::StateChange()
{

	//기본 상태 시 무기상태 false 
	for (_uint i = 0; i < 3; i++)
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
		case Goblin::STATE_SPAWN_IDLE:
			m_eCurAniState = Goblin::Goblin_Idle;
			m_pNavi->Set_CellIndex(m_iNaviIdx);
			m_bSpear = false;
			break;
		case Goblin::STATE_SPAWN:
			m_bTurnEnd = false;
			m_bTurnStartCross = m_bTurnCross;
			if (Goblin::SPAWN_POTRAL == m_eSpawnType)
				m_eCurAniState = Goblin::Goblin_Spawn;
			else if (Goblin::SPAWN_APEX == m_eSpawnType)
				m_eCurAniState = Goblin::Goblin_Spawn_Climb_Hovel;
			else if (Goblin::SPAWN_SIT == m_eSpawnType)
				m_eCurAniState = Goblin::Goblin_Sit_Idle;
			break;
		case Goblin::STATE_IDLE:
		{
			m_eCurAniState = Goblin::Goblin_Idle;
			m_fPatternSpeed = 0.f;

			if (m_ePreMachineState == Goblin::STATE_HIT)
				m_fPatternTimer = 1.f;
			else
				m_fPatternTimer = 0.5f;
		}
		break;
		case Goblin::STATE_ATK:
		{
			_vec3 vPos;
			_vec3 vTargetPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			dynamic_cast<CTransform*> (m_pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_INFO(INFO_POS, &vTargetPos);
			_float fDis = D3DXVec3Length(&(vTargetPos - vPos));

			if (fDis < 3.5f)
			{
				m_iPatternNum = 3;
				m_bBackStep = false;
			}
			else
				m_iPatternNum = RandNext(0, 3);
			//m_iPrePatternNum = m_iPatternNum;
			//m_iPatternNum = 1;
			//m_bSpear = false;
			SetAtkPattern();
		}
			break;
		case Goblin::STATE_TURN:
			m_fTurnAngle = m_fAngle;
			m_bTurnEnd = false;
			m_bSpear = false;
			m_fPatternSpeed = 0.f;
			m_fPatternTimer = 0.5f;
			m_bTurnStartCross = m_bTurnCross;
			if (m_fAngle > 180.f)
			{

				m_eCurAniState = Goblin::Goblin_Turn_L;
				break;
			}
			else if (m_fTurnAngle > 30.f)
			{
				m_eCurAniState = Goblin::Goblin_Turn_R;
				break;
			}
			break;
		case Goblin::STATE_MOVE:
			break;
		case Goblin::STATE_CHASE:

			break;
		case Goblin::STATE_HIT:
			m_bSpear =  false;
			m_bHitEnd = false;
			
			break;
		case Goblin::STATE_HITEND:
			break;
		case Goblin::STATE_DEAD:
			{
				m_eCurAniState = Goblin::Goblin_Death;
				CGameObject* pObj;
				_vec3 vPos;
				_uint iNavIdx = m_pNavi->Get_CellIndex();
				m_pTransformCom->Get_INFO(INFO_POS, &vPos);
				for (_uint i = 0; i < RandNext(5, 20); i++)
				{
					pObj = CGameMgr::GetInstance()->GetItem(DROPITEM::ITEM_SOUL);
					pObj->SetPos(vPos, ID_DYNAMIC);
					dynamic_cast<CItem*>(pObj)->SetNavi(m_pNavi);

				} 
				if (RandNext(0, 2) == 0)
				{
					pObj = CGameMgr::GetInstance()->GetItem(DROPITEM::ITEM_STONE);
					pObj->SetPos(vPos, ID_DYNAMIC);
					dynamic_cast<CItem*>(pObj)->SetNavi(m_pNavi);
				}
			}
			break;
		case Goblin::STATE_END:
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
		case Goblin::Goblin_Attack_01:

			m_fInitAttackMoveSpeed = 25.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bAttackMoveEnd = false;
			m_bNexAni = false;
			m_bBlend = true;
			m_bSound = false;
			break;
		case Goblin::Goblin_Attack_02:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_MONSTER_ATK_3);
			CSoundMgr::Get_Instance()->PlaySound(L"en_fleamag_armored_swipe_2_01.ogg", CSoundMgr::CHANNEL_MONSTER_ATK_3);

			m_fInitAttackMoveSpeed = 100.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bAttackMoveEnd = false;
			m_bNexAni = false;
			m_bBlend = true;
			m_bSound = false;
			break;
		case Goblin::Goblin_Attack_Spear:
		{
			_vec3 vRot = m_pTransformCom->Get_Rot();
			m_fSpearAngle = D3DXToDegree(vRot.y);
			if (m_fSpearAngle < 0.f)
			{
				m_fSpearAngle += 360.f;
			}
			m_bBlend = true;
			m_bSound = false;
		}
			break;
		case Goblin::Goblin_Attack_Dash_Back:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_GOBLIN);
			CSoundMgr::Get_Instance()->PlaySound(L"en_fleamag_jump_02.ogg", CSoundMgr::CHANNEL_GOBLIN);
			m_fInitAttackMoveSpeed = 20.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bAttackMoveEnd = false;
			m_bNexAni = false;
			m_bBlend = true;
			break;
		case Goblin::Goblin_Idle:
			break;
		case Goblin::Goblin_Impact_B:
		case Goblin::Goblin_Impact_F:
		{
			USES_CONVERSION;

			_uint iIdx = RandNext(0, 3);
			wstring wstrSound = L"imp_general_0";
			wstring wstrTag = L".ogg";
			wstrSound += to_wstring(iIdx);
			wstrSound += wstrTag;
			TCHAR* pTag = W2BSTR(wstrSound.c_str());
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_GOBLIN);
			CSoundMgr::Get_Instance()->PlaySound(pTag, CSoundMgr::CHANNEL_GOBLIN);

			m_fInitAttackMoveSpeed = 5.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bBlend = false;
		}
			break;
		case Goblin::Goblin_Jump_Apex:

			m_bBlend = true;
			break;
		case Goblin::Goblin_Jump_Fall:
			break;
		case Goblin::Goblin_Jump_Land:
			break;
		case Goblin::Goblin_Jump_Launch:
			break;
		case Goblin::Goblin_Jump_Launch_Pose:
			break;
		case Goblin::Goblin_Sit_End:
			m_fSpawnSpeed = GetRandomFloat(30.f, 30.f);
			m_bBlend = true;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_GOBLIN);
			CSoundMgr::Get_Instance()->PlaySound(L"en_fleamag_jump_02.ogg", CSoundMgr::CHANNEL_GOBLIN);
			break;
		case Goblin::Goblin_Sit_Idle:
			break;
		case Goblin::Goblin_Spawn:
			m_fSpawnSpeed = GetRandomFloat(30.f, 30.f);
			m_bBlend = true;
			break;
		case Goblin::Goblin_Spawn_Climb_Hovel:
			m_fSpawnSpeed = GetRandomFloat(60.f,60.f);
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_GOBLIN);
			CSoundMgr::Get_Instance()->PlaySound(L"en_fleamag_spawn_vo_01.ogg", CSoundMgr::CHANNEL_GOBLIN);
			m_bBlend = true;
			break;
		case Goblin::Goblin_Turn_L:
		case Goblin::Goblin_Turn_R:
			m_bBlend = true;
			break;
		case Goblin::Goblin_Run_F:
			m_fSoundTime = 0.3f;
			break;
		case Goblin::Goblin_Death:
			break;
		case Goblin::Goblin_DeathPose:
			break;
		case Goblin::Goblin_Knock_b_Apex:
			m_fInitAttackMoveSpeed = 5.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bBlend = true;
			m_bAttackMoveEnd = false;
			break;
		case Goblin::Goblin_Knock_b_Fall:
		case Goblin::Goblin_Knock_b_Idle:
		case Goblin::Goblin_Knock_b_Land:
		case Goblin::Goblin_Knock_b_Recorver:
			m_bAttackMoveEnd = false;
			m_bBlend = true;
			break;
		case Goblin::End:
			break;
		default:
			break;
		}
		m_ePreAniState = m_eCurAniState;
	}

}
void CGoblin::StateActor(_float fDeltaTime)
{

	_vec3	vPos;
	_vec3 vInitPos;
	m_fMoveSpeed = 5.f;
	m_pTransformCom->Get_INFO(INFO_POS, &vInitPos);

	if(!m_bSpawnEnd)
		m_pNavi->Set_CellIndex(m_iNaviIdx);
	vPos = m_pNavi->MoveOn_NaviMesh(&vInitPos, &m_vDir, m_fMoveSpeed, fDeltaTime, m_pCalculatorCom);

	m_fNaviY = vPos.y;
	
	DirSet(fDeltaTime, m_fAngleSpeed, false);

	switch (m_eMachineState)
	{
	case Goblin::STATE_SPAWN_IDLE:

		break;
	case Goblin::STATE_SPAWN:

		if (m_bTurnStartCross != m_bTurnCross)
		{
			m_bSpawnDir = true;
			if (vInitPos.y <= m_fNaviY)
			{
				m_bSpawnEnd = true;
			}
		}
		if (m_bSpawnAniEnd)
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
	case Goblin::STATE_IDLE:
	{
		if (Pattern_Timer(fDeltaTime))
		{
			auto iter_find = find_if(m_mapColider.begin(), m_mapColider.end(), CTag_Finder(L"Col_Body"));
			if (iter_find != m_mapColider.end())
			{
				if (iter_find->second->GetCol())
				{
					m_eMachineState = Goblin::STATE_CHASE;
				}
			}
		}
	}
	break;
	case Goblin::STATE_ATK:
		if (m_bAttackMoveEnd)
		{
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bAttackMoveEnd = false;
		}
		break;
	case Goblin::STATE_MOVE:
		break;
	case Goblin::STATE_TURN:
		if (m_bTurnStartCross != m_bTurnCross)
		{
			m_eCurAniState = Goblin::Goblin_Idle;
			m_bTurnEnd = true;
		}

		if (m_bTurnEnd)
		{
			if (m_eCurAniState == Goblin::Goblin_Idle)
			{
				if (Pattern_Timer(fDeltaTime))
				{
					m_eMachineState = Goblin::STATE_IDLE;
				}
				break;
			}
		}
		break;

	case Goblin::STATE_CHASE:
	{
		if (m_fAngle > 30.f&&m_fAngle<330.f)
		{
			m_eMachineState = Goblin::STATE_TURN;
			break;
		}
		_vec3 vPos;
		_vec3 vTargetPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		dynamic_cast<CTransform*> (m_pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_INFO(INFO_POS, &vTargetPos);
		_float fDis = D3DXVec3Length(&(vTargetPos - vPos));
		//패턴 셋팅
		if (fDis < 7.f)
		{
			m_eMachineState = Goblin::STATE_ATK;
			m_bChasingEnd = true;
		}
		else
		{
			m_eCurAniState = Goblin::Goblin_Run_F;
		}
	}
	break;
	case Goblin::STATE_HIT:

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
		if (m_bHit || m_bHitStart)
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
						m_eCurAniState = Goblin::Goblin_Impact_F;
						iter.second->SetCol(false);
						m_fHitSpeed = 0.f;
						m_tCharInfo.fDmg = 0.f;
						m_fHitTime = 0.1f;
						break;
					}
					else if (L"Col_Back" == ColKey)
					{
						m_eCurAniState = Goblin::Goblin_Impact_B;
					
						m_fHitSpeed = 0.f;
						m_tCharInfo.fDmg = 0.f;
						iter.second->SetCol(false);
						m_fHitTime = 0.1f;
						break;
					}
				}
				else
				{
					if (L"Col_Front" == ColKey ||
						L"Col_Back" == ColKey)
					{
						m_eCurAniState = Goblin::Goblin_Knock_b_Apex;

						m_fHitSpeed = 0.f;
						m_tCharInfo.fDmg = 0.f;
						m_fHitTime = 1.f;
						m_bHitStart = true;
						iter.second->SetCol(false);
						break;
					}
				}
			}
		}
		for (auto iter : m_mapColider)
		{
			if (iter.second->GetCol())
			{
				iter.second->SetCol(false);
			}
		}
		break;
	case Goblin::STATE_DEAD:
		break;
	case Goblin::STATE_END:
		break;
	default:
		break;
	}


	switch (m_eCurAniState)
	{
	case Goblin::Goblin_Attack_01:
	{
		m_tCharInfo.fAtk = 1.f;
		AtkColActive(0.5, 0.7, 0);
		if (!m_pNavi)
			break;
		if (m_pMeshCom->Is_Animationset(0.5))
		{
			if (!m_bSound)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_MONSTER_ATK_3);
				CSoundMgr::Get_Instance()->PlaySound(L"en_fleamag_armored_swipe_01.ogg", CSoundMgr::CHANNEL_MONSTER_ATK_3);
				m_bSound = true;
			}
		}
		if (m_pMeshCom->Is_Animationset(0.3))
		{
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 60.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));
			m_pTransformCom->Set_PosY(m_fNaviY);
		}
		break;
	}
	case Goblin::Goblin_Attack_02:
		m_tCharInfo.fAtk = 2.f;
		AtkColActive(0.4, 0.6, 0);
		if (!m_pNavi)
			break;

		if (m_pMeshCom->Is_Animationset(0.4))
		{
			if (!m_bSound)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_MONSTER_ATK_3);
				CSoundMgr::Get_Instance()->PlaySound(L"en_fleamag_armored_swipe_01.ogg", CSoundMgr::CHANNEL_MONSTER_ATK_3);
				m_bSound = true;
			}

			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 400.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));
			m_pTransformCom->Set_PosY(m_fNaviY);
		}
		break;
	case Goblin::Goblin_Attack_Spear:
		if (m_pMeshCom->Is_Animationset(0.3))
		{
			if (!m_bSpear)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_MONSTER_ATK_3);
				CSoundMgr::Get_Instance()->PlaySound(L"en_fleamag_armored_swipe_03.ogg", CSoundMgr::CHANNEL_MONSTER_ATK_3);
				SpawnGoblinBullet(m_fSpearAngle);
				m_bSpear = true;
			}
		}
		break;
	case Goblin::Goblin_Attack_Dash_Back:
		if (!m_pNavi)
			break;

		if (m_pMeshCom->Is_Animationset(0.2))
		{
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 50.f, 0.f, &-m_vDir, fDeltaTime, m_pCalculatorCom));
			m_pTransformCom->Set_PosY(m_fNaviY);
		}
		break;
	case Goblin::Goblin_Idle:
		break;
	case Goblin::Goblin_Impact_B:
	case Goblin::Goblin_Impact_F:
	{
		_vec3	vPos;

		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 15.f, 0.f, &-m_vTargetDir, fDeltaTime, m_pCalculatorCom));
		m_pTransformCom->Set_PosY(m_fNaviY);
	}
	break;
	case Goblin::Goblin_Jump_Apex:
		break;
	case Goblin::Goblin_Jump_Fall:
		break;
	case Goblin::Goblin_Jump_Land:
		break;
	case Goblin::Goblin_Jump_Launch:
		break;
	case Goblin::Goblin_Jump_Launch_Pose:
		break;
	case Goblin::Goblin_Sit_End:
		//	if (!m_bSpawnAniEnd)
		//	{
		//		if (m_pTransformCom->MoveStep(MOVETYPE_BREAK, &m_fSpawnSpeed, 100.f, 0.f, &_vec3(0.f, 1.f, 0.f), fDeltaTime))
		//		{
		//			m_bSpawnAniEnd = true;
		//		}
		//	}
		//	else
		//	{
		//		if (!m_bHitEnd)
		//		{
		//			m_pTransformCom->MoveStep(MOVETYPE_ACC, &m_fSpawnSpeed, 150.f, 200.f, &_vec3(0.f, -1.f, 0.f), fDeltaTime);
		//			//m_pTransformCom->Move_Pos(&-m_vTargetDir, m_fMoveSpeed*1.5f, fDeltaTime);
		//		}
		//	}
		
		//점프
		break;
	case Goblin::Goblin_Sit_Idle:
		break;
	case Goblin::Goblin_Spawn:
		if (m_pMeshCom->Is_Animationset(0.1))
		{
			if (!m_bSpawnAniEnd)
			{
				if (m_pTransformCom->MoveStep(MOVETYPE_BREAK, &m_fSpawnSpeed, 100.f, 0.f, &_vec3(0.f, 1.f, 0.f), fDeltaTime))
				{
					m_bSpawnAniEnd = true;
				}
			}
			else
			{
				if (!m_bSpawnEnd)
				{
					m_pTransformCom->MoveStep(MOVETYPE_ACC, &m_fSpawnSpeed, 100.f, 200.f, &_vec3(0.f, -1.f, 0.f), fDeltaTime);
					//m_pTransformCom->Move_Pos(&-m_vTargetDir, m_fMoveSpeed*1.5f, fDeltaTime);
				}
			}
		}

		break;
	case Goblin::Goblin_Spawn_Climb_Hovel:
		if (m_pMeshCom->Is_Animationset(0.6))
		{
			if (!m_bSpawnAniEnd)
			{
				if (m_pTransformCom->MoveStep(MOVETYPE_BREAK, &m_fSpawnSpeed, 300.f, 0.f, &_vec3(0.f, 1.f, 0.f), fDeltaTime))
				{
					m_bSpawnAniEnd = true;
				}
			}
			else
			{
				if (!m_bSpawnEnd)
				{
					_vec3 vLook;
					m_pTransformCom->Get_INFO(INFO_RIGHT ,&vLook);
					D3DXVec3Normalize(&vLook, &vLook);
					m_pTransformCom->MoveStep(MOVETYPE_ACC, &m_fSpawnSpeed, 30.f, 200.f, &_vec3(0.f, -1.f, 0.f), fDeltaTime);
					m_pTransformCom->Move_Pos(&vLook, 5.f, fDeltaTime);
				}
			}
		}
		break;
	case Goblin::Goblin_Turn_L:
	case Goblin::Goblin_Turn_R:
		if (m_pMeshCom->Is_Animationset(0.3))
		{
			DirSet(fDeltaTime, m_fAngleSpeed, true);
		}
		break;
	case Goblin::Goblin_Run_F:
	{
		if (!m_pNavi)
			break;
		_vec3	vPos;
		m_fMoveSpeed = 5.f;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(&m_pNavi->MoveOn_NaviMesh(&vPos, &m_vDir, m_fMoveSpeed, fDeltaTime, m_pCalculatorCom));
		m_pTransformCom->Set_PosY(m_fNaviY);

		m_fSoundSpeed += fDeltaTime;
		if (m_fSoundSpeed > m_fSoundTime)
		{
			USES_CONVERSION;

			_uint iIdx = RandNext(0, 3);
			wstring wstrSound = L"en_fleamag_foot_0";
			wstring wstrTag = L".ogg";
			wstrSound += to_wstring(iIdx);
			wstrSound += wstrTag;
			TCHAR* pTag = W2BSTR(wstrSound.c_str());
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_GOBLIN);
			CSoundMgr::Get_Instance()->PlaySound(pTag, CSoundMgr::CHANNEL_GOBLIN);
			m_fSoundSpeed = 0.f;
		}
		//m_pTransformCom->Move_Pos(&m_vDir, m_fMoveSpeed);
		break;
	}
	case Goblin::Goblin_Death:
		break;
	case Goblin::Goblin_DeathPose:
		if (m_fDissolveAmount > 1.f)
		{
			m_bActive = false;
			for (auto& iter : m_mapColider)
			{
				iter.second->SetDead(true);
			}
			m_bDead = true;
		}
		break;
	case Goblin::Goblin_Knock_b_Apex:
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
	case Goblin::Goblin_Knock_b_Fall:
		if (!m_bHitEnd)
		{

			m_pTransformCom->MoveStep(MOVETYPE_ACC, &m_fAttackMoveSpeed, 150.f, 200.f, &_vec3(0.f, -1.f, 0.f), fDeltaTime);
			m_pTransformCom->Move_Pos(&-m_vTargetDir, m_fMoveSpeed*1.5f, fDeltaTime);
		}
	case Goblin::Goblin_Knock_b_Idle:
		break;
	case Goblin::Goblin_Knock_b_Land:
		m_pTransformCom->Set_PosY(m_fNaviY);
		break;
	case Goblin::Goblin_Knock_b_Recorver:
		break;
	case Goblin::End:
		break;
	default:
		break;
	}
}

//다음 동작으로 자동으로 연결 
void CGoblin::StateLinker(_float fDeltaTime)
{

	switch (m_eCurAniState)
	{
	case Goblin::Goblin_Attack_01:
	case Goblin::Goblin_Attack_02:
	case Goblin::Goblin_Attack_Spear:
	case Goblin::Goblin_Attack_Dash_Back:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eMachineState = Goblin::STATE_IDLE;
		}
		break;
	case Goblin::Goblin_Idle:
		break;
	case Goblin::Goblin_Impact_B:
	case Goblin::Goblin_Impact_F:
		m_eMachineState = Goblin::STATE_HITEND;
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eMachineState = Goblin::STATE_IDLE;
		}
		break;
	case Goblin::Goblin_Sit_End:
		//if (m_bSpawnEnd)
		//{
		//	m_eMachineState = Goblin::STATE_IDLE;
		//	m_pTransformCom->Set_PosY(m_fNaviY);
		//}
		if (m_pMeshCom->Is_Animationset(0.7))
		{
			m_pTransformCom->Set_PosY(m_fNaviY);
		}
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_bSpawnEnd = true;
			m_bSpawnAniEnd = true;
			
			m_eMachineState = Goblin::STATE_IDLE;
		}
		break;
	case Goblin::Goblin_Sit_Idle:
	{
		auto iter_find = find_if(m_mapColider.begin(), m_mapColider.end(), CTag_Finder(L"Col_Body"));
		if (iter_find != m_mapColider.end())
		{
			if (iter_find->second->GetCol())
			{
				m_eCurAniState = Goblin::Goblin_Sit_End;
			}
		}
	}
		break;
	case Goblin::Goblin_Spawn:
		if (m_bSpawnEnd)
		{
			m_eMachineState = Goblin::STATE_IDLE;
		}
		break;
	case Goblin::Goblin_Spawn_Climb_Hovel:

		if (m_bSpawnEnd)
		{
			m_pTransformCom->Set_PosY(m_fNaviY);
			m_eMachineState = Goblin::STATE_IDLE;
		}
		
		break;
	case Goblin::Goblin_Turn_L:
	case Goblin::Goblin_Turn_R:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			//m_eCurAniState = Goblin::Goblin_Idle;
			m_bTurnEnd = true;
		}
		break;
	case Goblin::Goblin_Run_F:
		break;
	case Goblin::Goblin_Death:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eCurAniState = Goblin::Goblin_DeathPose;
		}
		break;
	case Goblin::Goblin_DeathPose:
		break;
	case Goblin::Goblin_Knock_b_Apex:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eCurAniState = Goblin::Goblin_Knock_b_Fall;
		}
		break;
	case Goblin::Goblin_Knock_b_Fall:
		if (m_bHitEnd)
		{
			m_eCurAniState = Goblin::Goblin_Knock_b_Land;
		}
		break;
	case Goblin::Goblin_Knock_b_Idle:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eCurAniState = Goblin::Goblin_Knock_b_Recorver;
		}
		break;
	case Goblin::Goblin_Knock_b_Land:
		if (m_pMeshCom->Is_Animationset(0.8))
		{
			m_eCurAniState = Goblin::Goblin_Knock_b_Idle;
		}
		break;
	case Goblin::Goblin_Knock_b_Recorver:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_bHitStart = false;
			m_eMachineState = Goblin::STATE_IDLE;
		}
		break;
	case Goblin::End:
		break;
	default:
		break;
	}


}



HRESULT CGoblin::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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

_bool CGoblin::Pattern_Timer(_float fDeltaTime)
{
	m_fPatternSpeed += fDeltaTime;
	if (m_fPatternSpeed > m_fPatternTimer)
	{
		m_fPatternSpeed = 0.f;
		return true;
	}
	return false;
}

void CGoblin::SetAtkPattern()
{
	switch (m_iPatternNum)
	{
	case 0:
		m_eCurAniState = Goblin::Goblin_Attack_01;
		break;
	case 1:
		m_eCurAniState = Goblin::Goblin_Attack_02;
		break;
	case 2:
		m_eCurAniState = Goblin::Goblin_Attack_Spear;
		break;
	case 3:
		m_eCurAniState = Goblin::Goblin_Attack_Dash_Back;
		break;

	}

}


void CGoblin::TakeDmg(_float fDmg)
{
	m_tCharInfo.fDmg = fDmg;
	m_tCharInfo.fHp -= fDmg;
	m_eMachineState = Goblin::STATE_HIT;
	//m_bHit = true;
}

void CGoblin::AtkColActive(double dStart, double dEnd, _uint iWeaponIdx)
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

void CGoblin::DirSet(_float fDeltaTime, _float fAngleSpeed, _bool bAngleSet)
{
	if (!m_pTarget)
	{
		m_pTarget = CGameMgr::GetInstance()->GetPlayer();
	}
	_vec3 vPos;
	_vec3 vTargetPos;
	m_pTransformCom->Get_INFO(INFO_POS, &vPos);
	dynamic_cast<CTransform*> (m_pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_INFO(INFO_POS, &vTargetPos);

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

void CGoblin::SpawnGoblinBullet(_float fAngle)
{
	//총알 생성
	CGameObject* pBullet = nullptr;
	CTransform* pBulletTrans = nullptr;

	_vec3 vPos;
	m_pTransformCom->Get_INFO(INFO_POS, &vPos);
	vPos.y += 1.f;
	pBullet = CGameMgr::GetInstance()->GetEnemyBullet(BULLET::BULLET_GOBLINSPEAR);
	dynamic_cast<CBullet*>(pBullet)->SetMove(true);
	pBulletTrans = dynamic_cast<CTransform*>(pBullet->Get_Component(L"Com_Transform", ID_DYNAMIC));
	pBulletTrans->Set_Rot(0.f, D3DXToRadian(fAngle+90.f), 0.f);
	pBulletTrans->Set_Pos(&vPos);

}

void CGoblin::DeadCheck()
{
	if (m_tCharInfo.fHp <= 0.f)
		m_eMachineState = Goblin::STATE_DEAD;
}

void CGoblin::EnemyTurn(_float fAngle)
{

}

void CGoblin::SetOption(void * pArg)
{
	if (pArg)
	{
		memcpy(&m_eSpawnType, pArg, sizeof(_uint));
	}

	m_eMachineState = Goblin::STATE_SPAWN;

}


_float CGoblin::GetRandomFloat(_float lowBound, _float highBound)
{
	if (lowBound >= highBound) // bad input
		return lowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}