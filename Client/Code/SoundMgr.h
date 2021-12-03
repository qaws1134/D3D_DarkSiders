#pragma once

#include "Define.h"
#include "Base.h"
#include "GameObject.h"
#include "io.h"
#include "fmod.hpp"
#include "fmod_dsp.h"
#include "fmod_errors.h"
#include "fmod.h"
#pragma comment(lib, "fmodex_vc.lib")

class CSoundMgr : public CBase
{
public:
	static CSoundMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSoundMgr; 

		return m_pInstance; 
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance; 
			m_pInstance = nullptr; 
		}
	}
public:
	enum CHANNELID
	{
		CHANEL_BGM,
		CHANEL_STARTSTAGE,
		CHANEL_STAGE,
		CHANNEL_PLAYER,
		CHANNEL_PLAYER_ATK1,
		CHANNEL_PLAYER_ATK2,
		CHANNEL_NPC,
		CHANNEL_GOBLIN,
		CHANNEL_GOBLIN_Vo,
		CHANNEL_GRINNER,
		CHANNEL_GRINNER_Vo,
		CHANNEL_ANGEL,
		CHANNEL_ANGEL_Vo,
		CHANNEL_WATERBOSS,
		CHANNEL_MONSTER_ATK_1,
		CHANNEL_MONSTER_ATK_2,
		CHANNEL_MONSTER_ATK_3,
		CHANNEL_MONSTER_ATK_4,
		CHANNEL_EVNET,
		CHANNEL_ITEM,
		CHANNEL_SERPENT,
		CHANNEL_SERPENT_IDLE,
		CHANNEL_EFFECT1,
		CHANNEL_EFFECT2,
		CHANNEL_UI,
		CHANNEL_UI1,
		CHANNEL_UI2,
		CHANNEL_DOOR,

		CHANNEL_CHEST,
		CHANNEL_END
	};
private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Initialize(); 

	void Release(); 
public:
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID);
	void PlayBGM(TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();

private:
	void LoadSoundFile(); 
	virtual void Free()override;
private:
	static CSoundMgr* m_pInstance; 
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound; 
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[CHANNEL_END];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem; 

};

