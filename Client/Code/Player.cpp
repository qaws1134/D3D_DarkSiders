#include "stdafx.h"
#include "Player.h"
#include "Enum.h"
#include "Export_Function.h"
#include "UIMgr.h"
#include "GameMgr.h"
#include "Effect.h"
#include "StaticCamera.h"
#include "SoundMgr.h"
#include "ColMesh.h"
#include "Effect_Trail.h"
#include "EffectMesh.h"


#define	  MOVEROTSPEED 360.f


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
	//m_pTransformCom->Set_Rot(0.f, D3DXToRadian(45.f), 0.f);
	m_pTransformCom->Update_Component(0.f);
	m_pMeshCom->Set_AnimationIndex(0);
	m_eCharState = War::CHAR_IDLE;
	
	//������ üũ
	dAttackCheckFrame = 0.5;
	dDashCheckFrame = 0.5;
	dJumpLandCheckFrame = 0.7f;
	


	//��Ʈ ����
	m_fHitSpeed = 1.5f;
	m_fHitTime = 1.0f;



	//�۶��̵� ����
	m_fGlideTime = 0.3f;
	m_fGlideSpeed = 0.3f;
	
	m_fGlideEndSpeed = 2.5f;
	m_fGlideEndTime = 2.5f;
	m_ePreElement = War::ELEMENT_END;

	SetCharInfo(100.f, 1.f);
	m_eElement = War::ELEMENT_EARTH;		//������ �Ӽ�

	m_tTrail.iTrailCount = 30;
	m_tTrail.fTrailEmitRate = 0.0015f;
	m_tTrail.fTrailSize = 380.f;
	m_tTrail.vColor = { 1.f,1.f,1.f,0.8f };


	return S_OK;
}

void CPlayer::Late_Ready_Object()
{
}


_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	if (!m_pTransformCom)
		return 0;
	m_fFrameSpeed = fTimeDelta;
	if (!m_bHit)
	{
		m_bCol = false;
	}

	_int iExit = CGameObject::Update_Object(fTimeDelta);
	Key_Input(fTimeDelta);
	StateChange();
	StateActer(fTimeDelta);
	StateLinker(fTimeDelta);

	
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
} 

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
	
	m_mapComponent[0].erase(L"Com_Transform");
	m_mapComponent[0].clear();
	for_each(m_mapComponent[1].begin(), m_mapComponent[1].end(), CDeleteMap());
	m_mapComponent[1].clear();
	CGameObject::Free();
}




HRESULT CPlayer::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Engine::Clone_Prototype(L"War"));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	//// Renderer
	//pComponent = m_pRendererCom = Engine::Get_Renderer();
	//pComponent->AddRef();
	//NULL_CHECK_RETURN(m_pRendererCom, E_FAIL);
	//m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<CShader*>(Engine::Clone_Prototype(L"Proto_Shader_Mesh"));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent=m_pNavi = dynamic_cast<CNaviMesh*>(Engine::Clone_Prototype(L"Proto_Navi"));
	NULL_CHECK_RETURN(m_pNavi, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Navi", pComponent);

	return S_OK;
}

void CPlayer::Render_Object(void)
{

	m_pMeshCom->Play_Animation(m_fFrameSpeed);
	LPD3DXEFFECT	 pEffect = m_pShaderCom->Get_EffectHandle();
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), )

	_uint iMaxPass = 0;

	pEffect->Begin(&iMaxPass, NULL);		// 1���� : ���� ���̴� ������ ��ȯ�ϴ� pass�� �ִ� ����
											// 2���� : �����ϴ� ����� ���� FLAG
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Meshes(pEffect);
	//if(m_pNavi)
	//	m_pNavi->Render_NaviMesh();
	pEffect->EndPass();
	pEffect->End();


	Safe_Release(pEffect);
}
void CPlayer::Key_Input(const _float & fTimeDelta)
{
	InteractionTimer(fTimeDelta);
	if (Key_Down(KEY_F))
	{
		if (m_bUIOn)
			m_bUIOn = false;
		else
			m_bUIOn = true;
		m_fInteractionSpeed = 0.f;
	}


	//�Ӽ� ����â
	if (Key_Pressing(KEY_TAB))
	{
		if (!m_bUIOn)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_UI);
			CSoundMgr::Get_Instance()->PlaySound(L"ui_window_open.ogg", CSoundMgr::CHANNEL_UI);
			CUIMgr::GetInstance()->SetActiveElementUI(true);
			Stop_TimeDelta(L"Timer_Immediate", false);
			m_bUIShowing = true;
			m_bUIOn = true;
		}
	}
	else
	{
		if (CUIMgr::GetInstance()->GetElemetUIActive())
		{
			CUIMgr::GetInstance()->SetActiveElementUI(false);
			Stop_TimeDelta(L"Timer_Immediate", true);
			m_bUIShowing = false;
			m_bUIOn = false;
		}
	}

	if (Key_Down(KEY_I))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_UI);
		CSoundMgr::Get_Instance()->PlaySound(L"ui_window_open.ogg", CSoundMgr::CHANNEL_UI);
			if (!m_bUIOn)
			{
				CUIMgr::GetInstance()->SetActiveCoreTreeUI(true);
				CUIMgr::GetInstance()->SetActivePlayerInfo(true);

				Stop_TimeDelta(L"Timer_Immediate",false);
				m_bUIOn = true;
			}
			else
			{
				if (CUIMgr::GetInstance()->GetCoreTreeUIActive())
				{
					CUIMgr::GetInstance()->SetActiveCoreTreeUI(false);
					CUIMgr::GetInstance()->SetActivePlayerInfo(false);

					Stop_TimeDelta(L"Timer_Immediate", true);
					m_bUIOn = false;
				}
			}

	}
	if (Key_Down(KEY_NUM0))
	{
		m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
	}

	if (m_eMachineState == War::HIT)
		return;
	
	//if (m_bUIOn)
	//{
	//	return;
	//}
	m_pTransformCom->Get_INFO(INFO_LOOK, &m_vDir);
	m_pTransformCom->Get_INFO(INFO_RIGHT, &m_vRight);
	m_eDir = War::IDLE;
	//���� �ö��̴� false
	auto iter_find = find_if(m_mapColider.begin(), m_mapColider.end(), CTag_Finder(L"Col_Weapon0"));
	if (iter_find != m_mapColider.end())
		iter_find->second->SetActive(false);

	if (m_eMachineState != War::ATTACK &&
		m_eMachineState != War::DASH&&
		m_eMachineState != War::JUMP&&
		m_eMachineState != War::JUMP_CB&&
		m_eMachineState != War::HIT&&
		m_eMachineState != War::CHEST_OPNE)
	{
		if (m_eCharState == War::CHAR_IDLE)
			m_eMachineState = War::STATE_IDLE;
		else if (m_eCharState == War::COMBAT)
			m_eMachineState = War::STATE_IDLE_CB;

	}




	if (Key_Pressing(KEY_W))
	{
		m_eDir = War::UP;
		m_eKeyState = War::WASD;
	}
	else if (Key_Pressing(KEY_S))
	{
		m_eDir = War::DOWN;
		m_eKeyState = War::WASD;
	}

	if (Key_Pressing(KEY_A))
	{
		m_eDir = War::LEFT;
		if (Key_Pressing(KEY_W))
			m_eDir = War::UP_LEFT;
		if (Key_Pressing(KEY_S))
			m_eDir = War::DOWN_LEFT;
		m_eKeyState = War::WASD;
	}
	else if (Key_Pressing(KEY_D))
	{
		m_eDir = War::RIGHT;
		if (Key_Pressing(KEY_W))
			m_eDir = War::UP_RIGHT;
		if (Key_Pressing(KEY_S))
			m_eDir = War::DOWN_RIGHT;
		m_eKeyState = War::WASD;
	}

	if (Key_Down(KEY_SPACE))
	{
		if (m_eMachineState != War::ATTACK)
		{
			if (m_eCharState == War::COMBAT)
				m_eMachineState = War::JUMP_CB;
			else if (m_eCharState == War::CHAR_IDLE)
				m_eMachineState = War::JUMP;
		}
		m_fGlideSpeed = 0.f;
		m_fGlideEndSpeed = 0.f;

		m_eKeyState = War::SPACE;
	}

	//����
	if (Key_Pressing(KEY_SPACE))
	{
		m_bGlide = GlideTimer(fTimeDelta);
	}
	if (Key_Up(KEY_SPACE))
	{
 		//m_fGlideSpeed = 2.f;
		m_bGlide = false;
		m_fGlideEndSpeed = 0.f;
		//m_eKeyState = War::KEYSTATE_END;
		m_bGlideOn = false;
	}



	if (Key_Down(KEY_SHIFT))
	{
		m_eMachineState = War::DASH;
	}

	if (Key_Down(KEY_LBUTTON))
	{
		m_eKeyState = War::LBUTTON;
		m_eMachineState = War::ATTACK;
		m_fCToISpeed = 0.f;

	}
	if (Key_Pressing(KEY_RBUTTON))
	{
		m_fElementSpeed += fTimeDelta;
		if (m_fElementSpeed > m_fElementTime)
		{
			m_bElement = true;
		}
		else
			m_bElement = false;

		m_eKeyState = War::RBUTTON;
		m_eMachineState = War::ATTACK;
		m_fCToISpeed = 0.f;
	}
	//if (Key_Down(KEY_RBUTTON))
	//{
	//	m_eKeyState = War::RBUTTON;
	//	m_eMachineState = War::ATTACK;
	//	m_fCToISpeed = 0.f;
	//}


	//��ų
	if (Key_Down(KEY_1))
	{
		m_eMachineState = War::ATTACK;
		m_eKeyState = War::NUM1;
	}
	if (Key_Down(KEY_2))
	{
		m_eMachineState = War::ATTACK;
		m_eKeyState = War::NUM2;
	}
	if (Key_Down(KEY_3))
	{
		m_eMachineState = War::ATTACK;
		m_eKeyState = War::NUM3;
	}

	//����

	if (m_eDir != War::IDLE)
	{
		if (m_eMachineState != War::ATTACK &&
			m_eMachineState != War::DASH&&
			m_eMachineState != War::JUMP&&
			m_eMachineState != War::JUMP_CB&&
			m_eMachineState != War::HIT&&
			m_eMachineState != War::CHEST_OPNE)
		{
			D3DXVec3Normalize(&m_vDir, &m_vDir);
			DirSet(m_eDir, fTimeDelta, MOVEROTSPEED);
			m_eMachineState = War::MOVE;
		}
	}

	if (Key_Pressing(KEY_MBUTTON))
	{
		m_eMachineState = War::BLOCK;
		m_eCharState = War::COMBAT;
		m_fCToISpeed = 0.f;
	}


	////�ھ�Ʈ�� UI
	//if (Key_Down(KEY_O))
	//{
	//	if (!m_bUIOn)
	//	{
	//		CUIMgr::GetInstance()->SetActiveCoreTreeUI(true);
	//		CUIMgr::GetInstance()->SetActivePlayerInfo(true);

	//		Stop_TimeDelta(L"Timer_Immediate",false);
	//		m_bUIOn = true;
	//	}
	//	else
	//	{
	//		if (CUIMgr::GetInstance()->GetCoreTreeUIActive())
	//		{
	//			CUIMgr::GetInstance()->SetActiveCoreTreeUI(false);
	//			CUIMgr::GetInstance()->SetActivePlayerInfo(false);

	//			Stop_TimeDelta(L"Timer_Immediate", true);
	//			m_bUIOn = false;
	//		}
	//	}
	//}

	////���� Ű 
	//if (Key_Down(KEY_P))
	//{
	//	if (!m_bUIOn)
	//	{
	//		CUIMgr::GetInstance()->SetActiveStoreActiveUI(true);

	//		Stop_TimeDelta(L"Timer_Immediate", false);
	//		m_bUIOn = true;
	//	}
	//	else
	//	{
	//		if (CUIMgr::GetInstance()->GetStoreUIActive())
	//		{
	//			CUIMgr::GetInstance()->SetActiveStoreActiveUI(false);

	//			Stop_TimeDelta(L"Timer_Immediate", true);
	//			m_bUIOn = false;
	//		}
	//	}
	//}

	//if (Key_Down(KEY_I))
	//{
	//	if (!m_bUIOn)
	//	{
	//		CUIMgr::GetInstance()->SetActiveStoreStoneUI(true);

	//		Stop_TimeDelta(L"Timer_Immediate", false);
	//		m_bUIOn = true;
	//	}
	//	else
	//	{
	//		if (CUIMgr::GetInstance()->GetStoreUIActive())
	//		{
	//			CUIMgr::GetInstance()->SetActiveStoreStoneUI(false);

	//			Stop_TimeDelta(L"Timer_Immediate", true);
	//			m_bUIOn = false;
	//		}
	//	}
	//}
	//if (Key_Down(KEY_NUM0))
	//{
	//	m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
	//}

	//���� �� Ȯ�� UI
	//if (Key_Down(KEY_P))
	//{
	//	if (!bUIOn)
	//	{
	//		CUIMgr::GetInstance()->SetActiveCoreTreeUI(true);
	//		Stop_TimeDelta(L"Timer_Immediate", false);
	//		bUIOn = true;
	//	}
	//	else
	//	{
	//		if (CUIMgr::GetInstance()->GetCoreTreeUIActive())
	//		{
	//			CUIMgr::GetInstance()->SetActiveCoreTreeUI(false);
	//			Stop_TimeDelta(L"Timer_Immediate", true);
	//			bUIOn = false;
	//		}
	//	}
	//}
	//�浹ü ��ȣ�ۿ�

	//m_pTransformCom->Move_Pos(&vDir, 5.f, fTimeDelta);

	//if (Key_Down(KEY_NUM1))
	//{
	//	Set_PlayerState(War::HIT);
	//	Take_Dmg(4.f);
	//}

	//if (Key_Down(KEY_NUM2))
	//{
	//	Set_PlayerState(War::HIT);
	//	Take_Dmg(6.f);
	//}


	//if (Key_Down(KEY_NUM3))
	//{
	//	Set_PlayerState(War::HIT);
	//	Take_Dmg(11.f);
	//}

	//�׺�Ž�
	if (m_pNavi)
	{
		////5.f �ӵ��� �̵�.
		////y�� �����ͼ� ���� �ּ� y���� �������� -> ���� ��� y�� ������ -> �׺� y�� �����ϸ� �ɵ�?
		_vec3	vPos;
		
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_fJumpY = m_pNavi->MoveOn_NaviMesh(&vPos, &m_vDir, 1.f, fTimeDelta, m_pCalculatorCom).y;

		_uint Idx = m_pNavi->Get_CellIndex();
		CGameMgr::GetInstance()->SetPlayerNaviIdx(m_pNavi->Get_CellIndex());

	}
}

void CPlayer::StateChange()
{

	if (m_ePreElement != m_eElement)
	{
		switch (m_eElement)
		{
		case War::ELEMENT_EARTH:
			m_tTrail.iTrailCount = 40;
			m_tTrail.fTrailEmitRate = 0.00015f;
			m_tTrail.fTrailSize = 400.f;
			m_tTrail.iTextureNum = 4;
			m_tTrail.vColor = { 0.2f,0.7f,0.2f,0.7f };
			break;
		case War::ELEMENT_FLAME:
			m_tTrail.iTrailCount = 30;
			m_tTrail.fTrailEmitRate = 0.00015f;
			m_tTrail.fTrailSize = 400.f;
			m_tTrail.iTextureNum = 1;
			m_tTrail.vColor = { 0.9f,0.5f,0.1f,0.7f };
			break;
		case War::ELEMENT_LIGHTNING:
			m_tTrail.iTrailCount = 40;
			m_tTrail.fTrailEmitRate = 0.00015f;
			m_tTrail.fTrailSize = 400.f;
			m_tTrail.iTextureNum = 4;
			m_tTrail.vColor = { 1.f,0.9f,0.1f,0.7f };
			break;
		case War::ELEMENT_VAMP:
			m_tTrail.iTrailCount = 30;
			m_tTrail.fTrailEmitRate = 0.00015f;
			m_tTrail.fTrailSize = 400.f;
			m_tTrail.iTextureNum = 1;
			m_tTrail.vColor = { 0.8f,0.1f,0.1f,0.7f };
			break;
		case War::ELEMENT_WIND:
			m_tTrail.iTrailCount = 40;
			m_tTrail.fTrailEmitRate = 0.00015f;
			m_tTrail.fTrailSize = 400.f;
			m_tTrail.iTextureNum = 4;
			m_tTrail.vColor = { 0.7f,0.7f,0.7f,0.7f };
			break;
		case War::ELEMENT_DEATH:
			m_tTrail.iTrailCount = 40;
			m_tTrail.fTrailEmitRate = 0.00015f;
			m_tTrail.fTrailSize = 400.f;
			m_tTrail.iTextureNum = 4;
			m_tTrail.vColor = { 0.f,0.6f,0.9f,0.7f };
			break;
		case War::ELEMENT_END:
			break;
		}
		if (!m_bTrail)
		{
			CGameObject* pTrail = Engine::Get_GameObject(L"Environment", L"Trail");
			if (pTrail)
			{
				const _matrix *BoneMat;
				const D3DXFRAME_DERIVED*		pFrame = m_pMeshCom->Get_FrameByName("Bone_War_Weapon_Sword");
				BoneMat = &pFrame->CombinedTransformMatrix;

					dynamic_cast<CEffect_Trail*>(pTrail)->Set_MatrixInfo(m_pTransformCom->Get_WorldMatrix(), BoneMat);
				m_tOutTrail = m_tTrail;
				m_bTrail = true;
			}

		}		//else
			//pTrail->SetOption(&m_tOutTrail);
		m_ePreElement = m_eElement;
	}

	if (Key_Down(KEY_4))
	{
		m_eMachineState = War::STATE_IDLE_CB;
	}
	if (Key_Down(KEY_5))
	{
		m_eMachineState = War::MOVE;
	}
	//�÷��̾� ���� ��ȯ �� 
	if (m_ePreMachineState != m_eMachineState)
	{
		switch (m_eMachineState)
		{
		case War::STATE_IDLE:
			if (m_ePreMachineState == War::STATE_IDLE_CB || m_ePreMachineState == War::HIT)
			{
				m_eCurAniState = War::War_Idle_Combat_to_Idle;
				m_bBlend = false;
			}
			else if (m_ePreMachineState == War::JUMP )
			{
				m_eCurAniState = War::War_Idle;
				m_bBlend = false;
			}
			else
			{
				m_eCurAniState = War::War_Idle;
				m_bBlend = true;
			}
			m_eCharState = War::CHAR_IDLE;
			break;
		case War::STATE_IDLE_CB:
			if (m_ePreMachineState == War::STATE_IDLE)
			{
				m_eCurAniState = War::War_Idle_to_Idle_Combat;
			}
			else if (m_ePreMachineState == War::MOVE)
			{
				m_eCurAniState = War::War_Idle_Combat;
				m_bBlend = true;
			}
			else if (m_ePreMachineState == War::BLOCK)
			{
				m_bBlend = true;
				m_eCurAniState = War::War_Idle_Combat;
			}
			else
			{
				m_eCurAniState = War::War_Idle_Combat;
				m_bBlend = false;
			}
			m_eCharState = War::CHAR_IDLE;
			break;
		case War::MOVE:
			//ĳ���� ���� �Ǵ�
			if (m_eCharState == War::CHAR_IDLE)
			{
				m_eCurAniState = War::War_Run;
			}
			else if(m_eCharState== War::COMBAT)
			{
				m_eCurAniState = War::War_Run_F_Combat;
			}
			m_bJumpBall = false;
			m_bBlend = true;
			m_fSoundTime = 0.4f;
			break;

		case War::ATTACK:
			if (m_eKeyState == War::LBUTTON)
			{
				if (m_eCharState == War::AIR)
				{
					m_eCharState = War::AIR_COMBAT;
					m_eCurAniState = War::War_Atk_Air_Light_01;
				}
				else
					m_eCurAniState = War::War_Atk_Light_01;
			}
			if (m_eKeyState == War::RBUTTON)
			{
				if (m_eCharState != War::AIR)
				{
					m_eCurAniState = War::War_Atk_Heavy_01;
				}
			}
			if (m_eKeyState == War::NUM1)
			{
				m_eCurAniState = War::War_Skill_01;
			}
			if (m_eKeyState == War::NUM2)
			{
				m_eCurAniState = War::War_Skill_02_Start;
			}
			if (m_eKeyState == War::NUM3)
			{
				m_eCurAniState = War::War_Skill_03;
			}
			if (m_ePreMachineState == War::DASH)
			{
				m_eCurAniState = War::War_Atk_Dash;
			}

			if (m_eCharState == War::AIR )
			{
				m_eCharState = War::AIR_COMBAT;
			}
			else
				m_eCharState = War::COMBAT;

			m_bBlend = false;
			break;
		case War::BLOCK:
			if(m_ePreMachineState!=War::HIT)
				m_eCurAniState = War::War_Block_Start;
			break;
		case War::JUMP:

 			m_eCurAniState = War::War_Jump;
		
			m_bBlend = false;

			if (m_bJumpBall)
			{
				m_fJumpPower = 20.f;
				m_bJumpBall = false;
			}
			else
			{
				m_fJumpPower = 15.f;
			}
			m_fInitJumpPower = m_fJumpPower;
			//m_fJumpY = m_pTransformCom->Get_PosY();
			m_bJumpEnd = false;
			m_bJumpAniEnd = false;
			m_bNexAni = false;
			m_eCharState = War::AIR;
			m_fGlideEndSpeed = 0.f;
			break;
		case War::JUMP_CB:

			m_eCurAniState = War::War_Jump_Combat;

			
			m_eCharState = War::AIR;
			m_bBlend = false;

			if (m_bJumpBall)
			{
				m_fJumpPower = 20.f;
				m_bJumpBall = false;
			}
			else
			{
				m_fJumpPower = 15.f;
			}
			m_fInitJumpPower = m_fJumpPower;
			//m_fJumpY = m_pTransformCom->Get_PosY();
			m_bJumpEnd = false;
			m_bJumpAniEnd = false;
			m_bNexAni = false;
			m_fGlideEndSpeed = 0.f;
			break;
		case War::JUMPATTACK:
			//m_eCharState = War::AIR;
			//m_bBlend = false;
			//m_fJumpPower = 15.f;
			//m_fInitJumpPower = m_fJumpPower;
			//m_bJumpEnd = false;
			//m_bJumpAniEnd = false;
			//m_bNexAni = false;
			break;
		case War::DASH:
			m_eCurAniState = War::War_Dash;
			m_eCharState = War::COMBAT;
			DirSet_Combo();
			m_bBlend = false;
			break;
		case War::HIT:
			if (m_bHit)
				break;

			if (m_pColTarget)
			{
				_vec3 vTargetPos;
				_vec3 vPos;
				m_pTransformCom->Get_INFO(INFO_POS, &vPos);
				dynamic_cast<CTransform*>(m_pColTarget->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_INFO(INFO_POS, &vTargetPos);
				m_vHitDir = vPos - vTargetPos;
				m_vHitDir.y = 0.f;

				D3DXVec3Normalize(&m_vHitDir, &m_vHitDir);
			}
			if (m_ePreMachineState == War::BLOCK)
			{
				//��������ϋ�
				if(m_tCharInfo.fDmg < 2.f)
					m_eCurAniState = War::War_Block_Impact_Light;
				else if(m_tCharInfo.fDmg<3.f)
					m_eCurAniState = War::War_Block_Impact_Medium;
				else if (m_tCharInfo.fDmg< 4.f)
					m_eCurAniState = War::War_Block_Impact_Heavy;
				m_eCharState = War::COMBAT;
			}
			else
			{
				//�Ϲ� �����ϋ�
				if (m_tCharInfo.fDmg < 2.f)
				{
					//2���� ������ �Ϲ� ����Ʈ
					for (auto iter : m_mapColider)
					{
						wstring ColKey = iter.first;
						if (iter.second->GetCol())
						{
							if (L"Col_Front" == ColKey)
							{
								m_eCurAniState = War::War_Impack_From_Front;
							}
							else if (L"Col_Back" == ColKey)
							{
								m_eCurAniState = War::War_Impack_From_Left;
							}
							else if (L"Col_Left" == ColKey)
							{
								m_eCurAniState = War::War_Impack_From_Right;
							}
							else if (L"Col_Right" == ColKey)
							{
								m_eCurAniState = War::War_Impack_From_Back;
							}
							iter.second->SetCol(false);
						}
					}

					m_fCToISpeed = 0.f;
					m_eCharState = War::COMBAT;
				}
				else
				{
					m_eCurAniState = War::War_Knockback_Start;
					//���󰡱� 
					m_bBlend = true;
					m_fJumpPower = 30.f;
					m_fInitJumpPower = m_fJumpPower;
					//m_fJumpY = m_pTransformCom->Get_PosY();
					m_bJumpEnd = false;
					m_bJumpAniEnd = false;
					m_bNexAni = false;
					m_eCharState = War::AIR;
				}
			}
			m_fHitMoveSpeed = 50.f;
			m_fHitSpeed = 0.f;
			m_tCharInfo.fDmg = 0.f;
			break;
		case War::CHEST_OPNE:
			m_eCurAniState = War::War_Idle;
			m_bChestOpen = false;
			m_fChestOpenSpeed = 0.f;
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
		m_tOutTrail = m_tTrail;
		

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
		case War::War_Jump_Combat:
			m_eKeyState = War::KEYSTATE_END;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_jump_02.ogg", CSoundMgr::CHANNEL_PLAYER);
			break;
		case War::War_Jump_Combat_Land:
			break;
		case War::War_Jump_Combat_Land_Run:
			break;
		case War::War_Jump_Double:
			m_eKeyState = War::KEYSTATE_END;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_jumpdouble_01.ogg", CSoundMgr::CHANNEL_PLAYER);
			break;
		case War::War_Jump_Fall:
		case War::War_Jump_Fall_Combat:
			m_eKeyState = War::KEYSTATE_END;
			break;
		case War::War_Jump_Land:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_jump_ledge_02.ogg", CSoundMgr::CHANNEL_PLAYER);

			break;
		case War::War_Jump_Land_Heavy:
			break;
		case War::War_Jump_Land_Run:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_jump_ledge_02.ogg", CSoundMgr::CHANNEL_PLAYER);

			break;
		case War::War_Dash:
			m_eKeyState = War::KEYSTATE_END;
			m_fInitDashSpeed = 1500.f;
			m_fDashSpeed = m_fInitDashSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_dash_1_02.ogg", CSoundMgr::CHANNEL_PLAYER);

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
			m_tCharInfo.fAtk = 1.f;
			m_eKeyState = War::KEYSTATE_END;
			DirSet_Combo();
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_attack_1_02.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
			m_fInitAttackMoveSpeed = 500.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
			break;
		case War::War_Atk_Light_02:
			m_tCharInfo.fAtk = 1.f;
			m_eKeyState = War::KEYSTATE_END;
			DirSet_Combo();
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK2);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_attack_2_02.ogg", CSoundMgr::CHANNEL_PLAYER_ATK2);
			m_fInitAttackMoveSpeed = 500.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
			break;
		case War::War_Atk_Light_03:
			m_tCharInfo.fAtk = 1.f;
			m_eKeyState = War::KEYSTATE_END;
			DirSet_Combo();

			m_fInitAttackMoveSpeed = 500.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
			m_bSound = false;

			break;
		case War::War_Atk_Light_04:
			m_tCharInfo.fAtk = 1.f;
			m_eKeyState = War::KEYSTATE_END;
			DirSet_Combo();
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_attack_4_a_02.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
			m_fInitAttackMoveSpeed = 500.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
			m_bSound = false;
			break;
		case War::War_Atk_Air_Light_01:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_attack_1_02.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
			m_bBlend = false;
			m_tCharInfo.fAtk = 1.f;
			m_eKeyState = War::KEYSTATE_END;
			break;
		case War::War_Atk_Air_Light_02:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_air_attack_3.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
			m_bBlend = false;
			m_tCharInfo.fAtk = 1.f;
			m_eKeyState = War::KEYSTATE_END;
			break;
		case War::War_Atk_Air_Filpsaw_Start:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_flamebrand_start_01.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
			m_bBlend = false;
			DirSet_Combo();
			break;
		case War::War_Atk_Air_Filpsaw_Loop:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_flamebrand_level1_loop.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
			m_fSoundTime = 0.5f;
			m_tOutTrail.fTrailSize = m_tTrail.fTrailSize + 40.f;
			m_tOutTrail.iTrailCount = m_tTrail.iTrailCount + 30;

			m_bBlend = false;
			m_bShake = false;
			break;
		case War::War_Atk_Air_Filpsaw_Land:
			m_eKeyState = War::KEYSTATE_END;
			m_tCharInfo.fAtk = 2.f;
			//DirSet_Combo();
			m_fInitAttackMoveSpeed = 500.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
			
			break;
		case War::War_Atk_Dash:
			//����
			m_fInitDashSpeed = 1500.f;
			m_fDashSpeed = m_fInitDashSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_attack_dash.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
			break;
		case War::War_Atk_Earth_Start:
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_special_charge.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
			//����
			DirSet_Combo();
			m_bBlend = true;
		}
			break;
		case War::War_Atk_Earth_Loop:
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_special_charge_loop.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
			CGameObject* pObj = CGameMgr::GetInstance()->GetEffect((_uint)EFFECT::EFFECT_ACT1_2x2);
			m_fSoundTime = 0.5f;
			_vec3 vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			vPos.y += 2.f;
			pObj->SetPos(vPos, ID_DYNAMIC);
			dynamic_cast<CEffect*>(pObj)->SetEffColor(_vec4{ 0.1f,0.9f,0.1f,1.f });
			m_bBlend = false;
			//����
		}
			break;
		case War::War_Atk_Earth_End:
		{
			CGameObject* pEff =CGameMgr::GetInstance()->GetEffect3D(EFFECT::EFFECT3D_STONECLUSTER);
			dynamic_cast<CEffectMesh*>(pEff)->SetDir(m_vDir);
			_vec3 vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			_vec3 vNormalDir = m_vDir;
			D3DXVec3Normalize(&vNormalDir, &vNormalDir);
			pEff->SetPos(vPos + (vNormalDir), ID_DYNAMIC);

			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_earthsplitter_land.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
			m_eKeyState = War::KEYSTATE_END;
			//����

			m_fInitAttackMoveSpeed = 800.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
		}
			break;
		case War::War_Atk_Lightning:
		{
			m_eKeyState = War::KEYSTATE_END;
			DirSet_Combo();
			m_bSound = false;
			m_fInitAttackMoveSpeed = 500.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;

			break;
		}
		case War::War_Atk_Flamebrand:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_flamebrand_level1_loop.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
			m_tCharInfo.fAtk = 1.f;
			m_fSoundTime = 1.f;
			DirSet_Combo();
			m_tOutTrail.fTrailSize = m_tTrail.fTrailSize + 40.f;
			m_tOutTrail.iTrailCount = m_tTrail.iTrailCount + 70;
			break;
		case War::War_Atk_Flamebrand_Start:

			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_flamebrand_start_01.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
			break;
		case War::War_Atk_Flamebrand_End:
			m_tCharInfo.fAtk = 1.f;
			m_bBlend = false;
			m_fInitAttackMoveSpeed = 800.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_flamebrand_end_level1.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
			break;
		case	War::War_Atk_LoomingDeath:
			m_tCharInfo.fAtk = 3.f;
			m_eKeyState = War::KEYSTATE_END;
			DirSet_Combo();
			m_fInitAttackMoveSpeed = 500.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
			m_bNexAni2 = false;
			m_bSound = false;

			break;
		case	War::War_Atk_Vamp_Start:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_vampiric_flipsaw_start.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
			DirSet_Combo();
			m_fInitAttackMoveSpeed = 800.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
			break;
		case	War::War_Atk_Vamp_Loop:
			m_fSoundTime = 0.5f;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_vampiric_flipsaw_loop.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
			m_tCharInfo.fAtk = 1.f;
			m_tOutTrail.fTrailSize = m_tTrail.fTrailSize + 70.f;
			m_tOutTrail.iTrailCount = m_tTrail.iTrailCount + 30;

			break;
		case	War::War_Atk_Vamp_Finish:
			dynamic_cast<CStaticCamera*>(CGameMgr::GetInstance()->GetCamera())->CameraShake(0.25f, 0.1f);
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_vampiric_grind_1.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
			m_tCharInfo.fAtk = 3.f;
			m_fInitAttackMoveSpeed = 800.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
			m_tOutTrail.fTrailSize = m_tTrail.fTrailSize + 150.f;
			m_tOutTrail.iTrailCount = m_tTrail.iTrailCount + 30;

			break;
		case	War::War_Atk_Launch_A:
		case	War::War_Atk_Launch_B:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_attack_launch_02.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
			m_tCharInfo.fAtk = 1.f;
			DirSet_Combo();
			m_bBlend = false;					
			m_fJumpPower = 15.f;
			m_fInitJumpPower = m_fJumpPower;
			//m_fJumpY = m_pTransformCom->Get_PosY();
			m_bJumpEnd = false;
			m_bJumpAniEnd = false;
			m_bNexAni = false;
			m_eCharState = War::AIR;
			break;
		case	War::War_Atk_Wind_Start   :
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_special_charge.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
			DirSet_Combo();
			break;
		case	War::War_Atk_Wind_Loop    :
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_special_charge_loop.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
			m_fSoundTime = 0.5f;
			break;
		case	War::War_Atk_Wind_End :
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_special_charge_indicator.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
			m_tCharInfo.fAtk = 3.f;
			m_fInitAttackMoveSpeed = 800.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
			m_bShake = false;
			break;
		case War::War_Chest_Open:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_chest_open.ogg", CSoundMgr::CHANNEL_PLAYER);
			m_bBlend = false;
			break;
		case War::War_Death:
			break;
		case War::War_Block_Idle:
			m_bBlend = false;
			break;
		case War::War_Block_Impact_Heavy:

			dynamic_cast<CStaticCamera*>(CGameMgr::GetInstance()->GetCamera())->CameraShake(0.3f, 0.1f);
			m_bBlend = true;
			m_fInitAttackMoveSpeed = 8.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"imp_sword_01.ogg", CSoundMgr::CHANNEL_PLAYER);

			break;
		case War::War_Block_Impact_Light:
			dynamic_cast<CStaticCamera*>(CGameMgr::GetInstance()->GetCamera())->CameraShake(0.2f, 0.1f);
			m_bBlend = true;
			m_fInitAttackMoveSpeed = 5.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"imp_sword_01.ogg", CSoundMgr::CHANNEL_PLAYER);
			break;
		case War::War_Block_Impact_Medium:
			dynamic_cast<CStaticCamera*>(CGameMgr::GetInstance()->GetCamera())->CameraShake(0.25f, 0.1f);
			m_bBlend = true;
			m_fInitAttackMoveSpeed = 6.5f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"imp_sword_01.ogg", CSoundMgr::CHANNEL_PLAYER);
			break;
		case War::War_Block_Start:
			m_bBlend = false;
			break;
		case War::War_Knockback_Fall:
			m_fJumpPower = 15.f;
			m_fInitJumpPower = m_fJumpPower;
			break;
		case War::War_Knockback_Land:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_jumpdouble_01.ogg", CSoundMgr::CHANNEL_PLAYER);
			m_bBlend = true;
			break;
		case War::War_Knockback_Loop:
			m_fJumpPower = 15.f;
			m_fInitJumpPower = m_fJumpPower;
			m_bBlend = false;
			break;
		case War::War_Knockback_Start:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"imp_fire_03.ogg", CSoundMgr::CHANNEL_PLAYER);
			dynamic_cast<CStaticCamera*>(CGameMgr::GetInstance()->GetCamera())->CameraShake(0.3f, 0.1f);
			m_fJumpPower = 15.f;
			m_fInitJumpPower = m_fJumpPower;
			m_bBlend = false;
			break;
		case War::War_Impack_From_Back:
		case War::War_Impack_From_Front:
		case War::War_Impack_From_Left:
		case War::War_Impack_From_Right:
			dynamic_cast<CStaticCamera*>(CGameMgr::GetInstance()->GetCamera())->CameraShake(0.1f, 0.1f);
			m_fInitAttackMoveSpeed = 800.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;

			break;
		case War::War_Skill_01:
			m_bSound = false;
			m_tCharInfo.fAtk = 3.f;
			m_eKeyState = War::KEYSTATE_END;
			DirSet_Combo();
			m_fInitAttackMoveSpeed = 800.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
			break;
		case War::War_Skill_02_Start:
			m_eKeyState = War::KEYSTATE_END;
			DirSet_Combo();
			m_fInitAttackMoveSpeed = 800.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
			m_bSkill2End = false;
			break;
		case War::War_Skill_02_Strike:
			m_bBlend = true;
			break;
		case War::War_Skill_02_Run:
			DirSet_Combo();
			//m_fInitAttackMoveSpeed = 800.f;
		//	m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
		//	m_bNexAni = false;
		//	m_bAttackMoveEnd = false;
			break;
		case War::War_Skill_03:
			m_eKeyState = War::KEYSTATE_END;
			m_bBlend = true;
			DirSet_Combo();
			m_fInitAttackMoveSpeed = 800.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
			break;
		case War::War_Glide:
			m_bSound = false;
			break;
		case War::War_Glide_Start:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"char_shared_glide.ogg", CSoundMgr::CHANNEL_PLAYER);
			m_bGlideOn = true;
			break;
		case War::War_Atk_Heavy_01:
			m_fElementSpeed = 0.f;
			m_bSound = false;
			m_tCharInfo.fAtk = 1.f;
			m_eKeyState = War::KEYSTATE_END;
			DirSet_Combo();
			m_fInitAttackMoveSpeed = 800.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
			break;

		case War::War_Atk_Heavy_02:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK2);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_attack_3_02.ogg", CSoundMgr::CHANNEL_PLAYER_ATK2);
			m_bSound = false;
			m_tCharInfo.fAtk = 1.f;
			m_eKeyState = War::KEYSTATE_END;
			DirSet_Combo();
			m_fInitAttackMoveSpeed = 800.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
			m_bShake = false;
			break;

		case War::War_Atk_Heavy_03:
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_attack_heavy_3.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
			m_tCharInfo.fAtk =1.f;
			m_eKeyState = War::KEYSTATE_END;
			DirSet_Combo();
			m_fInitAttackMoveSpeed = 800.f;
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed;
			m_bNexAni = false;
			m_bAttackMoveEnd = false;
			m_bShake = false;
			break;
		case War::War_End:
			break;
		}
		m_ePreAniState = m_eCurAniState;
		m_pMeshCom->Set_AnimationIndex(m_eCurAniState,m_bBlend);
	}

}
void CPlayer::StateActer(_float fDeltaTime)
{
	
	switch (m_eMachineState)
	{
	case War::STATE_IDLE:
		m_pTransformCom->Move_Pos(&m_vDir, 0.f, fDeltaTime);
		break;
	case War::STATE_IDLE_CB:
		m_pTransformCom->Move_Pos(&m_vDir, 0.f, fDeltaTime);
		break;
	case War::MOVE:
	{		
		_vec3 vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);

	}
	break;
	case War::ATTACK:
	{		
		if (m_bAttackMoveEnd)
		{
			m_fAttackMoveSpeed = m_fInitAttackMoveSpeed + m_fAddPower;
			m_bAttackMoveEnd = false;
			m_fAddPower = 0.f;
		}
		_vec3 vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		if (vPos.y < m_fJumpY)
		{
			m_bJumpEnd = true;
		}
		
	}
		//���ݿ� ���� �̵��� 
		break;
	case War::BLOCK:
		//if(//�浹 -> �˹�)
		//{

		//}
		break;

	case War::JUMP:
	case War::JUMP_CB:
	{	
		
		//������
		if (m_bJumpBall)
		{
			//m_eKeyState = War::KEYSTATE_END;
			m_fGlideSpeed = 0.f;
			m_ePreMachineState = War::STATE_IDLE;
			m_fJumpPower = m_fInitJumpPower;
			m_bJumpEnd = false;

			break;
		}

		if (m_bJumpAniEnd)
		{
			m_fJumpPower = m_fInitJumpPower+ m_fAddPower;
			m_bJumpAniEnd = false;
			m_fAddPower = 0.f;
		}
		//�������� �Ǵ� 
		if (m_pMeshCom->Is_Animationset(0.1))
		{
			_vec3 vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			if (vPos.y < m_fJumpY)
			{
				m_bJumpEnd = true;
			}
		}
		DirSet(m_eDir, fDeltaTime, MOVEROTSPEED);
		m_bGlideEnd = GlideEndTimer(fDeltaTime);
	}
		break;
	case War::JUMPATTACK:
	{		
		_vec3 vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		if (vPos.y < m_fJumpY)
		{
			m_bJumpEnd = true;
		}
	}
		//�ȳ������� �Ǵ� 
		break;
	case War::DASH:
		//�ӵ� ���� �� ���� 
		if (m_bDashMoveEnd)
		{
			m_fDashSpeed = m_fInitDashSpeed + m_fAddPower;
			m_bDashMoveEnd = false;
			m_fAddPower = 0.f;
		}
		break;
	case War::HIT:
		//������ ���� �˹�
		if (m_ePreMachineState == War::BLOCK)
		{

		}
		else
		{
			if (m_bJumpAniEnd)
			{
				m_fJumpPower = m_fInitJumpPower + m_fAddPower;
				m_bJumpAniEnd = false;
				m_fAddPower = 0.f;
			}
			//�������� �Ǵ� 
			_vec3 vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			if (vPos.y < m_fJumpY)
			{
				m_bJumpEnd = true;
			}
		}
		break;
	case War::CHEST_OPNE:
		if (!m_bChestOpen)
		{
			m_fChestOpenSpeed += fDeltaTime;
			if (m_fChestOpenSpeed > m_fChestOpenTime)
			{
				m_bChestOpen = true;
				m_eCurAniState = War::War_Chest_Open;
			}
		}
		break;
	case War::STATE_END:
		break;
	default:
		break;
	}

	//���� �� �̵� ����
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
	case War::War_Run_F_Combat:
	{
		_vec3	vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(&m_pNavi->MoveOn_NaviMesh(&vPos, &m_vDir, m_fMoveSpeed, fDeltaTime, m_pCalculatorCom));

		m_fSoundSpeed += fDeltaTime;
		if (m_fSoundSpeed > m_fSoundTime)
		{
			USES_CONVERSION;

			_uint iIdx = RandNext(0, 9);
			wstring wstrSound = L"char_war_foot_0";
			wstring wstrTag= L".ogg";
			wstrSound += to_wstring(iIdx);
			wstrSound += wstrTag;
			TCHAR* pTag = W2BSTR(wstrSound.c_str());
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(pTag,CSoundMgr::CHANNEL_PLAYER);
			m_fSoundSpeed = 0.f;
		}
	}
	break;
	case War::War_Jump:
	case War::War_Jump_Combat:
		if (!m_bJumpEnd)
		{
			if (m_bJumpBall)
			{
				m_pTransformCom->MoveStep(MOVETYPE_BREAK, &m_fJumpPower, 50.f, 0.f, &_vec3(0.f, 1.f, 0.f), fDeltaTime);
			}
			else
				m_pTransformCom->MoveStep(MOVETYPE_BREAK, &m_fJumpPower, 30.f, 0.f, &_vec3(0.f, 1.f, 0.f), fDeltaTime);
			m_pTransformCom->Move_Pos(&m_vDir, m_fMoveSpeed*100.f, fDeltaTime);
		}
		break;
	case War::War_Jump_Combat_Land:
		break;
	case War::War_Jump_Combat_Land_Run:
	case War::War_Jump_Land_Run:
		m_pTransformCom->Move_Pos(&m_vDir, m_fMoveSpeed*100.f, fDeltaTime);
		m_pTransformCom->Set_PosY(m_fJumpY);
		break;
	case War::War_Jump_Double:
		if (!m_bJumpEnd)
		{
			if (!m_bNexAni)
			{
				m_bJumpAniEnd = true;
				m_bNexAni = true;
			}

			m_pTransformCom->MoveStep(MOVETYPE_BREAK, &m_fJumpPower, 40.f, 0.f, &_vec3(0.f, 1.f, 0.f), fDeltaTime);
			m_pTransformCom->Move_Pos(&m_vDir, m_fMoveSpeed*100.f, fDeltaTime);
		}
		break;
	case War::War_Jump_Fall:
	case War::War_Jump_Fall_Combat:
		if (!m_bJumpEnd)
		{
			m_pTransformCom->MoveStep(MOVETYPE_ACC, &m_fJumpPower, 120.f, 100.f, &_vec3(0.f, -1.f, 0.f), fDeltaTime);
			m_pTransformCom->Move_Pos(&m_vDir, m_fMoveSpeed*100.f, fDeltaTime);
		}
		break;
	case War::War_Jump_Land:
	case War::War_Jump_Land_Heavy:
		m_pTransformCom->Set_PosY(m_fJumpY);
		break;
	case War::War_Dash:
		if (!m_pMeshCom->Is_Animationset(dDashCheckFrame - 0.35f))
		{
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_ACC, &m_fDashSpeed, 8000.f, 3800.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

		}
		else
		{
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fDashSpeed, 15000.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fDashSpeed, 15000.f, 0.f, &m_vDir, fDeltaTime);
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			if (!m_bNexAni)
			{
				m_bDashMoveEnd = true;
				m_bNexAni = true;
			}
		}
		m_pTransformCom->Set_PosY(m_fJumpY);
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
		//0~0.3
		AtkColActive(0, dAttackCheckFrame - 0.2);
		if (!m_pMeshCom->Is_Animationset(dAttackCheckFrame - 0.1))
		{
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 800.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

			//m_pTransformCom->Move_Pos(&m_vDir, m_fAttackMoveSpeed*10.f, fDeltaTime);
			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 800.f, 0.f, &m_vDir, fDeltaTime);
		}
		else
		{	//0.3~1.0
			if (!m_bNexAni)
			{
				m_bAttackMoveEnd = true;
				m_bNexAni = true;
			}
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 1000.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 1000.f, 0.f, &m_vDir, fDeltaTime);
		}
		m_pTransformCom->Set_PosY(m_fJumpY);
		break;
	case War::War_Atk_Light_02:
		AtkColActive(0, dAttackCheckFrame - 0.1);
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame - 0.3))
		{
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 900.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 900.f, 0.f, &m_vDir, fDeltaTime);
		}
		m_pTransformCom->Set_PosY(m_fJumpY);
		break;
	case War::War_Atk_Light_03:
		AtkColActive(0, dAttackCheckFrame - 0.1);
		if (!m_pMeshCom->Is_Animationset(dAttackCheckFrame - 0.2))
		{
			if (!m_bSound)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
				CSoundMgr::Get_Instance()->PlaySound(L"char_war_attack_3_02.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
				m_bSound = true;
			}
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 800.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

			//m_pTransformCom->Move_Pos(&m_vDir, m_fAttackMoveSpeed*10.f, fDeltaTime);
			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 800.f, 0.f, &m_vDir, fDeltaTime);
		}
		else
		{
			if (!m_bNexAni)
			{
				m_bAttackMoveEnd = true;
				m_bNexAni = true;
			}
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 2000.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 2000.f, 0.f, &m_vDir, fDeltaTime);
		}
		m_pTransformCom->Set_PosY(m_fJumpY);
		break;
	case War::War_Atk_Light_04:
		AtkColActive(dAttackCheckFrame - 0.1, dAttackCheckFrame);

		if (!m_pMeshCom->Is_Animationset(dAttackCheckFrame - 0.1))
		{
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 300.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 300.f, 0.f, &m_vDir, fDeltaTime);
		}
		m_pTransformCom->Set_PosY(m_fJumpY);
		break;
	case War::War_Atk_Air_Filpsaw_Start:
	case War::War_Atk_Air_Filpsaw_Loop:
		AtkColLoop();
		if (!m_bJumpEnd)
		{
			m_fSoundSpeed += fDeltaTime;
			if (m_fSoundSpeed > m_fSoundTime)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
				CSoundMgr::Get_Instance()->PlaySound(L"char_war_flamebrand_level1_loop.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
				m_fSoundSpeed = 0.f;
			}
			m_pTransformCom->MoveStep(MOVETYPE_ACC, &m_fJumpPower, 40.f, 70.f, &_vec3(0.f, -1.f, 0.f), fDeltaTime);
			m_pTransformCom->Move_Pos(&m_vDir, m_fMoveSpeed*150.f, fDeltaTime);
		}
		break;
	case War::War_Atk_Air_Filpsaw_Land:
		AtkColActive(0, dAttackCheckFrame - 0.1);
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame))
		{
			if (!m_bNexAni)
			{
				m_bAttackMoveEnd = true;
				m_bNexAni = true;
			}
			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 800.f, 0.f, &m_vDir, fDeltaTime);
		}
		//if (!m_bJumpEnd)
		//{
		//	m_pTransformCom->MoveStep(MOVETYPE_ACC, &m_fJumpPower, 40.f, 70.f, &_vec3(0.f, -1.f, 0.f), fDeltaTime);
		//	//m_pTransformCom->Move_Pos(&m_vDir, m_fMoveSpeed*100.f, fDeltaTime);
		//}
		break;
	case War::War_Atk_Dash:
	{
		AtkColActive(0, dAttackCheckFrame - 0.1);
		_vec3	vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		if (!m_pMeshCom->Is_Animationset(dDashCheckFrame - 0.35))
		{
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_ACC, &m_fDashSpeed, 6000.f, 3800.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_ACC, &m_fDashSpeed, 8000.f, 3800.f, &m_vDir, fDeltaTime);
		}
		else
		{
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fDashSpeed, 10000.f, 800.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fDashSpeed, 10000.f, 800.f, &m_vDir, fDeltaTime);
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			if (!m_bNexAni)
			{
				m_bDashMoveEnd = true;
				m_bNexAni = true;
			}
		}
		m_pTransformCom->Set_PosY(m_fJumpY);
	}
	break;
	case War::War_Atk_Earth_Start:
		break;
	case War::War_Atk_Earth_Loop:
		if (m_fSoundSpeed > m_fSoundTime)
		{
			m_fSoundSpeed = 0.f;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_special_charge_loop.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
		}
		break;
	case War::War_Atk_Earth_End:
	{
		m_bShake = true;
		_vec3	vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame + 0.1))
		{
			if (!m_bNexAni)
			{
				m_bAttackMoveEnd = true;
				m_bNexAni = true;
			}
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 2000.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 2000.f, 0.f, &m_vDir, fDeltaTime);
		}
		else
		{
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 2000.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 2000.f, 0.f, &m_vDir, fDeltaTime);

		}
		m_pTransformCom->Set_PosY(m_fJumpY);
	}
	break;
	case War::War_Atk_Flamebrand:
		AtkColLoop();
		m_pTransformCom->Move_Pos(&m_vDir, m_fMoveSpeed*100.f, fDeltaTime);
		m_pTransformCom->Set_PosY(m_fJumpY);
		m_fSoundSpeed += fDeltaTime;
		if (m_fSoundSpeed > m_fSoundTime)
		{
			m_fSoundSpeed = 0.f;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_flamebrand_level1_loop.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
		}

		break;
	case War::War_Atk_Flamebrand_Start:
	case War::War_Atk_Flamebrand_End:
	{
		AtkColActive(0, dAttackCheckFrame - 0.1);
		_vec3	vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 2000.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));
		//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 2000.f, 0.f, &m_vDir, fDeltaTime);


		m_pTransformCom->Set_PosY(m_fJumpY);
	}
	break;
	case War::War_Atk_Lightning:
	{
		AtkColActive(0.1, dAttackCheckFrame - 0.1);
		_vec3	vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		if (!m_pMeshCom->Is_Animationset(dAttackCheckFrame - 0.3))
		{
			//m_pTransformCom->Move_Pos(&m_vDir, m_fAttackMoveSpeed*10.f, fDeltaTime);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 1500.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 1500.f, 0.f, &m_vDir, fDeltaTime);
		}
		else
		{
			if (!m_bNexAni)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK2);
				CSoundMgr::Get_Instance()->PlaySound(L"char_war_thunderclap_hit_2.ogg", CSoundMgr::CHANNEL_PLAYER_ATK2);
				dynamic_cast<CStaticCamera*>(CGameMgr::GetInstance()->GetCamera())->CameraShake(0.25f, 0.1f);
				//����Ʈ 
				CGameObject* pObj = CGameMgr::GetInstance()->GetEffect((_uint)EFFECT::EFFECT_LIGHTNING_WEAPON_CLUSTER);
				_vec3 vPos;
				m_pTransformCom->Get_INFO(INFO_POS, &vPos);

				const _matrix *BoneMat;
				const D3DXFRAME_DERIVED*		pFrame = m_pMeshCom->Get_FrameByName("Bone_War_Weapon_Sword");
				BoneMat = &pFrame->CombinedTransformMatrix;

				_vec3 vNormalDir;
				D3DXVec3Normalize(&vNormalDir, &m_vDir);
				pObj->SetPos((vPos), ID_DYNAMIC);
				CTransform* pTrans=  dynamic_cast<CTransform*> (pObj->Get_Component(L"Com_Transform", ID_DYNAMIC));
				_matrix* pEffMat = m_pTransformCom->Get_WorldMatrix();
				pTrans->Set_WorldMatrix(&(*BoneMat  * *pEffMat));
				_vec3 vEffPos;
				pTrans->Get_INFO(INFO_POS, &vEffPos);
				pObj->SetPos((vEffPos+(vNormalDir*8.5f)), ID_DYNAMIC);

				pObj = CGameMgr::GetInstance()->GetEffect((_uint)EFFECT::EFFECT_EXPLOSION);
				pObj->SetPos((vEffPos + (vNormalDir*13.f)), ID_DYNAMIC);

				pObj = CGameMgr::GetInstance()->GetPlayerBullet(BULLET::BULLET_SPAWNFADE_PLAYER);
				pObj->SetPos((vEffPos + (vNormalDir*13.f)), ID_DYNAMIC);
				pObj = CGameMgr::GetInstance()->GetPlayerBullet(BULLET::BULLET_SPAWNFADE_PLAYER);
				pObj->SetPos((vEffPos + (vNormalDir*9.f)), ID_DYNAMIC);
				pObj = CGameMgr::GetInstance()->GetPlayerBullet(BULLET::BULLET_SPAWNFADE_PLAYER);
				pObj->SetPos((vEffPos + (vNormalDir*5.f)), ID_DYNAMIC);
				//////////////////////////////////////////////////////////////////////////
				m_bAttackMoveEnd = true;
				m_bNexAni = true;
			}
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_ACC, &m_fAttackMoveSpeed, 10000.f, 2200.f, &m_vDir, fDeltaTime, m_pCalculatorCom));
			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_ACC, &m_fAttackMoveSpeed, 10000.f, 2200.f, &m_vDir, fDeltaTime);
		}
		m_pTransformCom->Set_PosY(m_fJumpY);
	}
	break;
	case War::War_Atk_Wind_Start:
		break;
	case War::War_Atk_Wind_Loop:
		m_fSoundSpeed += fDeltaTime;
		if (m_fSoundSpeed > m_fSoundTime)
		{
			m_fSoundSpeed = 0.f;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_special_charge_loop.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
		}
		break;
	case War::War_Atk_Wind_End:
	{
		AtkColActive(0, dAttackCheckFrame - 0.1);
		if (!m_bNexAni)
		{
			m_bAttackMoveEnd = true;
			m_bNexAni = true;
		}
		_vec3	vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 1500.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

		//m_pTransformCom->Move_Pos(&m_vDir, m_fAttackMoveSpeed*10.f, fDeltaTime);
	//	m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 1500.f, 0.f, &m_vDir, fDeltaTime);
		m_pTransformCom->Set_PosY(m_fJumpY);
	}
	break;
	case War::War_Atk_LoomingDeath:
	{
		_vec3	vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		AtkColActive(0, dAttackCheckFrame + 0.3);
		if (!m_pMeshCom->Is_Animationset(dAttackCheckFrame - 0.3))
		{
			if (!m_bSound)
			{
				m_bSound = true;
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
				CSoundMgr::Get_Instance()->PlaySound(L"char_war_looming_death_start.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
				dynamic_cast<CStaticCamera*>(CGameMgr::GetInstance()->GetCamera())->CameraShake(0.1f, 0.1f);
			}
			//m_pTransformCom->Move_Pos(&m_vDir, m_fAttackMoveSpeed*10.f, fDeltaTime);

			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 800.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 800.f, 0.f, &m_vDir, fDeltaTime);
		}
		else if (!m_pMeshCom->Is_Animationset(dAttackCheckFrame + 0.19))
		{	//0.3~1.0
			if (!m_bNexAni)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK2);
				CSoundMgr::Get_Instance()->PlaySound(L"char_war_looming_death_land.ogg", CSoundMgr::CHANNEL_PLAYER_ATK2);
				m_bAttackMoveEnd = true;
				m_bNexAni = true;
				dynamic_cast<CStaticCamera*>(CGameMgr::GetInstance()->GetCamera())->CameraShake(0.1f, 0.1f);
			}

			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 500.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 500.f, 0.f, &m_vDir, fDeltaTime);
		}
		else
		{
			if (!m_bNexAni2)
			{

				m_bAttackMoveEnd = true;
				m_bNexAni2 = true;
		
			}

			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 1400.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 1400.f, 0.f, &m_vDir, fDeltaTime);
		}
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame-0.1))
		{	
			if (m_bSound)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
				CSoundMgr::Get_Instance()->PlaySound(L"char_war_looming_death_end.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
				dynamic_cast<CStaticCamera*>(CGameMgr::GetInstance()->GetCamera())->CameraShake(0.2f, 0.1f);
				m_bSound = false;
			}
		}
		m_pTransformCom->Set_PosY(m_fJumpY);

		break;
	}
	case War::War_Atk_Vamp_Start:
		AtkColLoop();
		if (!m_pMeshCom->Is_Animationset(dAttackCheckFrame - 0.07))
		{
			//m_pTransformCom->Move_Pos(&m_vDir, m_fAttackMoveSpeed*10.f, fDeltaTime);
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 800.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 800.f, 0.f, &m_vDir, fDeltaTime);
		}
		m_pTransformCom->Set_PosY(m_fJumpY);
		break;
	case War::War_Atk_Vamp_Loop:
		AtkColLoop();
		m_fSoundSpeed += fDeltaTime;
		if (m_fSoundSpeed > m_fSoundTime)
		{
			m_fSoundSpeed = 0.f;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
			CSoundMgr::Get_Instance()->PlaySound(L"char_war_vampiric_flipsaw_loop.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
		}
		break;
	case War::War_Atk_Vamp_Finish:
		AtkColActive(0, dAttackCheckFrame - 0.1);
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame - 0.3))
		{
			if (!m_bNexAni)
			{
				m_bAttackMoveEnd = true;
				m_bNexAni = true;
			}
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 2200.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

			//m_pTransformCom->Move_Pos(&m_vDir, m_fAttackMoveSpeed*10.f, fDeltaTime);
			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 2200.f, 0.f, &m_vDir, fDeltaTime);
		}
		m_pTransformCom->Set_PosY(m_fJumpY);
		break;
	case War::War_Atk_Launch_A:
	case War::War_Atk_Launch_B:
		AtkColActive(0, dAttackCheckFrame - 0.1);
		if (!m_bJumpEnd)
		{
			if (m_pMeshCom->Is_Animationset(dAttackCheckFrame - 0.2))
			{

				m_pTransformCom->MoveStep(MOVETYPE_BREAK, &m_fJumpPower, 30.f, 0.f, &_vec3(0.f, 1.f, 0.f), fDeltaTime);
				//m_pTransformCom->Move_Pos(&m_vDir, m_fMoveSpeed*100.f, fDeltaTime);
			}
		}
		break;
	case War::War_Atk_Air_Light_01:
		AtkColActive(0, dAttackCheckFrame - 0.2);
		break;
	case War::War_Atk_Air_Light_02:
		AtkColActive(0, dAttackCheckFrame - 0.2);
		break;
	case War::War_Chest_Open:

		break;
	case War::War_Death:
		break;
	case War::War_Block_Idle:
		break;
	case War::War_Block_Impact_Heavy:
	case War::War_Block_Impact_Light:
	case War::War_Block_Impact_Medium:
	{
		_vec3	vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 1000.f, 0.f, &m_vHitDir, fDeltaTime, m_pCalculatorCom));

		//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 1000.f, 0.f, &-m_vDir, fDeltaTime);
		m_pTransformCom->Set_PosY(m_fJumpY);
	}
	break;
	case War::War_Block_Start:
		break;
	case War::War_Knockback_Fall:
		//�򵥹����� �˹� �ڷ�����

		break;
	case War::War_Knockback_Land:
		m_pTransformCom->Set_PosY(m_fJumpY);
		break;
	case War::War_Knockback_Loop:
		if (!m_bJumpEnd)
		{

			m_pTransformCom->MoveStep(MOVETYPE_ACC, &m_fJumpPower, 150.f, 200.f, &_vec3(0.f, -1.f, 0.f), fDeltaTime);
			m_pTransformCom->Move_Pos(&m_vHitDir, m_fMoveSpeed*1.5f, fDeltaTime);
		}
		//m_pTransformCom->Set_PosY(m_fJumpY);
		break;
	case War::War_Knockback_Start:
		if (!m_bJumpEnd)
		{
			if (!m_bNexAni)
			{
				m_bJumpAniEnd = true;
				m_bNexAni = true;
			}
			m_pTransformCom->MoveStep(MOVETYPE_BREAK, &m_fJumpPower, 300.f, 0.f, &_vec3(0.f, 1.f, 0.f), fDeltaTime);
			m_pTransformCom->Move_Pos(&m_vHitDir, m_fMoveSpeed*1.5f, fDeltaTime);
		}

		break;
	case War::War_Impack_From_Back:
	{
		_vec3	vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fHitMoveSpeed, 3000.f, 0.f, &m_vHitDir, fDeltaTime, m_pCalculatorCom));

		//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 3000.f, 0.f, &m_vDir, fDeltaTime);
		m_pTransformCom->Set_PosY(m_fJumpY);
	}
	break;
	case War::War_Impack_From_Front:
	{
		_vec3	vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fHitMoveSpeed, 3000.f, 0.f, &m_vHitDir, fDeltaTime, m_pCalculatorCom));
		//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 3000.f, 0.f, &-m_vDir, fDeltaTime);
		m_pTransformCom->Set_PosY(m_fJumpY);
	}
	break;
	case War::War_Impack_From_Left:
	{
		_vec3	vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fHitMoveSpeed, 3000.f, 0.f, &m_vHitDir, fDeltaTime, m_pCalculatorCom));

		//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 3000.f, 0.f, &m_vRight, fDeltaTime);
		m_pTransformCom->Set_PosY(m_fJumpY);
		break;
	}
	case War::War_Impack_From_Right:
	{
		_vec3	vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fHitMoveSpeed, 3000.f, 0.f, &m_vHitDir, fDeltaTime, m_pCalculatorCom));

		//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 3000.f, 0.f, &-m_vRight, fDeltaTime);
		m_pTransformCom->Set_PosY(m_fJumpY);
		break;
	}
	case War::War_Skill_01:
		AtkColActive(dAttackCheckFrame - 0.4, dAttackCheckFrame-0.1 );
		if (!m_bNexAni)
		{
			m_bAttackMoveEnd = true;
			m_bNexAni = true;
			m_fAddPower = 0.f;
		}
		//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_ACC, &m_fAttackMoveSpeed, 10000.f, 4000.f, &m_vDir, fDeltaTime);	
		if (m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_ACC, &m_fAttackMoveSpeed, 6000.f, 2000.f, &m_vDir, fDeltaTime))
		{
			m_pTransformCom->Set_PosY(m_fJumpY);
		}
		if (!m_pMeshCom->Is_AnimationsetFinish())
		{
			auto iter_find = find_if(m_mapColider.begin(), m_mapColider.end(), CTag_Finder(L"Col_Body"));
			iter_find->second->SetCol(false);
		}
		else
		{
			auto iter_find = find_if(m_mapColider.begin(), m_mapColider.end(), CTag_Finder(L"Col_Body"));
			iter_find->second->SetCol(true);
			
		}
		break;
	case War::War_Skill_02_Start:
	{
		_vec3	vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_ACC, &m_fAttackMoveSpeed, 10000.f, 6000.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

		//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_ACC, &m_fAttackMoveSpeed, 10000.f, 6000.f, &m_vDir, fDeltaTime);
		m_pTransformCom->Set_PosY(m_fJumpY);
	}
		break;
	case War::War_Skill_02_Strike:
	{
		_vec3	vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 30000.f, 300.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

		//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 30000.f, 300.f, &m_vDir, fDeltaTime);
		m_pTransformCom->Set_PosY(m_fJumpY);
	}
		break;
	case War::War_Skill_02_Run:
	{
		if (m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_ACC, &m_fAttackMoveSpeed, 10000.f, 6000.f, &m_vDir, fDeltaTime))
			m_bSkill2End = true;
		m_pTransformCom->Set_PosY(m_fJumpY);
	}
		break;
	case War::War_Skill_03:
		break;
	case War::War_Glide_Start:
	case War::War_Glide:

		if (!m_bJumpEnd)
		{
			_float fGlidePower = m_fJumpPower*0.1f;
			m_pTransformCom->MoveStep(MOVETYPE_ACC, &fGlidePower, 120.f, 100.f, &_vec3(0.f, -1.f, 0.f), fDeltaTime);
			m_pTransformCom->Move_Pos(&m_vDir, m_fMoveSpeed*100.f, fDeltaTime);
		}
		if (m_bJumpBall)
		{
			if (m_bSound)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER);
				CSoundMgr::Get_Instance()->PlaySound(L"char_shared_glide_breath_01.ogg", CSoundMgr::CHANNEL_PLAYER);
				m_bSound = true;
			}
		}
		break;
	case War::War_Atk_Heavy_01:
		AtkColActive(dAttackCheckFrame-0.2, dAttackCheckFrame );
		if (!m_pMeshCom->Is_Animationset(dAttackCheckFrame - 0.2))
		{
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 800.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

			//m_pTransformCom->Move_Pos(&m_vDir, m_fAttackMoveSpeed*10.f, fDeltaTime);
		//	m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 800.f, 0.f, &m_vDir, fDeltaTime);
		}
		else
		{
			if (!m_bNexAni)
			{
				m_bAttackMoveEnd = true;
				m_bNexAni = true;
			}
			_vec3	vPos;
			m_pTransformCom->Get_INFO(INFO_POS, &vPos);
			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_ACC, &m_fAttackMoveSpeed, 8000.f, 1500.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_ACC, &m_fAttackMoveSpeed, 10000.f, 2100.f, &m_vDir, fDeltaTime);
		}
		m_pTransformCom->Set_PosY(m_fJumpY);
		break;
	case War::War_Atk_Heavy_02:
	{
		_vec3	vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		AtkColActive(dAttackCheckFrame, dAttackCheckFrame + 0.1);
		if (!m_pMeshCom->Is_Animationset(dAttackCheckFrame - 0.2))
		{
			//m_pTransformCom->Move_Pos(&m_vDir, m_fAttackMoveSpeed*10.f, fDeltaTime);

			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 600.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 500.f, 0.f, &m_vDir, fDeltaTime);
		}
		else
		{
			/*		if (!m_bNexAni)
					{
						m_bAttackMoveEnd = true;
						m_bNexAni = true;
					}*/

			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 900.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));

			//	m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 900.f, 0.f, &m_vDir, fDeltaTime);
		}
		m_pTransformCom->Set_PosY(m_fJumpY);
		break;
	}
	case War::War_Atk_Heavy_03:
	{
		_vec3	vPos;
		m_pTransformCom->Get_INFO(INFO_POS, &vPos);
		AtkColActive(dAttackCheckFrame - 0.2, dAttackCheckFrame+0.2);
		if (!m_pMeshCom->Is_Animationset(dAttackCheckFrame - 0.2))
		{
			//m_pTransformCom->Move_Pos(&m_vDir, m_fAttackMoveSpeed*10.f, fDeltaTime);

			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 1000.f, 0.f, &m_vDir, fDeltaTime, m_pCalculatorCom));


			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_BREAK, &m_fAttackMoveSpeed, 800.f, 0.f, &m_vDir, fDeltaTime);
		}
		else
		{
			if (!m_bNexAni)
			{
				m_bAttackMoveEnd = true;
				m_bNexAni = true;
				m_fAddPower = 0.f;
			}

			m_pTransformCom->Set_Pos(&m_pNavi->MoveStepOn_NaviMesh(&vPos, MOVETYPE::MOVETYPE_ACC, &m_fAttackMoveSpeed, 7000.f, 1700.f, &m_vDir, fDeltaTime, m_pCalculatorCom));


			//m_pTransformCom->MoveStep(MOVETYPE::MOVETYPE_ACC, &m_fAttackMoveSpeed, 7000.f, 1700.f, &m_vDir, fDeltaTime);
		}
		m_pTransformCom->Set_PosY(m_fJumpY);
		break;
	}
	case War::War_End:
		break;
	default:
		break;
	}


}
//���� �������� �ڵ����� ���� 
void CPlayer::StateLinker(_float fDeltaTime)
{
	switch (m_eCurAniState)
	{
	case War::War_Idle:
		m_eCharState = War::CHAR_IDLE;
		break;
	case War::War_Idle_Combat:
		m_eCharState = War::COMBAT;
		if (Combat_to_Idle_Timer(fDeltaTime))
		{
			m_eCharState = War::CHAR_IDLE;
			m_eMachineState = War::STATE_IDLE;
		}
		break;
	case War::War_Idle_Combat_to_Idle:
		//Į����ֱ�
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Idle;
			m_eCharState = War::CHAR_IDLE;
		}
		break;
	case War::War_Idle_to_Idle_Combat:
		//Į�̱�
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Idle_Combat;
			m_eCharState = War::COMBAT;
		}
		break;
	case War::War_Run:
	{

		m_eCharState = War::CHAR_IDLE;
		//Į�ְ� �޸���
	}
		break;
	case War::War_Run_F_Combat:
	{

		m_eCharState = War::COMBAT;
		//Į�̰� �޸���
		Combat_to_Idle_Timer(fDeltaTime);
	}
		break;
	case War::War_Jump:
		if (m_pMeshCom->Is_Animationset(0.3))
		{
			//Ű ���°� Ȯ�εǸ� 
			if (m_eKeyState == War::SPACE)
			{
				m_eCurAniState = War::War_Jump_Double;
			}
			else if (m_eKeyState == War::LBUTTON)
			{
				m_eCurAniState = War::War_Atk_Air_Light_01;
			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Jump_Fall;
		}
		break;
	case War::War_Jump_Combat:
		if (m_pMeshCom->Is_Animationset(0.3))
		{
			//Ű ���°� Ȯ�εǸ� 
			if (m_eKeyState == War::SPACE)
			{
				m_eCurAniState = War::War_Jump_Double;
			}
			else if (m_eKeyState == War::LBUTTON)
			{
				m_eCurAniState = War::War_Atk_Air_Light_01;
			}
		}
		
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Jump_Fall_Combat;
		}
		break;
	case War::War_Jump_Combat_Land:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eMachineState = War::STATE_IDLE_CB;
		}
		break;
	case War::War_Jump_Combat_Land_Run:
		if (m_pMeshCom->Is_Animationset(dJumpLandCheckFrame))
		{
			m_eMachineState = War::MOVE;
			m_eCurAniState = War::War_Run_F_Combat;
		}
		break;
	case War::War_Jump_Double:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Jump_Fall;
		}
		break;
	case War::War_Jump_Fall:
		if (m_bGlide&&!m_bGlideEnd&&!m_bGlideOn)
		{
			m_eCurAniState = War::War_Glide_Start;
		}
		//if() ���� ������ ����� 
		//Ű�Է��� ������ land run
		if (!m_bJumpEnd)
		{
			if (m_eKeyState == War::SPACE)
			{
					m_eCurAniState = War::War_Jump_Double;
			}
			else if (m_eKeyState == War::LBUTTON)
			{
				m_eCurAniState = War::War_Atk_Air_Light_01;
			}
		}
		if (m_bJumpEnd)
		{
			if (m_eKeyState == War::WASD)
			{

				m_eCurAniState = War::War_Jump_Land_Run;
				if (m_eCharState == War::COMBAT)
					m_eCurAniState = War::War_Jump_Combat_Land_Run;
			}
			else
			{
				m_eCurAniState = War::War_Jump_Land;
			}
		}
		break;
	case War::War_Jump_Fall_Combat:
		if (m_bGlide && !m_bGlideEnd && !m_bGlideOn)
		{
			if(m_ePreAniState!= War::War_Glide)
				m_eCurAniState = War::War_Glide_Start;
		}
		if (m_bJumpEnd)
		{
			if (m_eKeyState == War::WASD)
			{
				m_eCurAniState = War::War_Jump_Combat_Land_Run;
			}
			else if (m_eKeyState == War::LBUTTON)
			{
				m_eCurAniState = War::War_Atk_Air_Light_01;
			}
			else
			{
				m_eCurAniState = War::War_Jump_Combat_Land;
			}
		}
		break;
	case War::War_Jump_Land:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			if (m_eKeyState == War::WASD)
			{
				m_eMachineState = War::MOVE;
				m_eCurAniState = War::War_Run;
			}
			m_eMachineState = War::STATE_IDLE;
		}
		break;
	case War::War_Jump_Land_Heavy:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eMachineState = War::MOVE;
		}
		break;
	case War::War_Jump_Land_Run:
		if (m_pMeshCom->Is_Animationset(dJumpLandCheckFrame))
		{
			m_eMachineState = War::MOVE;
			m_eCurAniState = War::War_Run;
		}
		break;
	case War::War_Dash:
		if (m_pMeshCom->Is_Animationset(dDashCheckFrame))
		{
			switch (m_eKeyState)
			{
			case War::LBUTTON:
			case War::RBUTTON:
				m_eMachineState = War::ATTACK;
				break;
			case War::WASD:
				m_eMachineState = War::MOVE;
				break;
			case War::KEYSTATE_END:
				break;
			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eMachineState = War::STATE_IDLE_CB;
		}
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
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame-0.3))
		{
			//Ű ���°� Ȯ�εǸ� 
			if (m_eKeyState == War::LBUTTON )
			{
				m_eCurAniState = War::War_Atk_Light_02;
			}
			if ( m_eKeyState == War::RBUTTON)
			{
				m_eCurAniState = War::War_Atk_Heavy_01;
			}
			if (m_eKeyState == War::SPACE)
			{
				m_eCurAniState = War::War_Atk_Launch_B;
			}
		}

		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eMachineState = War::STATE_IDLE_CB;
		}
		break;
	case War::War_Atk_Light_02:
		if(m_pMeshCom->Is_Animationset(dAttackCheckFrame))
		{
			//Ű ���°� Ȯ�εǸ� 
			if (m_eKeyState == War::LBUTTON )
			{
				m_eCurAniState = War::War_Atk_Light_03;
			}
			else if (m_eKeyState == War::RBUTTON)
			{
				m_eCurAniState = War::War_Atk_Heavy_01;
			}
			if (m_eKeyState == War::SPACE)
			{
				m_eCurAniState = War::War_Atk_Launch_A;
			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			//m_eCurAniState = War::War_Idle_Combat;
			m_eMachineState = War::STATE_IDLE_CB;
		
		}
		break;
	case War::War_Atk_Light_03:
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame-0.1))
		{
			//Ű ���°� Ȯ�εǸ� 
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
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame - 0.1))
		{
			if (!m_bSound)
			{
				dynamic_cast<CStaticCamera*>(CGameMgr::GetInstance()->GetCamera())->CameraShake(0.3f, 0.15f);
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK2);
				CSoundMgr::Get_Instance()->PlaySound(L"char_war_land_explode.ogg", CSoundMgr::CHANNEL_PLAYER_ATK2);
				CGameObject* pEff = CGameMgr::GetInstance()->GetEffect3D(EFFECT::EFFECT3D_STONE_CIRCLE);
				dynamic_cast<CEffectMesh*>(pEff)->SetDir(m_vDir);
				_vec3 vPos;
				m_pTransformCom->Get_INFO(INFO_POS, &vPos);

				_vec3 vNorDir = m_vDir;
				D3DXVec3Normalize(&vNorDir, &vNorDir);
				pEff->SetPos(vPos + vNorDir*3.f, ID_DYNAMIC);

				m_bSound = true;
			}
		}

		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame))
		{
			//Ű ���°� Ȯ�εǸ� 
			if (m_eKeyState == War::LBUTTON)
			{

				m_eCurAniState = War::War_Atk_Light_01;

			}
			else if (m_eKeyState == War::RBUTTON)
			{
				ElementAniSet();
			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			//m_eCurAniState = War::War_Idle_Combat;
			m_eMachineState = War::STATE_IDLE_CB;	
		}
		break;
	case War::War_Atk_Air_Light_01:
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame-0.3))
		{
			//Ű ���°� Ȯ�εǸ� 
			if (m_eKeyState == War::LBUTTON)
			{
				m_eCurAniState = War::War_Atk_Air_Light_02;
			}
			else if (m_eKeyState == War::RBUTTON)
			{
				m_eCurAniState = War::War_Atk_Air_Filpsaw_Start;
			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Jump_Fall_Combat;
		}
		break;
	case War::War_Atk_Air_Light_02:
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame-0.2))
		{
			if (m_eKeyState == War::LBUTTON)
			{
				m_eCurAniState = War::War_Atk_Air_Filpsaw_Start;
			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Jump_Fall_Combat;
		}
		break;
	case War::War_Atk_Air_Filpsaw_Start:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Atk_Air_Filpsaw_Loop;
		}
		break;
	case War::War_Atk_Air_Filpsaw_Loop:
		if (m_bJumpEnd)
		{
			m_eCurAniState = War::War_Atk_Air_Filpsaw_Land;
			if (!m_bShake)
			{
				dynamic_cast<CStaticCamera*>(CGameMgr::GetInstance()->GetCamera())->CameraShake(0.4f, 0.2f);

				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
				CSoundMgr::Get_Instance()->PlaySound(L"char_war_land_explode.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
				CGameObject* pEff = CGameMgr::GetInstance()->GetEffect3D(EFFECT::EFFECT3D_STONE_CIRCLE);
				dynamic_cast<CEffectMesh*>(pEff)->SetDir(m_vDir);
				_vec3 vPos;
				m_pTransformCom->Get_INFO(INFO_POS, &vPos);
				pEff->SetPos(vPos, ID_DYNAMIC);
				m_bShake = true;

			}
		}
		
		break;
	case War::War_Atk_Air_Filpsaw_Land:
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame ))
		{
			m_bBlend = true;
			switch (m_eKeyState)
			{
			case War::LBUTTON:
				m_eCurAniState = War::War_Atk_Light_01;
				break;
			case War::RBUTTON:
				m_eCurAniState = War::War_Atk_Heavy_01;
				break;
			case War::WASD:
				m_eMachineState = War::MOVE;
				break;
			case War::KEYSTATE_END:
				break;
				
			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eMachineState = War::STATE_IDLE_CB;
		}
		m_eCharState = War::COMBAT;
		break;
	case War::War_Atk_Dash:
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame-0.1))
		{
			m_bBlend = true;
			switch (m_eKeyState)
			{
			case War::LBUTTON:
				m_eCurAniState = War::War_Atk_Light_01;
				break;
			case War::RBUTTON:
				m_eCurAniState = War::War_Atk_Heavy_01;
				break;
			case War::WASD:
				m_eMachineState = War::MOVE;
				break;
			case War::KEYSTATE_END:
				break;
			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eMachineState = War::STATE_IDLE_CB;
		}
		break;
	case War::War_Atk_Earth_Start:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Atk_Earth_Loop;
		}
		break;
	case War::War_Atk_Earth_Loop:
		dynamic_cast<CStaticCamera*>(CGameMgr::GetInstance()->GetCamera())->CameraShake(0.f, 0.1f);
		if (Key_Pressing(KEY_RBUTTON))
			m_eKeyState = War::RBUTTON;
		else if (!Key_Pressing(KEY_RBUTTON))
			m_eKeyState = War::KEYSTATE_END;

		DirSet(m_eDir, fDeltaTime, MOVEROTSPEED);

		if (m_eKeyState == War::KEYSTATE_END)
		{
			m_eCurAniState = War::War_Atk_Earth_End;
		}
		break;
	case War::War_Atk_Earth_End:
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame+0.2))
		{
			if (!m_bShake)
			{
				m_bShake = true;
				dynamic_cast<CStaticCamera*>(CGameMgr::GetInstance()->GetCamera())->CameraShake(0.25f, 0.1f);
			}
			m_bBlend = true;
			switch (m_eKeyState)
			{
			case War::LBUTTON:
			case War::RBUTTON:
				m_eMachineState = War::ATTACK;
				break;
			case War::WASD:
				m_eMachineState = War::MOVE;
				break;
			case War::KEYSTATE_END:
				break;
			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eMachineState = War::STATE_IDLE_CB;
		}
		break;
	case War::War_Atk_Flamebrand:
		DirSet(m_eDir, fDeltaTime, MOVEROTSPEED);

		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			if (Key_Up(KEY_RBUTTON))
			{
				m_eCurAniState = War::War_Atk_Flamebrand_End;
			}
		}
		break;
	case War::War_Atk_Flamebrand_Start:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Atk_Flamebrand;
		}
		break;
	case War::War_Atk_Flamebrand_End:
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame + 0.2))
		{
			m_bBlend = true;
			switch (m_eKeyState)
			{
			case War::LBUTTON:
			case War::RBUTTON:
				m_eMachineState = War::ATTACK;
				break;
			case War::WASD:
				m_eMachineState = War::MOVE;
				break;
			case War::KEYSTATE_END:
				break;
			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eMachineState = War::STATE_IDLE_CB;
		}
		break;
	case War::War_Atk_Lightning:
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame + 0.2))
		{
			m_bBlend = true;
			switch (m_eKeyState)
			{
			case War::LBUTTON:
			case War::RBUTTON:
				m_eMachineState = War::ATTACK;
				break;
			case War::WASD:
				m_eMachineState = War::MOVE;
				break;
			case War::KEYSTATE_END:
				break;
			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eMachineState = War::STATE_IDLE_CB;
		}
		break;
	case	War::War_Atk_Wind_Start:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Atk_Wind_Loop;
		}

		break;
	case	War::War_Atk_Wind_Loop:
		DirSet(m_eDir, fDeltaTime, MOVEROTSPEED);
		dynamic_cast<CStaticCamera*>(CGameMgr::GetInstance()->GetCamera())->CameraShake(0.1f, 0.05f);
		if (Key_Up(KEY_RBUTTON))
		{
			m_eCurAniState = War::War_Atk_Wind_End;
		}
		break;
	case	War::War_Atk_Wind_End:
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame-0.25))
		{
			if(!m_bShake)
				dynamic_cast<CStaticCamera*>(CGameMgr::GetInstance()->GetCamera())->CameraShake(0.3f, 0.1f);
		}
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame + 0.2))
		{
			m_bBlend = true;
			switch (m_eKeyState)
			{
			case War::LBUTTON:
			case War::RBUTTON:
				m_eMachineState = War::ATTACK;
				break;
			case War::WASD:
				m_eMachineState = War::MOVE;
				break;
			case War::KEYSTATE_END:
				break;
			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eMachineState = War::STATE_IDLE_CB;
		}
		break;
	case	War::War_Atk_LoomingDeath:
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame + 0.2))
		{
			m_bBlend = true;
			switch (m_eKeyState)
			{
			case War::LBUTTON:
			case War::RBUTTON:
				m_eMachineState = War::ATTACK;
				break;
			case War::WASD:
				m_eMachineState = War::MOVE;
				break;
			case War::KEYSTATE_END:
				break;
			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eMachineState = War::STATE_IDLE_CB;
		}
		break;
	case	War::War_Atk_Vamp_Start:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Atk_Vamp_Loop;
		}
		break;
	case	War::War_Atk_Vamp_Loop:
		DirSet(m_eDir, fDeltaTime, MOVEROTSPEED);
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame))
		{
			if (Key_Up(KEY_RBUTTON))
			{
				m_eCurAniState = War::War_Atk_Vamp_Finish;
			}
		}
		break;
	case	War::War_Atk_Vamp_Finish:
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame + 0.3))
		{
			m_bBlend = true;
			switch (m_eKeyState)
			{
			case War::LBUTTON:
			case War::RBUTTON:
				m_eMachineState = War::ATTACK;
				break;
			case War::WASD:
				m_eMachineState = War::MOVE;
				break;
			case War::KEYSTATE_END:
				break;
			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eMachineState = War::STATE_IDLE_CB;
		}
		break;
	case	War::War_Atk_Launch_A:
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame))
		{
			if (m_eKeyState == War::LBUTTON)
			{
				m_eCurAniState = War::War_Atk_Air_Light_01;
			}
			else if (m_eKeyState == War::RBUTTON)
			{
				m_eCurAniState = War::War_Atk_Air_Filpsaw_Start;
			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Jump_Fall_Combat;
		}
		break;
	case	War::War_Atk_Launch_B:
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame))
		{
			if (m_eKeyState == War::LBUTTON)
			{
				m_eCurAniState = War::War_Atk_Air_Light_01;
			}
			else if (m_eKeyState == War::RBUTTON)
			{
				m_eCurAniState = War::War_Atk_Air_Filpsaw_Start;
			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Jump_Fall_Combat;
		}
		break;
	case War::War_Chest_Open:

		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eMachineState = War::STATE_IDLE;
		}
		break;
	case War::War_Death:
		break;
	case War::War_Block_Idle:
		break;
	case War::War_Block_Impact_Heavy:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Block_Idle;
			m_eMachineState = War::BLOCK;
		}
		break;
	case War::War_Block_Impact_Light:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Block_Idle;
			m_eMachineState = War::BLOCK;
		}
		break;
	case War::War_Block_Impact_Medium:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Block_Idle;
			m_eMachineState = War::BLOCK;
		}
		break;
	case War::War_Block_Start:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Block_Idle;

		}
		break;
	case War::War_Knockback_Fall:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Knockback_Land;
		}
		break;
	case War::War_Knockback_Land:
		m_eCharState = War::COMBAT;
		if (m_pMeshCom->Is_Animationset(0.7))
		{
			m_bBlend = true;
			switch (m_eKeyState)
			{
			case War::WASD:
				m_eMachineState = War::MOVE;
				break;
			case War::KEYSTATE_END:
				break;
			}
		}
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eMachineState = War::STATE_IDLE;
		}
		break;
	case War::War_Knockback_Loop:
		if (m_bJumpEnd)
		{
			m_eCurAniState = War::War_Knockback_Land;
		}
		break;
	case War::War_Knockback_Start:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Knockback_Loop;
		}
		break;
	case War::War_Impack_From_Back:
	case War::War_Impack_From_Front:
	case War::War_Impack_From_Left:
	case War::War_Impack_From_Right:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eMachineState = War::STATE_IDLE;
		}
		break;
	case War::War_Skill_01:
		if (m_pMeshCom->Is_Animationset(0.25))
		{
			if (!m_bShake)
			{
				dynamic_cast<CStaticCamera*>(CGameMgr::GetInstance()->GetCamera())->CameraShake(0.4f, 0.2f);
				m_bShake = true;
			}
			if (!m_bSound)
			{
				m_bSound = true;
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK2);
				CSoundMgr::Get_Instance()->PlaySound(L"char_war_land_explode.ogg", CSoundMgr::CHANNEL_PLAYER_ATK2);
				CGameObject* pEff = CGameMgr::GetInstance()->GetEffect3D(EFFECT::EFFECT3D_STONE_CIRCLE);
				dynamic_cast<CEffectMesh*>(pEff)->SetDir(m_vDir);
				_vec3 vPos;
				m_pTransformCom->Get_INFO(INFO_POS, &vPos);
				pEff->SetPos(vPos, ID_DYNAMIC);
			}
		}
		if (m_pMeshCom->Is_Animationset(0.9))
		{
			m_eMachineState = War::STATE_IDLE_CB;
			m_bShake = false;
		}
		break;
	case War::War_Skill_02_Start:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Skill_02_Run;
		}
		break;
	case War::War_Skill_02_Strike:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eMachineState = War::STATE_IDLE_CB;
		}
		break;
	case War::War_Skill_02_Run:
		if (m_bSkill2End)
		{
			m_eCurAniState = War::War_Skill_02_Strike;
		}
		break;
	case War::War_Skill_03:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eMachineState = War::STATE_IDLE_CB;
		}
		break;
	case War::War_Glide:
		if (m_bGlideEnd|| !m_bGlideOn)
		{
			if (m_eCharState == War::COMBAT)
				m_eCurAniState = War::War_Jump_Fall_Combat;
			else
				m_eCurAniState = War::War_Jump_Fall;
		}
		if (m_bJumpEnd)
		{
			if (m_eKeyState == War::WASD)
			{
				if (m_eCharState == War::AIR)
					m_eCurAniState = War::War_Jump_Land_Run;
				else if (m_eCharState == War::COMBAT)
					m_eCurAniState = War::War_Jump_Combat_Land_Run;
			}
			else
			{
				m_eCurAniState = War::War_Jump_Land;
			}
		}
		break;
	case War::War_Glide_Start:
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_eCurAniState = War::War_Glide;
		}
		break;
	case War::War_Atk_Heavy_01:
		if (m_pMeshCom->Is_Animationset(0.1))
		{
			if (!m_bSound)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK1);
				CSoundMgr::Get_Instance()->PlaySound(L"char_war_attack_heavy_1.ogg", CSoundMgr::CHANNEL_PLAYER_ATK1);
				m_bSound = true;
			}
		}
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame))
		{
			//Ű ���°� Ȯ�εǸ� 
			if (m_eKeyState == War::LBUTTON)
			{
				m_eCurAniState = War::War_Atk_Heavy_02;

			}
			else if (m_eKeyState == War::RBUTTON)
			{
				if (m_bElement)
				{
					ElementAniSet();
				}
				else
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
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame-0.03))
		{
			if (!m_bShake)
			{
				dynamic_cast<CStaticCamera*>(CGameMgr::GetInstance()->GetCamera())->CameraShake(0.3f, 0.15f);

			/*	CGameObject* pEff = CGameMgr::GetInstance()->GetEffect3D(EFFECT::EFFECT3D_STONE_CIRCLE);
				dynamic_cast<CEffectMesh*>(pEff)->SetDir(m_vDir);
				_vec3 vPos;
				m_pTransformCom->Get_INFO(INFO_POS, &vPos);
				_vec3 vNorDir = m_vDir;
				D3DXVec3Normalize(&vNorDir, &vNorDir);
				pEff->SetPos(vPos + vNorDir*3.f, ID_DYNAMIC);*/
				
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNEL_PLAYER_ATK2);
				CSoundMgr::Get_Instance()->PlaySound(L"char_war_land_explode.ogg", CSoundMgr::CHANNEL_PLAYER_ATK2);
				m_bShake = true;

			}
		}

		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame))
		{
			//Ű ���°� Ȯ�εǸ� 
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
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame - 0.15))
		{
			if (!m_bShake)
			{
				dynamic_cast<CStaticCamera*>(CGameMgr::GetInstance()->GetCamera())->CameraShake(0.3f, 0.2f);

				CGameObject* pEff = CGameMgr::GetInstance()->GetEffect3D(EFFECT::EFFECT3D_STONE_CIRCLE);
				dynamic_cast<CEffectMesh*>(pEff)->SetDir(m_vDir);
				_vec3 vPos;
				m_pTransformCom->Get_INFO(INFO_POS, &vPos);
				pEff->SetPos(vPos+m_vDir*3.f , ID_DYNAMIC);

				m_bShake = true;

			}
		}
		if (m_pMeshCom->Is_Animationset(dAttackCheckFrame-0.1))
		{
			//Ű ���°� Ȯ�εǸ� 
			if (m_eKeyState == War::LBUTTON)
			{
				m_eCurAniState = War::War_Atk_Light_01;

			}
			else if (m_eKeyState == War::RBUTTON)
			{
				//�Ӽ��� �´� �Ӽ����� ����
				ElementAniSet();
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

//�������¿��� �Ϲݻ��·� ��ȯ
_bool CPlayer::Combat_to_Idle_Timer(_float fDeltaTime)
{
	m_fCToISpeed += fDeltaTime;
	if (m_fCToISpeed > m_fCToITime)
	{
		m_fCToISpeed = 0.f;
		return true;
	}
	return false;
}




void CPlayer::DirSet(War::DIR eDir, _float fTimeDelta,_float fAngleSpeed)
{
	
	_vec3 vLook;
	m_pTransformCom->Get_INFO(INFO_LOOK, &vLook);
	CGameObject* pCamera = CGameMgr::GetInstance()->GetCamera();
	if (!pCamera)
		return;
	_vec3 vRight;
	//_vec3 vRight = _vec3(1.f, 0.f, 0.f);
	_vec3 vCross;
	dynamic_cast<CTransform*>(pCamera->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_INFO(INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vRight, &vRight);
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
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-fAngleSpeed * fTimeDelta));
			}
			else if (fAngle > 90.f + fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(fAngleSpeed * fTimeDelta));
			}
		}
		else
		{
			if (fAngle <= 270.f)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(fAngleSpeed * fTimeDelta));
			}
			else if (fAngle > 270.f)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-fAngleSpeed * fTimeDelta));
			}

		}
		break;
	case War::UP_LEFT:
		if (vCross.y > 0.f)
		{
			if (fAngle < 135.f - fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-fAngleSpeed * fTimeDelta));
			}
			else if (fAngle > 135.f + fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(fAngleSpeed * fTimeDelta));
			}
		}
		else
		{
			m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(fAngleSpeed * fTimeDelta));
		}
		break;
	case War::UP_RIGHT:
		if (vCross.y > 0.f)
		{
			if (fAngle < 45.f - fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-fAngleSpeed * fTimeDelta));
			}
			else if (fAngle > 45.f + fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(fAngleSpeed * fTimeDelta));
			}
		}
		else
		{
			m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-fAngleSpeed * fTimeDelta));
		}
		break;
	case War::LEFT:
		if (vCross.y > 0.f)
		{
			if (fAngle < 180.f - fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-fAngleSpeed * fTimeDelta));
			}
		}
		else
		{
			if (fAngle > 180.f + fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(fAngleSpeed * fTimeDelta));
			}
		}
		break;
	case War::RIGHT:
		if (vCross.y > 0.f)
		{
			if (fAngle > fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(fAngleSpeed * fTimeDelta));
			}
		}
		else
		{
			if (fAngle < 360.f - fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-fAngleSpeed * fTimeDelta));
			}
		}
		break;
	case War::DOWN:
		if (vCross.y > 0.f)
		{
			if (fAngle >= 90.f)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-fAngleSpeed * fTimeDelta));
			}
			else if (fAngle < 90.f)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(fAngleSpeed * fTimeDelta));
			}
		}
		else
		{
			if (fAngle > 270.f + fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(fAngleSpeed * fTimeDelta));
			}
			else if (fAngle < 270.f - fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-fAngleSpeed * fTimeDelta));
			}

		}

		break;
	case War::DOWN_LEFT:
		if (vCross.y > 0.f)
		{
			m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-fAngleSpeed * fTimeDelta));

		}
		else
		{
			if (fAngle < 225.f - fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-fAngleSpeed * fTimeDelta));
			}
			else if (fAngle > 225.f + fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(fAngleSpeed * fTimeDelta));
			}
		}
		break;
	case War::DOWN_RIGHT:
		if (vCross.y > 0.f)
		{
			m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(fAngleSpeed * fTimeDelta));
		}
		else
		{
			if (fAngle < 315.f - fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-fAngleSpeed * fTimeDelta));
			}
			else if (fAngle > 315.f + fAngleOffset)
			{
				m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(fAngleSpeed * fTimeDelta));
			}
		}
		break;
	case War::IDLE:
		m_vDir = _vec3(0.f, 0.f, 0.f);
		break;
	case War::DIRECTION_END:
		break;
	}
}

void CPlayer::DirSet_Combo()
{
	_vec3 vRot= m_pTransformCom->Get_Rot();

	//_vec3 vLook;
	//m_pTransformCom->Get_INFO(INFO_LOOK, &vLook);
	//_vec3 vRight = _vec3(0.f, 0.f, 1.f);
	//_vec3 vCross;
	//D3DXVec3Normalize(&vRight, &vRight);
	//D3DXVec3Cross(&vCross, &vLook, &vRight);

	//D3DXVec3Normalize(&vLook, &vLook);
	//_float fCos = D3DXVec3Dot(&vLook, &vRight);
	//_float fAngle = D3DXToDegree(acosf(fCos));

	//_float fAngleOffset = 3.f;

	//if (vCross.y < 0.f)
	//{
	//	fAngle = 360.f - fAngle;
	//}

	_float fOffsetAngle = dynamic_cast<CStaticCamera*>(CGameMgr::GetInstance()->GetCamera())->GetRotY();

	//fRadius*cosf(m_fAngle) - fRadius*sinf(m_fAngle)
	switch (m_eDir)
	{
	case War::UP: 
		vRot.y = D3DXToRadian(fOffsetAngle);
		break;
	case War::UP_LEFT:
		vRot.y = D3DXToRadian(315.f + fOffsetAngle);
		break;
	case War::UP_RIGHT:
		vRot.y = D3DXToRadian(45.f + fOffsetAngle);
		break;
	case War::LEFT:
		vRot.y = D3DXToRadian(270.f + fOffsetAngle);
		break;
	case War::RIGHT:
		vRot.y = D3DXToRadian(90.f + fOffsetAngle);
		break;
	case War::DOWN:
		vRot.y = D3DXToRadian(180.f + fOffsetAngle);
		break;
	case War::DOWN_LEFT:
		vRot.y = D3DXToRadian(225.f + fOffsetAngle);
		break;
	case War::DOWN_RIGHT:
		vRot.y = D3DXToRadian(135.f + fOffsetAngle);
		break;
	case War::IDLE:
		break;
	case War::DIRECTION_END:
		break;
	default:
		break;
	}
	m_pTransformCom->Set_Rot(&vRot);
}

void CPlayer::ElementAniSet()
{
	switch (m_eElement)
	{
	case War::ELEMENT_EARTH:
		m_eCurAniState = War::War_Atk_Earth_Start;
		break;
	case War::ELEMENT_FLAME:
		m_eCurAniState = War::War_Atk_Flamebrand_Start;
		break;
	case War::ELEMENT_LIGHTNING:
		m_eCurAniState = War::War_Atk_Lightning;

		break;
	case War::ELEMENT_VAMP:
		m_eCurAniState = War::War_Atk_Vamp_Start;

		break;
	case War::ELEMENT_WIND:
		m_eCurAniState = War::War_Atk_Wind_Start;

		break;
	case War::ELEMENT_DEATH:
		m_eCurAniState = War::War_Atk_LoomingDeath;

		break;
	case War::ELEMENT_END:
		break;
	}
}


HRESULT CPlayer::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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

void CPlayer::TakeDmg(_float Atk)
{
	m_tCharInfo.fDmg = Atk;
	m_tCharInfo.fHp -= Atk;
	m_eMachineState = War::HIT;
}

void CPlayer::AtkColActive(double dStart, double dEnd)
{
	auto iter_find = find_if(m_mapColider.begin(), m_mapColider.end(), CTag_Finder(L"Col_Weapon0"));
	if (iter_find == m_mapColider.end())
		return;

	if (m_pMeshCom->Is_Animationset(dEnd))
	{
		iter_find->second->SetActive(false);
	}
	else if(m_pMeshCom->Is_Animationset(dStart))
	{
		SpawnTrail();
		iter_find->second->SetActive(true);
	}
}

void CPlayer::AtkColLoop()
{
	auto iter_find = find_if(m_mapColider.begin(), m_mapColider.end(), CTag_Finder(L"Col_Weapon0"));
	if (iter_find == m_mapColider.end())
		return;
	SpawnTrail();
	iter_find->second->SetActive(true);
}

_bool CPlayer::GlideEndTimer(_float fTimeDelta)
{
	m_fGlideEndSpeed += fTimeDelta;
	if (m_fGlideEndSpeed > m_fGlideEndTime)
		return true;
	return false;
}

_bool CPlayer::GlideTimer(_float fTimeDelta)
{
	
	m_fGlideSpeed += fTimeDelta;

	if (m_fGlideSpeed > m_fGlideTime)
 		return true;

	return false;
}

void CPlayer::InteractionTimer(_float fDeltaTime)
{
	m_fInteractionSpeed += fDeltaTime;

	if (m_fInteractionSpeed > m_fInteractionTime)
	{
		m_bInteraction = false;
		return;
	}
	m_bInteraction = true;
}

void CPlayer::SpawnTrail()
{
	const _matrix *BoneMat;
	const D3DXFRAME_DERIVED*		pFrame = m_pMeshCom->Get_FrameByName("Bone_War_Weapon_Sword");
	BoneMat = &pFrame->CombinedTransformMatrix;
	CGameObject* pTrail = Engine::Get_GameObject(L"Environment", L"Trail");
	if (pTrail)
	{
		dynamic_cast<CEffect_Trail*>(pTrail)->Set_MatrixInfo(m_pTransformCom->Get_WorldMatrix(), BoneMat);
		pTrail->SetOption(&m_tOutTrail);
	}

}

