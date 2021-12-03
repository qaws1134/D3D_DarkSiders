#include "stdafx.h"
#include "Grinner.h"
#include "Enum.h"
#include "Export_Function.h"
#include "GameMgr.h"
#include "Bullet.h"
#include "SoundMgr.h"
#include "Item.h"

CGrinner::CGrinner(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CGrinner::CGrinner(const CGrinner& rhs)
	: CGameObject(rhs)
{

}

CGrinner::~CGrinner(void)
{

}

HRESULT CGrinner::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);

	m_pTransformCom->Update_Component(0.f);
	m_pMeshCom->Set_AnimationIndex(Grinner::Grinner_Idle);
	m_fPatternTimer = 1.f;
	m_fPatternSpeed = 0.f;
	m_eCurAniState = Grinner::Grinner_Idle;
	m_iPatternNum = 0;
	m_fAngleSpeed = 360.f;

	m_eMachineState = Grinner::STATE_SPAWN_IDLE;
	m_eSpawnType = Grinner::SPAWN_APEX;
	m_fHitSpeed = 1.0f;
	m_fHitTime = 0.1f;
	m_fAttackMoveSpeed = 5.f;
	SetCharInfo(15.f, 1.f);
	m_bActive = false;
	return S_OK;
}

void CGrinner::Late_Ready_Object()
{
}


_int CGrinner::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return 0;
	_int iExit = CGameObject::Update_Object(fTimeDelta);
	if (!m_pNavi)
	{
		m_pNavi = dynamic_cast<CNaviMesh*>(Clone_Prototype(L"Proto_Navi"));
		m_mapComponent[ID_STATIC].emplace(L"Com_Navi", m_pNavi);
		//m_fInitY = 
	}
		//m_pNavi =  CGameMgr::GetInstance()->GetNaviMesh();

	if (Key_Down(KEY_6))
	{
		m_eMachineState = Grinner::STATE_SPAWN;
	}


	DeadCheck();
	StateChange();
	StateActor(fTimeDelta);
	StateLinker(fTimeDelta);

	//m_pMeshCom->Play_Animation(fTimeDelta);
	if(m_bActive)
		Add_RenderGroup(RENDER_NONALPHA, this);
	return iExit;
}

CGrinner* CGrinner::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGrinner*	pInstance = new CGrinner(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

CGrinner * CGrinner::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoTag)
{
	CGrinner*	pInstance = new CGrinner(pGraphicDev);
	pInstance->SetProtoTag(ProtoTag);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CGrinner::Free(void)
{
	CGameObject::Free();
}


HRESULT CGrinner::Add_Component()
{
	USES_CONVERSION;
	CComponent*		pComponent = nullptr;

	const _tchar* pConvProtoTag = W2BSTR(m_wstrProtoTag.c_str());

	// Mesh
	if(m_wstrProtoTag == L"")
		pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Prototype(L"Grinner"));
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

void CGrinner::Render_Object(void)
{
	_float fTimeDelta = Get_TimeDelta(L"Timer_Immediate");
	//StateLinker(fTimeDelta);
	m_pMeshCom->Set_AnimationIndex(m_eCurAniState, m_bBlend);
	//m_pMeshCom->Play_Animation(0.f);
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
									
	if(m_eCurAniState != Grinner::Grinner_DeadPose)
		pEffect->BeginPass(0);
	else
	{
		m_fDissolveAmount += fTimeDelta*0.4f;
		pEffect->SetFloat("g_DissolveAmount", m_fDissolveAmount);
		m_pDissolveCom->Set_Texture(pEffect,"g_DissolveTexture",1);

		pEffect->BeginPass(4);
	}

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();
	pEffect->End();


	Safe_Release(pEffect);
}



void CGrinner::StateChange()
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
		case Grinner::STATE_SPAWN_IDLE:
			m_eCurAniState = Grinner::Grinner_Idle;
			for (_uint i = 0; i < 3; i++)
				m_bBarfin[i] = false;
			break;
		case Grinner::STATE_SPAWN:
			m_bTurnEnd = false;
			m_bTurnStartCross = m_bTurnCross;
			if(Grinner::SPAWN_POTRAL==m_eSpawnType)
				m_eCurAniState = Grinner::Grinner_PotalSpawn;
			else if (Grinner::SPAWN_APEX == m_eSpawnType)
				m_eCurAniState = Grinner::Grinner_Jump_Apex;


			break;
		case Grinner::STATE_IDLE:
		{
			m_eCurAniState = Grinner::Grinner_Idle;
			m_fPatternSpeed = 0.f;

			if (m_ePreMachineState == Grinner::STATE_HIT)
				m_fPatternTimer = 1.f;
			else
				m_fPatternTimer = 1.f;
		}
			break;
		case Grinner::STATE_ATK:
			m_iPatternNum = RandNext(0, 4);
			//m_iPatternNum = 1;
			SetAtkPattern();
			break;
		case Grinner::STATE_TURN:
			m_fTurnAngle = m_fAngle;
			m_bTurnEnd = false;
			m_fPatternSpeed = 0.f;
			m_fPatternTimer = 0.5f;
			m_bTurnStartCross = m_bTurnCross;
			if (m_fAngle > 180.f)
			{
				if (m_fTurnAngle < 270.f)
				{
					m_eCurAniState = Grinner::Grinner_Turn_90_L;
				}
				else if (m_fTurnAngle < 360.f)
				{
					m_eCurAniState = Grinner::Grinner_Turn_180_L;
				}
				break;
			}
			else if (m_fTurnAngle > 30.f)
			{
				if (m_fTurnAngle < 90.f)
				{
					m_eCurAniState = Grinner::Grinner_Turn_90_R;

				}
				else if (m_fTurnAngle < 180.f)
				{
					m_eCurAniState = Grinner::Grinner_Turn_180_R;
				}
				break;
			}
			break;
		case Grinner::STATE_MOVE:
			break;
		case Grinner::STATE_CHASE:

			break;
		case Grinner::STATE_HIT:
			for (_uint i = 0; i < 3; i++)
				m_bBarfin[i] = false;
			m_bHitEnd = false;
			//m_bHitStart = false;
			break;
		case Grinner::STATE_DEAD:
		{
			m_eCurAniState = Grinner::Grinner_Death;
			CGameObject* pObj;
			_vec3 vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			for (_uint i = 0; i < RandNext(10, 20); i++)
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
		case Grinner::STATE_END:
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
		case Grinner::Grinner_Atk_Flip:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_MONSTER_ATK_1);
			CSoundMgr::Get_Instance()->PlaySound(L"en_grinner_atk_flip_03.ogg", CSoundMgr::CHANNEL_MONSTER_ATK_1);
			m_fInitAttackMoveSpeed = 10.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bAttackMoveEnd = false;
			m_bNexAni = false;
			m_bBlend = true;
			break;
		case Grinner::Grinner_Atk_BarfinRainbows:
		{

			_vec3 vRot = m_pTransformCom->Get_Rot();
			m_fBarfinAngle = vRot.y;
			m_fBarfinAngle = D3DXToDegree(m_fBarfinAngle);
			if (m_fBarfinAngle < 0.f)
			{
				m_fBarfinAngle += 360.f;
			}
			m_bBlend = true;
		}
			break;
		case Grinner::Grinner_Atk_Lunge:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_MONSTER_ATK_1);
			CSoundMgr::Get_Instance()->PlaySound(L"en_grinner_atk_lunge_02.ogg", CSoundMgr::CHANNEL_MONSTER_ATK_1);
			m_fInitAttackMoveSpeed = 20.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bAttackMoveEnd = false;
			m_bNexAni = false;
			m_bBlend = true;
			break;
		case Grinner::Grinner_Atk_Swipe_Combo:

			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_MONSTER_ATK_1);
			CSoundMgr::Get_Instance()->PlaySound(L"en_grinner_atk_swipecombo_01.ogg", CSoundMgr::CHANNEL_MONSTER_ATK_1);
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_GRINNER_Vo);
			CSoundMgr::Get_Instance()->PlaySound(L"en_grinner_atk_swipecombo_vo_01.ogg", CSoundMgr::CHANNEL_GRINNER_Vo);
			m_fInitAttackMoveSpeed = 10.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bAttackMoveEnd = false;
			m_bNexAni = false;
			m_bBlend = true;
			m_tCharInfo.fAtk = 3.f;
			break;
		case Grinner::Grinner_DeadPose:

			break;
		case Grinner::Grinner_Death:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_GRINNER);
			CSoundMgr::Get_Instance()->PlaySound(L"en_grinner_death_01.ogg", CSoundMgr::CHANNEL_GRINNER);
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_GRINNER_Vo);
			CSoundMgr::Get_Instance()->PlaySound(L"en_grinner_death_vo_01.ogg", CSoundMgr::CHANNEL_GRINNER_Vo);
			break;
		case Grinner::Grinner_Death_War:
			break;
		case Grinner::Grinner_DeathPose_War:
			break;
		case Grinner::Grinner_Idle:
			break;
		case Grinner::Grinner_Impact_Flinch_B:
		case Grinner::Grinner_Impact_Flinch_F:
		case Grinner::Grinner_Impact_Flinch_L:
		case Grinner::Grinner_Impact_Flinch_R:	
			m_fInitAttackMoveSpeed = 5.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bBlend = false;
			break;
		case Grinner::Grinner_Impact_F:
		case Grinner::Grinner_Impact_L:
		case Grinner::Grinner_Impact_R:
		{

			USES_CONVERSION;

			_uint iIdx = RandNext(0, 3);
			wstring wstrSound = L"en_grinner_impact_front_vo_0";
			wstring wstrTag = L".ogg";
			wstrSound += to_wstring(iIdx);
			wstrSound += wstrTag;
			TCHAR* pTag = W2BSTR(wstrSound.c_str());
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_GRINNER);
			CSoundMgr::Get_Instance()->PlaySound(pTag, CSoundMgr::CHANNEL_GRINNER);


			m_fInitAttackMoveSpeed = 8.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
		}
			break;
		case Grinner::Grinner_Jump_Apex:
			m_fSpawnSpeed = 50.f;
			m_bBlend = true;
			break;
		case Grinner::Grinner_Jump_Fall:
			break;
		case Grinner::Grinner_Jump_Land:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_GRINNER);
			CSoundMgr::Get_Instance()->PlaySound(L"en_grinner_jump_land_01.ogg", CSoundMgr::CHANNEL_GRINNER);
			m_bBlend = false;
			break;
		case Grinner::Grinner_Jump_Launch:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_GRINNER);
			CSoundMgr::Get_Instance()->PlaySound(L"en_grinner_jump_launch_01.ogg", CSoundMgr::CHANNEL_GRINNER);
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_GRINNER_Vo);
			CSoundMgr::Get_Instance()->PlaySound(L"en_grinner_jump_launch_vo_01.ogg", CSoundMgr::CHANNEL_GRINNER_Vo);
			break;
		case Grinner::Grinner_Knock_B_Start:
			m_fInitAttackMoveSpeed = 5.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bBlend = true;
			m_bAttackMoveEnd = false;
			break;
		case Grinner::Grinner_Knock_B_Land:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_GRINNER);
			CSoundMgr::Get_Instance()->PlaySound(L"en_grinner_knockback_land_02.ogg", CSoundMgr::CHANNEL_GRINNER);
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_GRINNER_Vo);
			CSoundMgr::Get_Instance()->PlaySound(L"en_grinner_knockback_land_vo_01.ogg", CSoundMgr::CHANNEL_GRINNER_Vo);
			m_bAttackMoveEnd = false;
			m_bBlend = true;
			break;
		case Grinner::Grinner_Knock_B_Recover:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_GRINNER);
			CSoundMgr::Get_Instance()->PlaySound(L"en_grinner_knockback_recover_02.ogg", CSoundMgr::CHANNEL_GRINNER);
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_GRINNER_Vo);
			CSoundMgr::Get_Instance()->PlaySound(L"en_grinner_knockback_recover_vo_02.ogg", CSoundMgr::CHANNEL_GRINNER_Vo);
			m_bAttackMoveEnd = false;
			m_bBlend = true;
			break;
		case Grinner::Grinner_Knock_B_Apex:
		case Grinner::Grinner_Knock_B_Fall:
		case Grinner::Grinner_Knock_B_Idle:
			m_bAttackMoveEnd = false;
			m_bBlend = true;
			break;
		case Grinner::Grinner_PotalSpawn:
			m_bBlend = true;
			break;
		case Grinner::Grinner_Run_F:
			m_fSoundTime = 0.3f;
			break;
		case Grinner::Grinner_Run_FL:
			break;
		case Grinner::Grinner_Run_FR:
			break;
		case Grinner::Grinner_Turn_90_L:
		case Grinner::Grinner_Turn_90_R:
		case Grinner::Grinner_Turn_180_L:
		case Grinner::Grinner_Turn_180_R:
			CSoundMgr::Get_Instance()->PlaySound(L"en_grinner_turn180_Left_02.ogg", CSoundMgr::CHANNEL_GRINNER);
			CSoundMgr::Get_Instance()->PlaySound(L"en_grinner_turn180_left_vo_02.ogg", CSoundMgr::CHANNEL_GRINNER_Vo);
			m_bBlend = true;
			break;
		case Grinner::Grinner_Walk_B:
			break;
		case Grinner::Grinner_Walk_BL:
			break;
		case Grinner::Grinner_Walk_BR:
			break;
		case Grinner::Grinner_Walk_F:
			m_fSoundTime = 0.4f;
			break;
		case Grinner::Grinner_Walk_L:
			break;
		case Grinner::Grinner_Walk_R:
			break;
		default:
			break;
		}
		m_ePreAniState = m_eCurAniState;

	}

}
void CGrinner::StateActor(_float fDeltaTime)
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
	case Grinner::STATE_SPAWN_IDLE:
		
		break;
	case Grinner::STATE_SPAWN:
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
	case Grinner::STATE_IDLE:
	{
		if (Pattern_Timer(fDeltaTime))
		{
			auto iter_find = find_if(m_mapColider.begin(), m_mapColider.end(), CTag_Finder(L"Col_Body"));
			if (iter_find != m_mapColider.end())
			{
				if (iter_find->second->GetCol())
				{
					m_eMachineState = Grinner::STATE_CHASE;
				}
			}
		}
	}
		break;
	case Grinner::STATE_ATK:
		if (m_bAttackMoveEnd)
		{
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed ;
			m_bAttackMoveEnd = false;
			//m_fAddPower = 0.f;
		}
		break;
	case Grinner::STATE_MOVE:
		break;
	case Grinner::STATE_TURN:
		if (m_bTurnStartCross != m_bTurnCross)
		{
			m_eCurAniState = Grinner::Grinner_Idle;
			m_bTurnEnd = true;
		}

		if (m_bTurnEnd)
		{
			if (m_eCurAniState == Grinner::Grinner_Idle)
			{
				if (Pattern_Timer(fDeltaTime))
				{
					m_eMachineState = Grinner::STATE_IDLE;
				}
				break;
			}
		}
		break;

	case Grinner::STATE_CHASE:
	{
		if (m_fAngle > 30.f&&m_fAngle<330.f)
		{
			m_eMachineState = Grinner::STATE_TURN;
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
			m_eMachineState = Grinner::STATE_ATK;
			m_bChasingEnd = true;
		}
		else if (fDis < 10.f)
		{
			m_eCurAniState = Grinner::Grinner_Walk_F;
		}
		else
		{
			m_eCurAniState = Grinner::Grinner_Run_F;
		}
	}
		break;
	case Grinner::STATE_HIT:

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
						m_eCurAniState = Grinner::Grinner_Impact_Flinch_F;
						iter.second->SetCol(false);
						m_fHitSpeed = 0.f;
						m_tCharInfo.fDmg = 0.f;
						m_fHitTime = 0.1f;
						break;
					}
					else if (L"Col_Back" == ColKey)
					{
						m_eCurAniState = Grinner::Grinner_Impact_Flinch_B;
						iter.second->SetCol(false);
						m_fHitSpeed = 0.f;
						m_tCharInfo.fDmg = 0.f;
						m_fHitTime = 0.1f;
						break;
					}
					else if (L"Col_Left" == ColKey)
					{
						m_eCurAniState = Grinner::Grinner_Impact_Flinch_L;
						iter.second->SetCol(false);
						m_fHitSpeed = 0.f;
						m_tCharInfo.fDmg = 0.f;
						m_fHitTime = 0.1f;
						break;
					}
					else if (L"Col_Right" == ColKey)
					{
						m_eCurAniState = Grinner::Grinner_Impact_Flinch_R;
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
						m_eCurAniState = Grinner::Grinner_Impact_F;
						iter.second->SetCol(false);
						m_fHitSpeed = 0.f;
						m_tCharInfo.fDmg = 0.f;
						m_fHitTime = 0.1f;
						break;
					}
					else if (L"Col_Back" == iter.first)
					{
						m_eCurAniState = Grinner::Grinner_Impact_Flinch_B;
						iter.second->SetCol(false);
						m_fHitSpeed = 0.f;
						m_tCharInfo.fDmg = 0.f;
						m_fHitTime = 0.1f;
						break;
					}
					else if (L"Col_Left" == iter.first)
					{
						m_eCurAniState = Grinner::Grinner_Impact_L;
						iter.second->SetCol(false);
						m_fHitSpeed = 0.f;
						m_tCharInfo.fDmg = 0.f;
						m_fHitTime = 0.1f;
						break;
					}
					else if (L"Col_Right" == iter.first)
					{
						m_eCurAniState = Grinner::Grinner_Impact_R;
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
						m_eCurAniState = Grinner::Grinner_Knock_B_Start;
						iter.second->SetCol(false);
						m_fHitSpeed = 0.f;
						m_tCharInfo.fDmg = 0.f;
						m_fHitTime = 1.f;
						m_bHitStart = true;
						break;
					}
				}

				//iter.second->SetCol(false);
				//m_fHitSpeed = 0.f;
				//m_tCharInfo.fDmg = 0.f;
			}
		}
		break;
	case Grinner::STATE_DEAD:
		
		break;
	case Grinner::STATE_END:
		break;
	default:
		break;
	}

	switch (m_eCurAniState)
	{
	case Grinner::Grinner_Atk_Flip:
	{
		m_tCharInfo.fAtk = 2.f;
		AtkColActive(0.2, 0.5, 0);
		m_tCharInfo.fAtk = 1.f;
		AtkColActive(0.4, 0.7, 1);
		AtkColActive(0.4, 0.7, 2);

		if (!m_pNavi)
			break;

		if (m_pMeshCom->Is_Animationset(0.3))
		{
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 15.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));
			m_pTransformCom->Set_PosY(m_fNaviY);
		}
		
	}
		break;
	case Grinner::Grinner_Atk_BarfinRainbows:
	{
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			for (_uint i = 0; i < 3; i++)
				m_bBarfin[i] = false;
		}
		else if (m_pMeshCom->Is_Animationset(0.7))
		{
			if (!m_bBarfin[2])
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_MONSTER_ATK_1);
				CSoundMgr::Get_Instance()->PlaySound(L"en_fleamag_armored_swipe_03.ogg", CSoundMgr::CHANNEL_MONSTER_ATK_1);

				SpawnBarfinBullet(m_fBarfinAngle+ 70.f);
				m_bBarfin[2] = true;
			}
		}
		else if (m_pMeshCom->Is_Animationset(0.5))
		{
			if (!m_bBarfin[1])
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_MONSTER_ATK_1);
				CSoundMgr::Get_Instance()->PlaySound(L"en_fleamag_armored_swipe_03.ogg", CSoundMgr::CHANNEL_MONSTER_ATK_1);

				SpawnBarfinBullet(m_fBarfinAngle+ 90.f);
				m_bBarfin[1] = true;
			}
		}
		else if (m_pMeshCom->Is_Animationset(0.3))
		{
			if (!m_bBarfin[0])
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_MONSTER_ATK_1);
				CSoundMgr::Get_Instance()->PlaySound(L"en_fleamag_armored_swipe_03.ogg", CSoundMgr::CHANNEL_MONSTER_ATK_1);

				SpawnBarfinBullet(m_fBarfinAngle+110.f);
				m_bBarfin[0] = true;
			}
		}



	}
		break;
	case Grinner::Grinner_Atk_Lunge:
	{
		m_tCharInfo.fAtk = 2.f;

		AtkColActive(0.4, 0.6, 1);
		AtkColActive(0.4, 0.6, 2);

		if (!m_pNavi)
			break;
		//_vec3	vPos;
		//m_fMoveSpeed = 5.f;
		//m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		//m_pTransformCom->Set_Pos(&m_pNavi->MoveOn_NaviMesh(&vPos, &m_vDir, m_fMoveSpeed, fDeltaTime, m_pCalculatorCom));
		if (m_pMeshCom->Is_Animationset(0.4))
		{
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 50.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));
			m_pTransformCom->Set_PosY(m_fNaviY);
		}

	}
		break;
	case Grinner::Grinner_Atk_Swipe_Combo:
	{
		m_tCharInfo.fAtk = 2.f;

		AtkColActive(0.2, 0.4, 1);
		AtkColActive(0.3, 0.5, 0);
		AtkColActive(0.4, 0.6, 2);

		if (!m_pNavi)
			break;
	
		if (m_pMeshCom->Is_Animationset(0.5))
		{
			if (!m_bNexAni)
			{
				m_bAttackMoveEnd = true;
				m_bNexAni = true;
			}
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 20.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));
			m_pTransformCom->Set_PosY(m_fNaviY);
		}
		else if(m_pMeshCom->Is_Animationset(0.3))
		{
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 15.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));
			m_pTransformCom->Set_PosY(m_fNaviY);
		}

	}
		break;
	case Grinner::Grinner_DeadPose:
	{
		if (m_fDissolveAmount > 1.f)
		{
			m_bActive = false;
			for (auto& iter : m_mapColider)
			{
				iter.second->SetDead(true);
			}
			m_bDead = true;
		}
	}
		break;
	case Grinner::Grinner_Death:
		break;
	case Grinner::Grinner_Death_War:
		break;
	case Grinner::Grinner_DeathPose_War:
		break;
	case Grinner::Grinner_Idle:
		break;
	case Grinner::Grinner_Impact_F:
	case Grinner::Grinner_Impact_Flinch_B:
	case Grinner::Grinner_Impact_Flinch_F:
	case Grinner::Grinner_Impact_Flinch_L:
	case Grinner::Grinner_Impact_Flinch_R:
	case Grinner::Grinner_Impact_L:
	case Grinner::Grinner_Impact_R:
	{
		_vec3	vPos;

		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 15.f, 0.f, &-m_vTargetDir, fDeltaTime, m_pCalculatorCom));
		m_pTransformCom->Set_PosY(m_fNaviY);
	}
		break;
	case Grinner::Grinner_Jump_Apex:
		if (!m_bHitEnd)
		{
			if (!m_bSpawnDir)
				DirSet(fDeltaTime, m_fAngleSpeed, true);

			m_pTransformCom->MoveStep(MOVETYPE_BREAK, &m_fSpawnSpeed, 100.f, 0.f, &_vec3(0.f, 1.f, 0.f), fDeltaTime);
			m_pTransformCom->Move_Pos(&m_vTargetDir, m_fMoveSpeed*0.5f, fDeltaTime);
		}

		break;
	case Grinner::Grinner_Jump_Fall:
		if (!m_bHitEnd)
		{
			if(!m_bSpawnDir)
				DirSet(fDeltaTime, m_fAngleSpeed, true);
			if (!m_bNexAni)
			{
				m_bHitAniEnd = true;
				m_bNexAni = true;
			}
			m_pTransformCom->MoveStep(MOVETYPE_ACC, &m_fSpawnSpeed, 150.f, 200.f, &_vec3(0.f, -1.f, 0.f), fDeltaTime);
			m_pTransformCom->Move_Pos(&m_vTargetDir, m_fMoveSpeed*0.5f, fDeltaTime);
		}
		break;
	case Grinner::Grinner_Jump_Land:
		m_pTransformCom->Set_PosY(m_fNaviY);
		break;
	case Grinner::Grinner_Jump_Launch:
		break;
	case Grinner::Grinner_Knock_B_Start:
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
	case Grinner::Grinner_Knock_B_Apex:
		if (!m_bHitEnd)
		{
			m_pTransformCom->MoveStep(MOVETYPE_BREAK, &m_fAttackMoveSpeed, 300.f, 0.f, &_vec3(0.f, 1.f, 0.f), fDeltaTime);
			m_pTransformCom->Move_Pos(&-m_vTargetDir, m_fMoveSpeed*1.5f, fDeltaTime);
		}
		break;
	case Grinner::Grinner_Knock_B_Fall:
		if (!m_bHitEnd)
		{

			m_pTransformCom->MoveStep(MOVETYPE_ACC, &m_fAttackMoveSpeed, 150.f, 200.f, &_vec3(0.f, -1.f, 0.f), fDeltaTime);
			m_pTransformCom->Move_Pos(&-m_vTargetDir, m_fMoveSpeed*1.5f, fDeltaTime);
		}
		break;
	case Grinner::Grinner_Knock_B_Land:
		m_pTransformCom->Set_PosY(m_fNaviY);
		break;
	case Grinner::Grinner_Knock_B_Recover:
		break;
		break;
	case Grinner::Grinner_Knock_B_Idle:
		break;
	case Grinner::Grinner_PotalSpawn:
		if (m_pMeshCom->Is_Animationset(0.6))
		{
			m_pTransformCom->MoveStep(MOVETYPE_BREAK, &m_fSpawnSpeed, 300.f, 0.f, &_vec3(0.f, 1.f, 0.f), fDeltaTime);
		}
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_pTransformCom->Set_PosY(m_fNaviY);
		}
		break;
	case Grinner::Grinner_Run_F:
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
			wstring wstrSound = L"en_grinner_footstep_run_0";
			wstring wstrTag = L".ogg";
			wstrSound += to_wstring(iIdx);
			wstrSound += wstrTag;
			TCHAR* pTag = W2BSTR(wstrSound.c_str());
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_GRINNER);
			CSoundMgr::Get_Instance()->PlaySound(pTag, CSoundMgr::CHANNEL_GRINNER);
			m_fSoundSpeed = 0.f;
		}


		//m_pTransformCom->Move_Pos(&m_vDir, m_fMoveSpeed);
		break;
	}
	case Grinner::Grinner_Run_FL:
		break;
	case Grinner::Grinner_Run_FR:
		break;
	case Grinner::Grinner_Turn_90_L:
	case Grinner::Grinner_Turn_90_R:
	case Grinner::Grinner_Turn_180_L:
	case Grinner::Grinner_Turn_180_R:
		if (m_pMeshCom->Is_Animationset(0.3))
		{
			DirSet(fDeltaTime, m_fAngleSpeed, true);
		}
		break;
	case Grinner::Grinner_Walk_B:
		break;
	case Grinner::Grinner_Walk_BL:
		break;
	case Grinner::Grinner_Walk_BR:
		break;
	case Grinner::Grinner_Walk_F:
	{
		if (!m_pNavi)
			break;
		_vec3	vPos;
		m_fMoveSpeed = 3.f;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(&m_pNavi->MoveOn_NaviMesh(&vPos, &m_vDir, m_fMoveSpeed, fDeltaTime, m_pCalculatorCom));
		m_pTransformCom->Set_PosY(m_fNaviY);

		m_fSoundSpeed += fDeltaTime;
		if (m_fSoundSpeed > m_fSoundTime)
		{
			USES_CONVERSION;

			_uint iIdx = RandNext(0, 3);
			wstring wstrSound = L"en_grinner_footstep_front_0";
			wstring wstrTag = L".ogg";
			wstrSound += to_wstring(iIdx);
			wstrSound += wstrTag;
			TCHAR* pTag = W2BSTR(wstrSound.c_str());
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_GRINNER);
			CSoundMgr::Get_Instance()->PlaySound(pTag, CSoundMgr::CHANNEL_GRINNER);
			m_fSoundSpeed = 0.f;
		}
	}
		break;
	case Grinner::Grinner_Walk_L:
		break;
	case Grinner::Grinner_Walk_R:
		break;
	case Grinner::End:
		break;
	default:
		break;
	}
}
//다음 동작으로 자동으로 연결 
void CGrinner::StateLinker(_float fDeltaTime)
{

	switch (m_eCurAniState)
	{
	case Grinner::Grinner_Atk_Flip:
	case Grinner::Grinner_Atk_BarfinRainbows:
	case Grinner::Grinner_Atk_Lunge:
	case Grinner::Grinner_Atk_Swipe_Combo:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eMachineState = Grinner::STATE_IDLE;
		}
		break;
	case Grinner::Grinner_DeadPose:
	{
		//삭제이펙트
	}
		break;
	case Grinner::Grinner_Death:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eCurAniState = Grinner::Grinner_DeadPose;
		}
		break;
	case Grinner::Grinner_Death_War:
		break;
	case Grinner::Grinner_DeathPose_War:
		break;
	case Grinner::Grinner_Idle:

		break;
	case Grinner::Grinner_Impact_F:
	case Grinner::Grinner_Impact_Flinch_B:
	case Grinner::Grinner_Impact_Flinch_F:
	case Grinner::Grinner_Impact_Flinch_L:
	case Grinner::Grinner_Impact_Flinch_R:
	case Grinner::Grinner_Impact_L:
	case Grinner::Grinner_Impact_R:
	case Grinner::Grinner_PotalSpawn:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eMachineState = Grinner::STATE_IDLE;
		}
		break;
	case Grinner::Grinner_Jump_Land:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eMachineState = Grinner::STATE_IDLE;
		}
		break;

	case Grinner::Grinner_Jump_Apex:
		if (m_pMeshCom->Is_Animationset(0.65))
		{
			m_eCurAniState = Grinner::Grinner_Jump_Fall;
		}
		break;
	case Grinner::Grinner_Jump_Fall:
		if (m_bSpawnEnd)
		{
			m_eCurAniState = Grinner::Grinner_Jump_Land;
		}
		break;
	case Grinner::Grinner_Jump_Launch:
		break;	
	case Grinner::Grinner_Knock_B_Start:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eCurAniState = Grinner::Grinner_Knock_B_Apex;
		}
		break;
	case Grinner::Grinner_Knock_B_Recover:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_bHitStart = false;
			m_eMachineState = Grinner::STATE_IDLE;
		}
		break;
	case Grinner::Grinner_Knock_B_Apex:
		if (m_pMeshCom->Is_Animationset(0.4))
		{
			m_eCurAniState = Grinner::Grinner_Knock_B_Fall;
		}
		break;
	case Grinner::Grinner_Knock_B_Fall:
		if (m_bHitEnd)
		{
			m_eCurAniState = Grinner::Grinner_Knock_B_Land;
		}
		break;
	case Grinner::Grinner_Knock_B_Land:
		if (m_pMeshCom->Is_Animationset(0.8))
		{
			m_eCurAniState = Grinner::Grinner_Knock_B_Idle;
		}	
		break;
	case Grinner::Grinner_Knock_B_Idle:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eCurAniState = Grinner::Grinner_Knock_B_Recover;
		}
		break;

	case Grinner::Grinner_Turn_90_L:
	case Grinner::Grinner_Turn_90_R:
	case Grinner::Grinner_Turn_180_L:
	case Grinner::Grinner_Turn_180_R:
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			//m_eCurAniState = Grinner::Grinner_Idle;
			m_bTurnEnd = true;
		}
		break;
	case Grinner::Grinner_Run_F:
		break;
	case Grinner::Grinner_Run_FL:
		break;
	case Grinner::Grinner_Run_FR:
		break;
	case Grinner::Grinner_Walk_B:
		break;
	case Grinner::Grinner_Walk_BL:
		break;
	case Grinner::Grinner_Walk_BR:
		break;
	case Grinner::Grinner_Walk_F:
		break;
	case Grinner::Grinner_Walk_L:
		break;
	case Grinner::Grinner_Walk_R:
		break;
	default:
		break;
	}

}



HRESULT CGrinner::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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

_bool CGrinner::Pattern_Timer(_float fDeltaTime)
{
	m_fPatternSpeed += fDeltaTime;
	if (m_fPatternSpeed > m_fPatternTimer)
	{
		m_fPatternSpeed = 0.f;
		return true;
	}
	return false;
}

void CGrinner::SetAtkPattern()
{
	switch (m_iPatternNum)
	{
	case 0:
		m_eCurAniState = Grinner::Grinner_Atk_Flip;
		break;
	case 1:
		m_eCurAniState = Grinner::Grinner_Atk_BarfinRainbows;
		break;
	case 2:
		m_eCurAniState = Grinner::Grinner_Atk_Lunge;
		break;
	case 3:
		m_eCurAniState = Grinner::Grinner_Atk_Swipe_Combo;
		break;
	case 4:
		//뒷무빙 
		//m_eMachineState = Grinner::STATE_MOVE;
		break;
	}	

}


void CGrinner::TakeDmg(_float fDmg)
{
	m_tCharInfo.fDmg = fDmg;
	m_tCharInfo.fHp -= fDmg;
	m_eMachineState = Grinner::STATE_HIT;
	//m_bHit = true;
}

void CGrinner::AtkColActive(double dStart, double dEnd, _uint iWeaponIdx)
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

void CGrinner::DirSet(_float fDeltaTime, _float fAngleSpeed,_bool bAngleSet)
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

void CGrinner::SpawnBarfinBullet(_float fAngle)
{
	//총알 생성
	CGameObject* pBullet = nullptr;
	CTransform* pBulletTrans = nullptr;

	_vec3 vPos;
	m_pTransformCom->Get_INFO(INFO_POS, &vPos);
	vPos.y += 1.f;
	pBullet = CGameMgr::GetInstance()->GetEnemyBullet(BULLET::BULLET_GRINNERBARFIN);
	dynamic_cast<CBullet*>(pBullet)->SetMove(true);
	pBulletTrans = dynamic_cast<CTransform*>(pBullet->Get_Component(L"Com_Transform", ID_DYNAMIC));
	pBulletTrans->Set_Rot(0.f, D3DXToRadian(fAngle), 0.f);
	pBulletTrans->Set_Pos(&vPos);
	
}

void CGrinner::DeadCheck()
{
	if (m_tCharInfo.fHp <= 0.f)
		m_eMachineState = Grinner::STATE_DEAD;
}

void CGrinner::EnemyTurn(_float fAngle)
{

}

void CGrinner::SetOption(void * pArg)
{
	if (pArg)
	{
		memcpy(&m_eSpawnType, pArg, sizeof(_uint));
	}

	m_eMachineState = Grinner::STATE_SPAWN;
	m_pNavi->Set_CellIndex(m_iNaviIdx);
	m_bActive = true;
}

