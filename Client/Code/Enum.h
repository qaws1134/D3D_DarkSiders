#ifndef ClientEnum_h__
#define ClientEnum_h__

namespace WHEEL
{
	enum MOVE {
		MOVE_UP,
		MOVE_POS,
		MOVE_DOWN,
		MOVE_END
	};
}


namespace UI
{

	enum ACTIVE_ITEM
	{
		ACTIVE_AIR_SPIN,		//에어 스핀 가능
		ACTIVE_LIGHTATTACK1UP,
		ACTIVE_LIGHTATTACK2UP,
		ACTIVE_HEAVYATTACK1UP,	//강공격1 이펙트 강화
		ACTIVE_HEAVYATTACK2UP,	//강공격2 이펙트 강화
		ACTIVE_DASHATTACK,		//대쉬공격
		ACTIVE_REFLECT,		//반사
		ACTIVE_REFLECTUP,		//반사 이펙트 강화
		ACTIVE_AIR_DASH,		//공중 내려찍기
		ACTIVE_COMBO_ELEMENT,	//기본공격 끝나고 오른쪽 버튼-> 속성공격
		ACTIVE_ITEM_END
	};
	enum STONE_ITEM
	{
		ITEM_BOX1,
		ITEM_BOX2,
		ITEM_BOX3,
		ITEM_GRINNER,
		ITEM_SKULLMAGE,
		ITEM_BROODI,
		ITEM_BAT,
		ITEM_END
	}


	enum TYPE
	{
		TYPE_DEFAULT,
		TYPE_SELECT_BOX,
		TYPE_FONT
	};
	enum SHADE
	{
		SHADE_DEAFAULT,
		SHADE_UVMOVE,
		SHADE_END
	};

	enum LISTKEY 
	{
		LISTKEY_WEAPONELEMENT,
		LISTKEY_CORETREE,
		LISTKEY_CORELIST,
		LISTKEY_PLAYERINFO,
		LISTKEY_STONE,
		LISTKEY_END

	};
	enum  ELEMENT{
		ELEMENT_00,
		ELEMENT_01,
		ELEMENT_02,
		ELEMENT_03,
		ELEMENT_04,
		ELEMENT_05,
		ELEMENT_END
	};
	enum STONE_ELEMENT
	{
		STONE_ATK,
		STONE_SKILL,
		STONE_HEALTH,
		STONE_ALL,
		STONE_ELEMENT_END
	};
	enum STONE
	{
		GOBLERIN,
		STATUE,
		KNIGHT,
		ICEMAGE,
		SKULL,
		CRIBO,
		LARVA,
		ANT,
		BLUEANGEL,
		GREMLIN1,
		GREMLIN2,
		GREENGOBLE,
		GRINNER,
		SKULLMAGE,
		BROODI,
		BAT,
		LEVIATHAN,
		NINJA,
		WATERBOSS,
		GRINNER2,
		CRIBO2,
		MAE,
		STONE_END
	};
	
}



namespace War {

	//버튼 방향값 입력 상태 
	enum DIR { 
		UP,
		UP_LEFT,
		UP_RIGHT, 
		LEFT, 
		RIGHT, 
		DOWN,
		DOWN_LEFT,
		DOWN_RIGHT, 
		IDLE, 
		DIRECTION_END 
	};

	//콤보를 위한 버튼 입력 상태 
	enum KeyState
	{
		LBUTTON,
		RBUTTON,
		SPACE,
		WASD,
		NUM1,
		NUM2,
		NUM3,
		KEYSTATE_END
	};

	//캐릭터 상태 
	enum CHARSTATE
	{
		CHAR_IDLE,
		COMBAT,
		AIR,
		AIR_COMBAT,
		CHAR_END
	};

	//전체적 스테이트 머신
	enum STATE
	{
		STATE_IDLE,
		STATE_IDLE_CB,
		MOVE,
		ATTACK,
		BLOCK,
		JUMP,
		JUMP_CB,
		JUMPATTACK,
		DASH,
		HIT,
		STATE_END
	};

	enum WEAPON_ELEMENT
	{
		ELEMENT_EARTH,
		ELEMENT_FLAME,
		ELEMENT_LIGHTNING,
		ELEMENT_DEATH,
		ELEMENT_WIND,
		ELEMENT_VAMP,
		ELEMENT_END

	};

	enum Ani {
		War_Idle,                                 //0	frame:[161]
		War_Idle_Combat,                          //1	frame:[91]
		War_Idle_Combat_to_Idle,                  //2	frame:[46]
		War_Idle_to_Idle_Combat,                  //3	frame:[46]
		War_Run,                                  //4	frame:[27]
		War_Run_F_Combat,                         //5	frame:[27]
		War_Jump,                                 //6	frame:[26]
		War_Jump_Combat,                          //7	frame:[26]
		War_Jump_Combat_Land,                     //8	frame:[26]
		War_Jump_Combat_Land_Run,                 //9	frame:[19]
		War_Jump_Double,                          //10	frame:[28]
		War_Jump_Fall,                            //11	frame:[2]
		War_Jump_Fall_Combat,                     //12	frame:[2]
		War_Jump_Land,                            //13	frame:[25]
		War_Jump_Land_Heavy,                      //14	frame:[49]
		War_Jump_Land_Run,                        //15	frame:[18]
		War_Dash,                                 //16	frame:[54]
		War_Dash_Air_Land,                        //17	frame:[42]
		War_Dash_Air_Loop,                        //18	frame:[61]
		War_Dash_Air_Start,                       //19	frame:[10]
		War_DashTo_Back,                          //20	frame:[35]
		War_Atk_Light_01,                         //21	frame:[64]
		War_Atk_Light_02,                         //22	frame:[44]
		War_Atk_Light_03,                         //23	frame:[66]
		War_Atk_Light_04,                         //24	frame:[74]
		War_Atk_Air_Filpsaw_Start,                //25	frame:[8]
		War_Atk_Air_Filpsaw_Loop,                 //26	frame:[6]
		War_Atk_Air_Filpsaw_Land,                 //27	frame:[57]
		War_Atk_Dash,                             //28	frame:[54]
		War_Atk_Earth_Start,                      //29	frame:[31]
		War_Atk_Earth_Loop,                       //30	frame:[66]
		War_Atk_Earth_End,                        //31	frame:[72]
		War_Atk_Flamebrand,                       //32	frame:[28]
		War_Atk_Flamebrand_Start,                 //33	frame:[9]
		War_Atk_Flamebrand_End,                   //34	frame:[49]
		War_Atk_Lightning,                        //35	frame:[65]
		War_Atk_Wind_Start,                       //36	frame:[31]
		War_Atk_Wind_Loop,                        //37	frame:[66]
		War_Atk_Wind_End,                         //38	frame:[71]
		War_Atk_LoomingDeath,                     //36	frame:[99]
		War_Atk_Vamp_Start,                       //37	frame:[19]
		War_Atk_Vamp_Loop,                        //38	frame:[8]
		War_Atk_Vamp_Finish,                      //39	frame:[48]
		War_Atk_Launch_A,                         //40	frame:[33]
		War_Atk_Launch_B,                         //41	frame:[33]
		War_Atk_Air_Light_01,                     //42	frame:[51]
		War_Atk_Air_Light_02,                     //43	frame:[51]
		War_Chest_Open,                           //44	frame:[66]
		War_Death,                                //45	frame:[101]
		War_Block_Idle,                           //46	frame:[91]
		War_Block_Impact_Heavy,                   //47	frame:[17]
		War_Block_Impact_Light,                   //48	frame:[17]
		War_Block_Impact_Medium,                  //49	frame:[17]
		War_Block_Start,                          //50	frame:[10]
		War_Knockback_Fall,                       //51	frame:[41]
		War_Knockback_Land,                       //52	frame:[53]
		War_Knockback_Loop,                       //53	frame:[41]
		War_Knockback_Start,                      //54	frame:[21]
		War_Impack_From_Back,                     //55	frame:[31]
		War_Impack_From_Front,                    //56	frame:[21]
		War_Impack_From_Left,                     //57	frame:[21]
		War_Impack_From_Right,                    //58	frame:[21]
		War_Skill_01,                             //59	frame:[72]
		War_Skill_02_Start,                       //60	frame:[11]
		War_Skill_02_Strike,                      //61	frame:[39]
		War_Skill_02_Run,                         //62	frame:[33]
		War_Skill_03,                             //63	frame:[56]
		War_Glide,                                //64	frame:[61]
		War_Glide_Start,                          //65	frame:[21]
		War_Atk_Heavy_01,                         //66	frame:[53]
		War_Atk_Heavy_02,                         //67	frame:[59]
		War_Atk_Heavy_03,                         //68	frame:[88]
		War_End
	};
}
namespace WaterBoss {

	enum DIR 
	{
		//플레이어 방향에 따른 각도 지정 
		

	};

	//패턴상태 
	enum STATE
	{
		STATE_IDLE,
		STATE_CALL_LIGHTNING,
		STATE_ORB,
		STATE_SLAM,
		STATE_IMPACT,
		STATE_WAVE,
		STATE_END
	};


	enum  Ani {
		Atk_CallLightning_Start,                  //0	frame:[66]
		Atk_CallLightning,                        //1	frame:[60]
		Atk_SummonOrb,                            //2	frame:[84]
		Atk_SummonOrb_L,                          //3	frame:[84]
		Atk_Tentade_Pummel,                       //4	frame:[131]
		Atk_TentadeSlam_FL,                       //5	frame:[121]
		Atk_TentadeSlam_FL_02,                    //6	frame:[121]
		Atk_TentadeSlam_FR,                       //7	frame:[121]
		Atk_TentadeSlam_L,                        //8	frame:[121]
		Atk_TentadeSlam_L_02,                     //9	frame:[121]
		Atk_TentadeSlam_R,                        //10	frame:[121]
		Atk_TentadeSlam_R_02,                     //11	frame:[121]
		Atk_WhirlPool,                            //12	frame:[96]
		Impact_Stun,                              //13	frame:[131]
		Impact_Stun_Loop,                         //14	frame:[441]
		TidalWave,                                //15	frame:[106]
		TidalWave_Impact,                         //16	frame:[201]
		TidalWave_Loop,                           //17	frame:[121]
		Idle,                                     //18	frame:[241]
		End
	};
}







#endif
