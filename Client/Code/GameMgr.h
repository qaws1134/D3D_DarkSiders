#ifndef GameMgr_h__
#define GameMgr_h__

#include "Base.h"
#include "Define.h"
#include "GameObject.h"

class CGameMgr : public CBase
{

	DECLARE_SINGLETON(CGameMgr)
private:
	explicit CGameMgr();
	virtual ~CGameMgr();

public:
	CGameObject* GetPlayer() { return m_pPlayer; }
	void SetPlayer(CGameObject*	pPlayer) { m_pPlayer = pPlayer; }
	

	
private:
	//플레이어 저장 
	CGameObject* m_pPlayer = nullptr;


	_float m_fSoul;
	


private:
	virtual void Free(void) override;

};


#endif // GameMgr_h__
