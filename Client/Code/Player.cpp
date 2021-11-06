#include "stdafx.h"
#include "Player.h"
#include "Enum.h"
#include "Export_Function.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CPlayer::CPlayer(const CPlayer& rhs)
	: CGameObject(rhs)
{

}

CPlayer::~CPlayer(void)
{

}

HRESULT CPlayer::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	//m_pTranssformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	m_pTransformCom->Update_Component(0.f);
	m_pMeshCom->Set_AnimationIndex(0);
	dAttackCheckFrame = 0.4;
	m_pTransformCom->Get_INFO(INFO_POS,&m_vPreAniPos);
	return S_OK;
}

void CPlayer::Late_Ready_Object()
{
}
//카메라가 본 위치를 따라가고
//

_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);
	_int iExit = CGameObject::Update_Object(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);
	StateChange();
	StateLinker();
	//SetNextAniPos();
	m_pMeshCom->Play_Animation(fTimeDelta);
	return iExit;
}

void CPlayer::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pMeshCom->Render_Meshes();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CPlayer::StateChange()
{
	//플레이어 상태 전환 시 
	if (m_ePreMachineState != m_eMachineState)
	{
		switch (m_eMachineState)
		{
		case War::STATE_IDLE:
			if (m_ePreMachineState == War::STATE_IDLE_CB)
			{
				m_eCurAniState = War::War_Idle_Combat_to_Idle;
			}
			else
			{
				m_eCurAniState = War::War_Idle;
			}
			break;
		case War::STATE_IDLE_CB:
			if (m_ePreMachineState == War::STATE_IDLE)
			{
				m_eCurAniState = War::War_Idle_to_Idle_Combat;
			}
			else
			{
				m_eCurAniState = War::War_Idle_Combat;
			}
			break;
		case War::MOVE:
			
			break;
		case War::MOVE_CB:
			break;
		case War::ATTACK:
			if (m_eKeyState == War::LBUTTON)
			{
				m_eCurAniState = War::War_Atk_Light_01;
			}
			if (m_eKeyState == War::RBUTTON)
			{
				m_eCurAniState = War::War_Atk_Heavy_01;
			}
			break;
		case War::JUMP:
			break;
		case War::JUMP_CB:
			break;
		case War::JUMPATTACK:
			break;
		case War::DASH:
			break;
		case War::HIT:
			break;
		case War::STATE_END:
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
		case War::War_Idle:
			

			break;
		case War::War_Idle_Combat:
			

			break;
		case War::War_Idle_Combat_to_Idle:
			
			break;
		case War::War_Idle_to_Idle_Combat:
		

			break;
		case War::War_Run:

			break;
		case War::War_Run_F_Combat:
			
			break;
		case War::War_Jump:
			m_eKeyState = War::KEYSTATE_END;

			break;
		case War::War_Jump_Combat:
			break;
		case War::War_Jump_Combat_Land:
			break;
		case War::War_Jump_Combat_Land_Run:
			break;
		case War::War_Jump_Double:
			break;
		case War::War_Jump_Fall:
			break;
		case War::War_Jump_Fall_Combat:
			break;
		case War::War_Jump_Land:
			break;
		case War::War_Jump_Land_Heavy:
			break;
		case War::War_Jump_Land_Run:
			break;
		case War::War_Dash:
			m_eKeyState = War::KEYSTATE_END;

			break;
		case War::War_Dash_Air_Land:
			break;
		case War::War_Dash_Air_Loop:
			break;
		case War::War_Dash_Air_Start:
			break;
		case War::War_DashTo_Back:
			break;
		case War::War_Atk_Light_01:
			m_eKeyState = War::KEYSTATE_END;
			break;
		case War::War_Atk_Light_02:
			m_eKeyState = War::KEYSTATE_END;
			break;
		case War::War_Atk_Light_03:
			m_eKeyState = War::KEYSTATE_END;
			break;
		case War::War_Atk_Light_04:
			m_eKeyState = War::KEYSTATE_END;
			break;
		case War::War_Atk_Air_Light_01:
			m_eKeyState = War::KEYSTATE_END;
			break;
		case War::War_Atk_Air_Light_02:
			m_eKeyState = War::KEYSTATE_END;
			break;
		case War::War_Atk_Air_Filpsaw_Start:
			break;
		case War::War_Atk_Air_Filpsaw_Loop:
			break;
		case War::War_Atk_Air_Filpsaw_Land:
			break;
		case War::War_Atk_Dash:
			break;
		case War::War_Atk_Earth_Start:
			break;
		case War::War_Atk_Earth_Loop:
			break;
		case War::War_Atk_Earth_End:
			break;
		case War::War_Atk_Flamebrand:
			break;
		case War::War_Atk_Flamebrand_Start:
			break;
		case War::War_Atk_Flamebrand_End:
			break;
		case War::War_Chest_Open:
			break;
		case War::War_Death:
			break;
		case War::War_Block_Idle:
			break;
		case War::War_Block_Impact_Heavy:
			break;
		case War::War_Block_Impact_Light:
			break;
		case War::War_Block_Impact_Medium:
			break;
		case War::War_Block_Start:
			break;
		case War::War_Knockback_Fall:
			break;
		case War::War_Knockback_Land:
			break;
		case War::War_Knockback_Loop:
			break;
		case War::War_Knockback_Start:
			break;
		case War::War_Impack_From_Back:
			break;
		case War::War_Impack_From_Front:
			break;
		case War::War_Impack_From_Left:
			break;
		case War::War_Impack_From_Right:
			break;
		case War::War_Skill_01:
			break;
		case War::War_Skill_02_Start:
			break;
		case War::War_Skill_02_Strike:
			break;
		case War::War_Skill_02_Run:
			break;
		case War::War_Skill_03:
			break;
		case War::War_Glide:
			break;
		case War::War_Glide_Start:
			break;
		case War::War_Atk_Heavy_01:
			m_eKeyState = War::KEYSTATE_END;
			break;
		case War::War_Atk_Heavy_02:
			m_eKeyState = War::KEYSTATE_END;
			break;
		case War::War_Atk_Heavy_03:
			m_eKeyState = War::KEYSTATE_END;
			break;
		case War::War_End:
			break;
		}
		m_ePreAniState = m_eCurAniState;
		m_pMeshCom->Set_AnimationIndex(m_eCurAniState);
	}

}
//null_Spine위치값 뺴오기 

//다음 동작으로 자동으로 연결 
void CPlayer::StateLinker()
{
	switch (m_eCurAniState)
	{
	case War::War_Idle:
		break;
	case War::War_Idle_Combat:
		break;
	case War::War_Idle_Combat_to_Idle:
		//칼집어넣기
		if(m_pMeshCom->Is_AnimationsetFinish())
			m_eCurAniState = War::War_Idle;
		break;
	case War::War_Idle_to_Idle_Combat:
		//칼뽑기
		if(m_pMeshCom->Is_AnimationsetFinish())
			m_eCurAniState = War::War_Idle_Combat;
		break;
	case War::War_Run:
		//칼넣고 달리기
		break;
	case War::War_Run_F_Combat:
		//칼뽑고 달리기

		break;
	case War::War_Jump:
		break;
	case War::War_Jump_Combat:
		break;
	case War::War_Jump_Combat_Land:
		break;
	case War::War_Jump_Combat_Land_Run:
		break;
	case War::War_Jump_Double:
		break;
	case War::War_Jump_Fall:
		break;
	case War::War_Jump_Fall_Combat:
		break;
	case War::War_Jump_Land:
		break;
	case War::War_Jump_Land_Heavy:
		break;
	case War::War_Jump_Land_Run:
		break;
	case War::War_Dash:
		break;
	case War::War_Dash_Air_Land:
		break;
	case War::War_Dash_Air_Loop:
		break;
	case War::War_Dash_Air_Start:
		break;
	case War::War_DashTo_Back:
		break;
	case War::War_Atk_Light_01:
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame))
		{
			//키 상태가 확인되면 
			if (m_eKeyState == War::LBUTTON )
			{
				m_eCurAniState = War::War_Atk_Light_02;
			}
			if ( m_eKeyState == War::RBUTTON)
			{
				m_eCurAniState = War::War_Atk_Heavy_01;
			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			//m_eCurAniState = War::War_Idle_Combat;
			m_eMachineState = War::STATE_IDLE_CB;
	
		}
		break;
	case War::War_Atk_Light_02:
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame))
		{
			//키 상태가 확인되면 
			if (m_eKeyState == War::LBUTTON )
			{
				m_eCurAniState = War::War_Atk_Light_03;
			}
			else if (m_eKeyState == War::RBUTTON)
			{
				m_eCurAniState = War::War_Atk_Heavy_01;
			}

		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			//m_eCurAniState = War::War_Idle_Combat;
			m_eMachineState = War::STATE_IDLE_CB;
		
		}
		break;
	case War::War_Atk_Light_03:
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame))
		{
			//키 상태가 확인되면 
			if (m_eKeyState == War::LBUTTON)
			{
				m_eCurAniState = War::War_Atk_Light_04;
	
			}
			else if (m_eKeyState == War::RBUTTON)
			{
				m_eCurAniState = War::War_Atk_Heavy_02;
	
			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			//m_eCurAniState = War::War_Idle_Combat;
			m_eMachineState = War::STATE_IDLE_CB;

		}
		break;
	case War::War_Atk_Light_04:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			//m_eCurAniState = War::War_Idle_Combat;
			m_eMachineState = War::STATE_IDLE_CB;
	
		}
		break;
	case War::War_Atk_Air_Light_01:
		break;
	case War::War_Atk_Air_Light_02:
		break;
	case War::War_Atk_Air_Filpsaw_Start:
		break;
	case War::War_Atk_Air_Filpsaw_Loop:
		break;
	case War::War_Atk_Air_Filpsaw_Land:
		break;
	case War::War_Atk_Dash:
		break;
	case War::War_Atk_Earth_Start:
		break;
	case War::War_Atk_Earth_Loop:
		break;
	case War::War_Atk_Earth_End:
		break;
	case War::War_Atk_Flamebrand:
		break;
	case War::War_Atk_Flamebrand_Start:
		break;
	case War::War_Atk_Flamebrand_End:
		break;
	case War::War_Chest_Open:
		break;
	case War::War_Death:
		break;
	case War::War_Block_Idle:
		break;
	case War::War_Block_Impact_Heavy:
		break;
	case War::War_Block_Impact_Light:
		break;
	case War::War_Block_Impact_Medium:
		break;
	case War::War_Block_Start:
		break;
	case War::War_Knockback_Fall:
		break;
	case War::War_Knockback_Land:
		break;
	case War::War_Knockback_Loop:
		break;
	case War::War_Knockback_Start:
		break;
	case War::War_Impack_From_Back:
		break;
	case War::War_Impack_From_Front:
		break;
	case War::War_Impack_From_Left:
		break;
	case War::War_Impack_From_Right:
		break;
	case War::War_Skill_01:
		break;
	case War::War_Skill_02_Start:
		break;
	case War::War_Skill_02_Strike:
		break;
	case War::War_Skill_02_Run:
		break;
	case War::War_Skill_03:
		break;
	case War::War_Glide:
		break;
	case War::War_Glide_Start:
		break;
	case War::War_Atk_Heavy_01:
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame))
		{
			//키 상태가 확인되면 
			if (m_eKeyState == War::LBUTTON)
			{
				m_eCurAniState = War::War_Atk_Heavy_02;

			}
			else if (m_eKeyState == War::RBUTTON)
			{
				m_eCurAniState = War::War_Atk_Heavy_02;

			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			//m_eCurAniState = War::War_Idle_Combat;
			m_eMachineState = War::STATE_IDLE_CB;

		}
		break;
	case War::War_Atk_Heavy_02:
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame))
		{
			//키 상태가 확인되면 
			if (m_eKeyState == War::LBUTTON)
			{
				m_eCurAniState = War::War_Atk_Heavy_03;

			}
			else if (m_eKeyState == War::RBUTTON)
			{
				m_eCurAniState = War::War_Atk_Heavy_03;

			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			//m_eCurAniState = War::War_Idle_Combat;
			m_eMachineState = War::STATE_IDLE_CB;

		}
		break;
	case War::War_Atk_Heavy_03:
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame))
		{
			//키 상태가 확인되면 
			if (m_eKeyState == War::LBUTTON)
			{
				m_eCurAniState = War::War_Atk_Light_01;

			}
			else if (m_eKeyState == War::RBUTTON)
			{
				//속성에 맞는 속성공격 ㄱㄱ
				m_eCurAniState = War::War_Atk_Heavy_01;
			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			//m_eCurAniState = War::War_Idle_Combat;
			m_eMachineState = War::STATE_IDLE_CB;
		}
		break;
	case War::War_End:
		break;
	}

}


void CPlayer::RunState()
{
	if (!m_bCombat)
	{
		m_pMeshCom->Set_AnimationIndex(War::War_Run);
	}
	else
	{
		m_pMeshCom->Set_AnimationIndex(War::War_Run_F_Combat);
	}
}

//void CPlayer::SetNextAniPos()
//{
//	//현재 로테이션 값으로 회전시키고 
//	
//	_vec3 vBonsPos = m_pMeshCom->GetBonePos("null_Spine");
//	_vec3 vPos;
//	m_pTransformCom->Get_INFO(INFO_POS, &vPos);
//
//	_vec3 vScale = m_pTransformCom->Get_Scale();
//
//	_vec3 vNextPos = _vec3{ (vBonsPos.x* vScale.x), (vBonsPos.y* vScale.y), (vBonsPos.z* vScale.z) }+m_vPreAniPos;
//
//	vNextPos.y = vPos.y;
//	m_pTransformCom->Set_Pos(&vNextPos);
//}


CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer*	pInstance = new CPlayer(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring ProtoMesh, _bool bColMode)
{
	CPlayer*	pInstance = new CPlayer(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Free(void)
{
	CGameObject::Free();
}

HRESULT CPlayer::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Prototype(L"War"));
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

	return S_OK;
}

void CPlayer::Key_Input(const _float & fTimeDelta)
{
	m_pTransformCom->Get_INFO(INFO_LOOK, &m_vDir);

	//if (!m_bCombo)
	//{
	//	if (!m_bCombat)
	//	{
	//		m_eCurAniState = War::War_Idle;
	//	}
	//	else
	//	{
	//		m_eCurAniState = War::War_Idle_to_Idle_Combat;
	//		m_eDir = War::IDLE;
	//	}
	//}
	m_eDir = War::IDLE;

	if (Key_Pressing(KEY_W))
	{
		m_eDir = War::UP;

		D3DXVec3Normalize(&m_vDir, &m_vDir);
		_vec3	vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		//m_pTransformCom->Set_Pos(&m_pNaviCom->MoveOn_NaviMesh(&vPos, &m_vDir, 5.f, fTimeDelta));
	}
	else if (Key_Pressing(KEY_S))
	{
		m_eDir = War::DOWN;
	}

	if (Key_Pressing(KEY_A))
	{
		m_eDir = War::LEFT;
		if (Key_Pressing(KEY_W))
			m_eDir = War::UP_LEFT;
		if(Key_Pressing(KEY_S))
			m_eDir = War::DOWN_LEFT;
	}
	else if (Key_Pressing(KEY_D))
	{
		m_eDir = War::RIGHT;
		if (Key_Pressing(KEY_W))
			m_eDir = War::UP_RIGHT;
		if (Key_Pressing(KEY_S))
			m_eDir = War::DOWN_RIGHT;
	}

	if (Key_Down(KEY_LBUTTON))
	{
		m_eKeyState = War::LBUTTON;
		m_eMachineState = War::ATTACK;

		//m_bCombat = true;
	}
	if (Key_Down(KEY_RBUTTON))
	{
		m_eKeyState = War::RBUTTON;
		m_eMachineState = War::ATTACK;
		//m_bCombat = true;
	}

	//m_pTransformCom->Move_Pos(&vDir, 5.f, fTimeDelta);

	//콤보상태가 아니면 달리기 
	if (m_eMachineState == War::ATTACK)
	{
		if (m_eKeyState == War::LBUTTON)
		{

		}
		else if (m_eKeyState == War::RBUTTON)
		{

		}
	}
	else
	{

	}


	//RunState();

	DirSet(m_eDir, fTimeDelta);

	//if (true == m_pMeshCom->Is_AnimationsetFinish())
	//	m_pMeshCom->Set_AnimationIndex(War::Ani::War_Idle);

}


void CPlayer::DirSet(War::DIR eDir, _float fTimeDelta)
{
	_vec3 vLook;
	m_pTransformCom->Get_INFO(INFO_LOOK, &vLook);
	_vec3 vRight = _vec3(1.f, 0.f, 0.f);
	_vec3 vCross;
	D3DXVec3Cross(&vCross, &vLook, &vRight);

	D3DXVec3Normalize(&vLook, &vLook);
	_float fCos = D3DXVec3Dot(&vLook, &vRight);
	_float fAngle = D3DXToDegree(acosf(fCos));

	_float fAngleOffset = 3.f;

	if (vCross.y < 0.f)
	{
		fAngle = 360.f - fAngle;
	}

	switch (eDir)
	{
	case War::UP:
		if (vCross.y > 0.f)
		{
			if (fAngle < 90.f - fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-180.f * fTimeDelta));
			}
			else if (fAngle > 90.f + fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
			}
		}
		else
		{
			if (fAngle <= 270.f)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
			}
			else if (fAngle > 270.f)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-180.f * fTimeDelta));
			}

		}
		break;
	case War::UP_LEFT:
		if (vCross.y > 0.f)
		{
			if (fAngle < 135.f - fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-180.f * fTimeDelta));
			}
			else if (fAngle > 135.f + fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
			}
		}
		else
		{
			m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
		}
		break;
	case War::UP_RIGHT:
		if (vCross.y > 0.f)
		{
			if (fAngle < 45.f - fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-180.f * fTimeDelta));
			}
			else if (fAngle > 45.f + fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
			}
		}
		else
		{
			m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-180.f * fTimeDelta));
		}
		break;
	case War::LEFT:
		if (vCross.y > 0.f)
		{
			if (fAngle < 180.f - fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-180.f * fTimeDelta));
			}
		}
		else
		{
			if (fAngle > 180.f + fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
			}
		}
		break;
	case War::RIGHT:
		if (vCross.y > 0.f)
		{
			if (fAngle > fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
			}
		}
		else
		{
			if (fAngle < 360.f - fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-180.f * fTimeDelta));
			}
		}
		break;
	case War::DOWN:
		if (vCross.y > 0.f)
		{
			if (fAngle >= 90.f)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-180.f * fTimeDelta));
			}
			else if (fAngle < 90.f)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
			}
		}
		else
		{
			if (fAngle > 270.f + fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
			}
			else if (fAngle < 270.f - fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-180.f * fTimeDelta));
			}

		}

		break;
	case War::DOWN_LEFT:
		if (vCross.y > 0.f)
		{
			m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-180.f * fTimeDelta));

		}
		else
		{
			if (fAngle < 225.f - fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-180.f * fTimeDelta));
			}
			else if (fAngle > 225.f + fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
			}
		}
		break;
	case War::DOWN_RIGHT:
		if (vCross.y > 0.f)
		{
			m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
		}
		else
		{
			if (fAngle < 315.f - fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-180.f * fTimeDelta));
			}
			else if (fAngle > 315.f + fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
			}
		}
		break;
	case War::IDLE:
		break;
	case War::DIRECTION_END:
		break;
	}
}