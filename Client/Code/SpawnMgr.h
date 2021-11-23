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

	//매시 스폰
	CGameObject*  Spawn(wstring Objkey, MESH tMesh, wstring* pLayerTag);


private:
	virtual void Free(void) override;

private:
	_uint m_iBuidingIdx = 0;
	_uint m_iEnemyIdx = 0;
	_uint m_iGrinnerIdx = 0;
	
};


#endif // SpawnMgr_h__
