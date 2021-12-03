#ifndef ClientEnum_h__
#define ClientEnum_h__


namespace PARTICLEEFF
{
	enum TYPE
	{
		PARTICLE_LIGHTNING,
		PARTICLE_END
	};
}


namespace DROPITEM
{
	enum TYPE
	{
		ITEM_STONE,
		ITEM_SOUL,
		ITEM_HEALTH,
		ITEM_END
	};
}

namespace EFFECTSET
{
	enum TYPE
	{
		BOSS_CALL_LIGHTNING_START,
		BOSS_CALL_LIGHTNING_MID,
		BOSS_CALL_LIGHTNING_END,

		EFFECTSET_END
	};
}


namespace EFFECT
{
	enum TYPE3D
	{
		EFFECT3D_STONECLUSTER,
		EFFECT3D_STONE1,
		EFFECT3D_STONE2,
		EFFECT3D_STONE_CIRCLE,
		EFFECT3D_CHAGE,
		EFFECT3D_CHAGE_CIRCLE,
		EFFECT3D_END
	};

	enum SHADER
	{
		SHADER_BLEND,
		SHADER_TEST,
		SHADER_BLACKOUT,
		SHADER_BLACKOUT_LIGHTNING,
		SHADER_END,
	};
	enum TYPE2D
	{
		EFFECT_LIGHTNING_CLUSTER,
		EFFECT_LIGHTNING_WEAPON_CLUSTER,
		EFFECT_WATER_CLUSTER,
		EFFECT_LIGHTNING1_1x4,
		EFFECT_LIGHTNING2_1x4,
		EFFECT_LIGHTNING3_1x4,
		EFFECT_LIGHTNING4_1x4,
		EFFECT_LIGHTNING1_PLAYER_1x4,
		EFFECT_LIGHTNING2_PLAYER_1x4,
		EFFECT_LIGHTNING3_PLAYER_1x4,
		EFFECT_LIGHTNING4_PLAYER_1x4,
		EFFECT_WATERBOOM1_2x2,
		EFFECT_WATERBOOM2_2x2,
		EFFECT_WATERBOOM3_2x2,
		EFFECT_WATERBOOM4_2x2,
		EFFECT_WATERBOOM5_2x2,
		EFFECT_WATERBOOM6_2x2,
		EFFECT_FOG1_2x2,
		EFFECT_FOG2_2x2,
		EFFECT_FOG3_2x2,
		EFFECT_ACT1_2x2,
		EFFECT_ACT2_2x2,
		EFFECT_ACT3_2x2,
		EFFECT_STONE_4x4,
		EFFECT_FLAME1_6x6,
		EFFECT_FLAME2_6x6,
		EFFECT_FLAME3_6x6,
		EFFECT_MAGIC_CIRCLE_SINGLE,
		EFFECT_EXPLOSION,
		EFFECT_END
	};

}

namespace BULLET{
	enum TYPE
	{
		BULLET_CALLLIGHTNING,
		BULLET_ORBLIGHTNING,
		BULLET_TSUNAMI,
		BULLET_GRINNERBARFIN,
		BULLET_GOBLINSPEAR,
		BULLET_SPAWNFADE_PLAYER,
		BULLET_END
	};


	enum ID
	{
		BULLET_PLAYER,
		BULLET_ENEMY,
		ID_END
	};

}


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

	enum KEYTEXTURE
	{
		KEYTEXTURE_LBUTTON,
		KEYTEXTURE_RBUTTON,
		KEYTEXTURE_MBUTTON,
		KEYTEXTURE_SHIFT,
		KEYTEXTURE_SPACE,

		KEYTEXTURE_END
	};

	enum ITEM
	{
		ACTIVE_AIR_SPIN,		//에어 스핀 가능
		ACTIVE_LIGHTATTACK1UP,  //약공격1 이펙트강화
		ACTIVE_LIGHTATTACK2UP, //약공격2 이펙트강화
		ACTIVE_HEAVYATTACK1UP,	//강공격1 이펙트 강화
		ACTIVE_HEAVYATTACK2UP,	//강공격2 이펙트 강화
		ACTIVE_DASHATTACK,		//대쉬공격
		ACTIVE_REFLECT,		//반사
		ACTIVE_REFLECTUP,		//반사 이펙트 강화
		ACTIVE_AIR_DASH,		//공중 내려찍기
		ACTIVE_COMBO_ELEMENT,	//기본공격 끝나고 오른쪽 버튼-> 속성공격
		ACTIVE_ITEM_END,

		ITEM_BOX1,
		ITEM_BOX2,
		ITEM_BOX3,
		ITEM_GRINNER,
		ITEM_SKULLMAGE,
		ITEM_BROODI,
		ITEM_BAT,
		ITEM_END
	};


	enum TYPE
	{
		TYPE_DEFAULT,
		TYPE_SELECT_BOX,
		TYPE_FONT
	};
	enum SHADE
	{
		SHADE_DEAFAULT,
		SHADE_UVMOVE_STONE,
		SHADE_UVMOVE_STORE,
		SHADE_BLEND,
		SHADE_STONE,
		SHADE_CORETREE_ELEMENT,
		SHADE_SELFILL,
		SHADE_END
	};

	enum LISTKEY 
	{
		LISTKEY_WEAPONELEMENT,
		
		LISTKEY_PLAYERINFO,

		LISTKEY_CORETREE,
		LISTKEY_CORELIST,
		LISTKEY_STONE,

		LISTKEY_TOASTMSG,

		LISTKEY_STOREBASE,
		LISTKEY_STOREACTIVE,
		LISTKEY_STORESTONE,

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

namespace Angel
{

	enum STATE
	{
		STATE_SPAWN_IDLE,
		STATE_SPAWN,
		STATE_IDLE,
		STATE_ATK,
		STATE_TURN,
		STATE_MOVE,
		STATE_CHASE,
		STATE_HIT,
		STATE_HITEND,
		STATE_DEAD,
		STATE_END
	};
	enum Ani {
		Angel_Atk_Combo,                          //0	frame:[147]
		Angel_Atk_Dash,                           //1	frame:[63]
		Angel_Atk_Dragon_Start,                   //2	frame:[36]
		Angel_Atk_Dragon_Hold,                    //3	frame:[41]
		Angel_Atk_Dragon_End,                     //4	frame:[81]
		Angel_Atk_Swipe_L,                        //5	frame:[56]
		Angel_Atk_Swipe_R,                        //6	frame:[49]
		Angel_Fly_F_Start,                        //7	frame:[26]
		Angel_Fly_F,                              //8	frame:[51]
		Angel_Fly_Idle,                           //9	frame:[26]
		Angel_Death_War,                          //10	frame:[99]
		Angel_DeathPose_War,                      //11	frame:[91]
		Angel_Impact_F,                           //12	frame:[36]
		Angel_Idle,                               //13	frame:[41]
		Angel_Impact_L,                           //14	frame:[36]
		Angel_Impact_R,                           //15	frame:[36]
		Angel_Impact_Flinch_F,                    //16	frame:[36]
		Angel_Impact_Flinch_L,                    //17	frame:[36]
		Angel_Impact_Flinch_R,                    //18	frame:[36]
		Angel_Knock_B_Start,                      //19	frame:[6]
		Angel_Knock_B_Idle,                       //20	frame:[121]
		Angel_Knock_B_Land,                       //21	frame:[33]
		Angel_Knock_B_Fall,                       //22	frame:[29]
		Angel_Knock_B_Apex,                       //23	frame:[26]
		Angel_Run_F,                              //24	frame:[31]
		Angel_Turn_90_L,                          //25	frame:[31]
		Angel_Turn_90_R,                          //26	frame:[31]
		Angel_Turn_180_L,                         //27	frame:[41]
		Angel_Turn_180_R,                         //28	frame:[41]
		Angel_Walk_B,                             //29	frame:[41]
		End
	};
}

namespace Serpent
{
	enum STATE
	{
		STATE_OPEN,
		STATE_CLEOSE,
		STATE_END
	};


	enum Ani {
		Serpent_Open,                             //0	frame:[65]
		Serpent_Open_Idle,                        //1	frame:[2]
		Serpent_Close,                            //2	frame:[65]
		Serpent_Close_Idle,                       //3	frame:[2]
		End
	};
}

namespace Vulgrim
{
	enum STATE
	{
		STATE_IDLE,	
		STATE_BUY,
		STATE_END
	};

	enum Ani{
		Vulgrim_Idle,                             //0	frame:[101]
		Vulgrim_Buy_Item_02,                      //1	frame:[121]
		End
	};
}

namespace Dis
{
	enum STATE
	{
		STATE_IDLE,
		STATE_ACTIVE,
		STATE_SPAWN,
		STATE_SPAWN_IDLE,
		STATE_BUY,
		STATE_END
	};
	enum Ani{
		Dis_Idle,                                 //0	frame:[91]
		Dis_Active_Idle,                          //1	frame:[91]
		Dis_Active_to_Idle,                       //2	frame:[61]
		Dis_Idle_to_Active,                       //3	frame:[61]
		Dis_Spawn,                                //4	frame:[121]
		Dis_Active_Emote_Purchase,                //5	frame:[91]
		End
	};
}

namespace Player_Barrier
{
	enum STATE
	{
		STATE_IDLE_OPEN,
		STATE_IDLE_CLOSE,
		STATE_OPEN,
		STATE_CLEOSE,
		STATE_END
	};


	enum Ani {
		PlayerBarrer_Close,                       //0	frame:[40]
		PlayerBarrer_Open,                        //1	frame:[55]
		End
	};
}



namespace Chest
{
	enum STATE
	{
		STATE_IDLE,
		STATE_OPEN,
		STATE_OPEND,
		STATE_HIT,
		STATE_DEAD,
		STATE_END
	};

	enum Ani {
		Chest_Idle,                               //0	frame:[101]
		Chest_Start,                              //1	frame:[101]
		Chest_Opened,                             //2	frame:[8]
		Chest_Open,                               //3	frame:[74]
		Chest_Impact,                             //4	frame:[26]
		Chest_Closed,                             //5	frame:[6]
		End
	};
}

namespace Grinner
{
	enum SPAWN
	{
		SPAWN_POTRAL,
		SPAWN_APEX,
		SPAWN_END
	};

	//패턴상태 
	enum STATE
	{
		STATE_SPAWN_IDLE,
		STATE_SPAWN,
		STATE_IDLE,
		STATE_ATK,
		STATE_TURN,
		STATE_MOVE,
		STATE_CHASE,
		STATE_HIT,
		STATE_HITEND,
		STATE_DEAD,
		STATE_END
	};
	enum Ani {
		Grinner_Atk_Flip,                         //0	frame:[73]
		Grinner_Atk_BarfinRainbows,               //1	frame:[131]
		Grinner_Atk_Lunge,                        //2	frame:[49]
		Grinner_Atk_Swipe_Combo,                  //3	frame:[81]
		Grinner_DeadPose,                         //4	frame:[2]
		Grinner_Death,                            //5	frame:[66]
		Grinner_Death_War,                        //6	frame:[163]
		Grinner_DeathPose_War,                    //7	frame:[173]
		Grinner_Idle,                             //8	frame:[81]
		Grinner_Impact_F,                         //9	frame:[36]
		Grinner_Impact_Flinch_B,                  //10	frame:[31]
		Grinner_Impact_Flinch_F,                  //11	frame:[31]
		Grinner_Impact_Flinch_L,                  //12	frame:[31]
		Grinner_Impact_Flinch_R,                  //13	frame:[31]
		Grinner_Impact_L,                         //14	frame:[36]
		Grinner_Impact_R,                         //15	frame:[36]
		Grinner_Jump_Apex,                        //16	frame:[26]
		Grinner_Jump_Fall,                        //17	frame:[26]
		Grinner_Jump_Land,                        //18	frame:[21]
		Grinner_Jump_Launch,                      //19	frame:[21]
		Grinner_Knock_B_Start,                    //20	frame:[6]
		Grinner_Knock_B_Apex,                     //21	frame:[26]
		Grinner_Knock_B_Fall,                     //22	frame:[29]
		Grinner_Knock_B_Land,                     //23	frame:[33]
		Grinner_Knock_B_Recover,                  //24	frame:[51]
		Grinner_Knock_B_Idle,                     //25	frame:[121]
		Grinner_PotalSpawn,                       //26	frame:[83]
		Grinner_Run_F,                            //27	frame:[61]
		Grinner_Run_FL,                           //28	frame:[61]
		Grinner_Run_FR,                           //29	frame:[61]
		Grinner_Turn_90_L,                        //30	frame:[26]
		Grinner_Turn_90_R,                        //31	frame:[26]
		Grinner_Turn_180_L,                       //32	frame:[31]
		Grinner_Turn_180_R,                       //33	frame:[31]
		Grinner_Walk_B,                           //34	frame:[41]
		Grinner_Walk_BL,                          //35	frame:[41]
		Grinner_Walk_BR,                          //36	frame:[41]
		Grinner_Walk_F,                           //37	frame:[41]
		Grinner_Walk_L,                           //38	frame:[41]
		Grinner_Walk_R,                           //39	frame:[41]
		End
	};
}

namespace Goblin {

	enum SPAWN
	{
		SPAWN_POTRAL,
		SPAWN_APEX,
		SPAWN_SIT,
		SPAWN_END
	};

	//패턴상태 
	enum STATE
	{
		STATE_SPAWN_IDLE,
		STATE_SPAWN,
		STATE_IDLE,
		STATE_ATK,
		STATE_TURN,
		STATE_MOVE,
		STATE_CHASE,
		STATE_HIT,
		STATE_HITEND,
		STATE_DEAD,
		STATE_END
	};

	enum Ani {
		Goblin_Attack_01,                         //0	frame:[61]
		Goblin_Attack_02,                         //1	frame:[63]
		Goblin_Attack_Spear,                      //2	frame:[76]
		Goblin_Attack_Dash_Back,                  //3	frame:[40]
		Goblin_Idle,                              //4	frame:[81]
		Goblin_Impact_B,                          //5	frame:[36]
		Goblin_Impact_F,                          //6	frame:[36]
		Goblin_Jump_Apex,                         //7	frame:[23]
		Goblin_Jump_Fall,                         //8	frame:[23]
		Goblin_Jump_Land,                         //9	frame:[30]
		Goblin_Jump_Launch,                       //10	frame:[11]
		Goblin_Jump_Launch_Pose,                  //11	frame:[23]
		Goblin_Sit_End,                           //12	frame:[45]
		Goblin_Sit_Idle,                          //13	frame:[63]
		Goblin_Spawn,                             //14	frame:[56]
		Goblin_Spawn_Climb_Hovel,                 //15	frame:[91]
		Goblin_Turn_L,                            //16	frame:[31]
		Goblin_Turn_R,                            //17	frame:[31]
		Goblin_Run_F,                             //18	frame:[17]
		Goblin_Death,                             //19	frame:[51]
		Goblin_DeathPose,                         //20	frame:[1]
		Goblin_Knock_b_Apex,                      //21	frame:[23]
		Goblin_Knock_b_Fall,                      //22	frame:[23]
		Goblin_Knock_b_Idle,                      //23	frame:[81]
		Goblin_Knock_b_Land,                      //24	frame:[34]
		Goblin_Knock_b_Recorver,                  //25	frame:[26]
		End
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
		CHEST_OPNE,
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
		STATE_SPAWN,
		STATE_IDLE,
		STATE_DEAD,
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
