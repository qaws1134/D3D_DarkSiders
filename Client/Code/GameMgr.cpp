#include "stdafx.h"
#include "GameMgr.h"

#include "Export_Function.h"


IMPLEMENT_SINGLETON(CGameMgr)

CGameMgr::CGameMgr()
{
	m_vecStone.reserve(22);
}

CGameMgr::~CGameMgr(void)
{

}


void CGameMgr::TakeStone(UI::STONE eStone)
{
	STONE tStone;

	//�ð������� 
	//Ư��ȿ��  ����, �ٴڿ� ���ӵ� ��� ����, ũ��Ƽ��Ȯ��, �̵��ӵ� �߰�  
	switch (eStone)
	{
	case UI::GOBLERIN:
		tStone = { eStone, L"���",L"��ų �� +1 ",UI::STONE_SKILL,false };
		break;
	case UI::STATUE:
		tStone = { eStone,L"����",L"��ų �� +1",UI::STONE_SKILL,true };
		break;
	case UI::KNIGHT:
		tStone = { eStone, L"���",L"��ų �� +1",UI::STONE_SKILL,false };
		break;
	case UI::ICEMAGE:
		tStone = { eStone,L"���������",L"��ų �� +1",UI::STONE_SKILL,false };
		break;
	case UI::SKULL:
		tStone = { eStone, L"����",L"��ų �� +1",UI::STONE_SKILL,false };
		break;
	case UI::CRIBO:
		tStone = { eStone,L"ũ����",L"��ų �� +1",UI::STONE_SKILL,true };
		break;
	case UI::LARVA:
		tStone = { eStone, L"�ֹ���",L"��ų �� +1",UI::STONE_SKILL,false };
		break;
	case UI::ANT:
		tStone = { eStone,L"�Ұ���",L"���� �� +1",UI::STONE_ATK,false };
		break;
	case UI::BLUEANGEL:
		tStone = { eStone,L"õ��",L"���� �� +1",UI::STONE_ATK,true };
		break;
	case UI::GREMLIN1:
		tStone = { eStone,L"�׷���",L"���� �� +1",UI::STONE_ATK,false };
		break;
	case UI::GREMLIN2:
		tStone = { eStone,L"��ȭ �׷���",L"���� �� + 5",UI::STONE_ATK,false };
		break;
	case UI::GREENGOBLE:
		tStone = { eStone,L"�ʷ� ���",L"���� �� + 5",UI::STONE_ATK,false };
		break;
	case UI::SKULLMAGE:
		tStone = { eStone, L"�ذ� ������",L"���� �� + 5",UI::STONE_ATK,false };
		break;
	case UI::GRINNER:
		tStone = { eStone, L"�׸���",L"���� �� + 5",UI::STONE_ATK,true };
		break;
	case UI::BROODI:
		tStone = { eStone,L"����",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	case UI::BAT:
		tStone = { eStone,L"����",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	case UI::LEVIATHAN:
		tStone = { eStone,L"�����ź",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	case UI::NINJA:
		tStone = { eStone,L"����",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	case UI::WATERBOSS:
		tStone = { eStone, L"�ٵ�",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	case UI::GRINNER2:
		tStone = { eStone,L"���� ��",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	case UI::CRIBO2:
		tStone = { eStone, L"���� ũ����",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	case UI::MAE:
		tStone = { eStone,L"���� ����",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	default:
		break;
	}
	m_vecStone.emplace_back(tStone);
	
}

STONE CGameMgr::GetStone(UI::STONE eStone)
{
	STONE tStone;
	switch (eStone)
	{
	case UI::GOBLERIN:
		tStone = { eStone, L"���",L"��ų �� +1 ",UI::STONE_SKILL,false };
		break;
	case UI::STATUE:
		tStone = { eStone,L"����",L"��ų �� +1",UI::STONE_SKILL,true };
		break;
	case UI::KNIGHT:
		tStone = { eStone, L"���",L"��ų �� +1",UI::STONE_SKILL,false };
		break;
	case UI::ICEMAGE:
		tStone = { eStone,L"���������",L"��ų �� +1",UI::STONE_SKILL,false };
		break;
	case UI::SKULL:
		tStone = { eStone, L"����",L"��ų �� +1",UI::STONE_SKILL,false };
		break;
	case UI::CRIBO:
		tStone = { eStone,L"ũ����",L"��ų �� +1",UI::STONE_SKILL,true };
		break;
	case UI::LARVA:
		tStone = { eStone, L"�ֹ���",L"��ų �� +1",UI::STONE_SKILL,false };
		break;
	case UI::ANT:
		tStone = { eStone,L"�Ұ���",L"���� �� +1",UI::STONE_ATK,false };
		break;
	case UI::BLUEANGEL:
		tStone = { eStone,L"õ��",L"���� �� +1",UI::STONE_ATK,true };
		break;
	case UI::GREMLIN1:
		tStone = { eStone,L"�׷���",L"���� �� +1",UI::STONE_ATK,false };
		break;
	case UI::GREMLIN2:
		tStone = { eStone,L"��ȭ �׷���",L"���� �� + 5",UI::STONE_ATK,false };
		break;
	case UI::GREENGOBLE:
		tStone = { eStone,L"�ʷ� ���",L"���� �� + 5",UI::STONE_ATK,false };
		break;
	case UI::SKULLMAGE:
		tStone = { eStone, L"�ذ� ������",L"���� �� + 5",UI::STONE_ATK,false };
		break;
	case UI::GRINNER:
		tStone = { eStone, L"�׸���",L"���� �� + 5",UI::STONE_ATK,true };
		break;
	case UI::BROODI:
		tStone = { eStone,L"����",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	case UI::BAT:
		tStone = { eStone,L"����",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	case UI::LEVIATHAN:
		tStone = { eStone,L"�����ź",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	case UI::NINJA:
		tStone = { eStone,L"����",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	case UI::WATERBOSS:
		tStone = { eStone, L"�ٵ�",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	case UI::GRINNER2:
		tStone = { eStone,L"���� ��",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	case UI::CRIBO2:
		tStone = { eStone, L"���� ũ����",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	case UI::MAE:
		tStone = { eStone,L"���� ����",L"ü�� +1",UI::STONE_HEALTH,false };
		break;
	default:
		break;
	}
	return tStone;
}


void CGameMgr::Free(void)
{
}


