#ifndef ClientEnum_h__
#define ClientEnum_h__

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
		MOVE_CB,
		ATTACK,
		JUMP,
		JUMP_CB,
		JUMPATTACK,
		DASH,
		HIT,
		STATE_END
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
		War_Atk_Air_Light_01,                     //25	frame:[51]
		War_Atk_Air_Light_02,                     //26	frame:[51]
		War_Atk_Air_Filpsaw_Start,                //27	frame:[8]
		War_Atk_Air_Filpsaw_Loop,                 //28	frame:[6]
		War_Atk_Air_Filpsaw_Land,                 //29	frame:[57]
		War_Atk_Dash,                             //30	frame:[54]
		War_Atk_Earth_Start,                      //31	frame:[31]
		War_Atk_Earth_Loop,                       //32	frame:[66]
		War_Atk_Earth_End,                        //33	frame:[72]
		War_Atk_Flamebrand,                       //34	frame:[28]
		War_Atk_Flamebrand_Start,                 //35	frame:[9]
		War_Atk_Flamebrand_End,                   //36	frame:[49]
		War_Chest_Open,                           //37	frame:[66]
		War_Death,                                //38	frame:[101]
		War_Block_Idle,                           //39	frame:[91]
		War_Block_Impact_Heavy,                   //40	frame:[17]
		War_Block_Impact_Light,                   //41	frame:[17]
		War_Block_Impact_Medium,                  //42	frame:[17]
		War_Block_Start,                          //43	frame:[10]
		War_Knockback_Fall,                       //44	frame:[41]
		War_Knockback_Land,                       //45	frame:[53]
		War_Knockback_Loop,                       //46	frame:[41]
		War_Knockback_Start,                      //47	frame:[21]
		War_Impack_From_Back,                     //48	frame:[31]
		War_Impack_From_Front,                    //49	frame:[21]
		War_Impack_From_Left,                     //50	frame:[21]
		War_Impack_From_Right,                    //51	frame:[21]
		War_Skill_01,                             //52	frame:[72]
		War_Skill_02_Start,                       //53	frame:[11]
		War_Skill_02_Strike,                      //54	frame:[39]
		War_Skill_02_Run,                         //55	frame:[33]
		War_Skill_03,                             //56	frame:[56]
		War_Glide,                                //57	frame:[61]
		War_Glide_Start,                          //58	frame:[21]
		War_Atk_Heavy_01,                         //59	frame:[53]
		War_Atk_Heavy_02,                         //60	frame:[59]
		War_Atk_Heavy_03,                         //61	frame:[88]
		War_End
	};
}
namespace WaterBoss {
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
