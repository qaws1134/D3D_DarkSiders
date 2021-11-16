#pragma once
#ifndef ColMgr_h__
#define ColMgr_h__

#include "Engine_Define.h"
#include "Base.h"

#include "Component.h"
#include "GameObject.h"

BEGIN(Engine)
class ENGINE_DLL CColMgr :public CBase
{
public:
	CColMgr();
	virtual ~CColMgr();
public:
	static void Col_Body(COLCHECK eColCheck,map<const _tchar* ,CGameObject*> _Dst,map<const _tchar* ,CGameObject*> _Src, MESHTYPE eMesh);
	static void SetColType(COLCHECK eColCheck,wstring* pDstTag, wstring* pSrcTag);
	
};
END
#endif // !ColMgr_h__
