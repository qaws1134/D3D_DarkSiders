#ifndef LoadMgr_h__
#define LoadMgr_h__

#include "Base.h"
#include "Define.h"
#include "GameObject.h"

class CLoadMgr : public CBase
{
	DECLARE_SINGLETON(CLoadMgr)
private:
	explicit CLoadMgr();
	virtual ~CLoadMgr();

public:


private:
	virtual void Free(void) override;

};


#endif // LoadMgr_h__
