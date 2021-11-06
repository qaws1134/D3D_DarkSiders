#ifndef SpawnMgr_h__
#define SpawnMgr_h__

#include "Base.h"
#include "Define.h"

#include "GameObject.h"

class CSpawnMgr : public CBase
{
	DECLARE_SINGLETON(CSpawnMgr)
private:
	explicit CSpawnMgr();
	virtual ~CSpawnMgr();


private:
	virtual void Free(void) override;

};


#endif // SpawnMgr_h__
