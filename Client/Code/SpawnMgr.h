#ifndef SpawnMgr_h__
#define SpawnMgr_h__

#include "Base.h"
#include "Define.h"
#include "Struct.h"
#include "GameObject.h"

class CSpawnMgr : public CBase
{
	DECLARE_SINGLETON(CSpawnMgr)
private:
	explicit CSpawnMgr();
	virtual ~CSpawnMgr();
public:
	//컬라이더 스폰
	CGameObject* Spawn(CGameObject* pTargetObj, LOAD_DATA_COL tCol);
	//네비매시 스폰
	void Spawn(MESH tMesh);
	//매시 스폰
	CGameObject*  Spawn(wstring Objkey, MESH tMesh);


private:
	virtual void Free(void) override;

};


#endif // SpawnMgr_h__
