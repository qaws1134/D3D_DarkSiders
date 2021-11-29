#ifndef StartStage_h__
#define StartStage_h__

#include "Define.h"
#include "Scene.h"
#include "Loading.h"
#include "BackGround.h"

#include "Player.h"
#include "WaterBoss.h"
//#include "Monster.h"
#include "Terrain.h"
#include "DynamicCamera.h"
#include "StaticCamera.h"
#include "GameMgr.h"
#include "UIMgr.h"
#include "UI.h"



//#include "SkyBox.h"
//#include "Stone.h"
//#include "Sword.h"
//#include "Effect.h"
//#include "UI.h"

class CStartStage : public CScene
{
private:
	explicit CStartStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStartStage(void);

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(void) override;
	virtual void Begin_Scene() override;

	void SetNextScene() { m_bSceneStart = true; }

private:
	HRESULT				Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT				Ready_LightInfo(void);

	
private:
	CLoading*			m_pLoading = nullptr;

	_ulong					m_dwRenderCnt = 0;

	_long					m_dwPosX = 0;
	_long					m_dwPosY = 0;

	_bool	m_bSceneStart =false;
	_tchar					m_szFPS[256] ;
	_tchar					m_szPosX[256];
	_tchar					m_szPosY[256];
	_float					m_fTime = 0.f;

public:
	static CStartStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};


#endif // StartStage_h__
