#ifndef Well512Random_h__
#define Well512Random_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL Well512Random : public CBase
{
	DECLARE_SINGLETON(Well512Random)
private:
	Well512Random();
	virtual ~Well512Random(void);

public:
	void RandomInit(unsigned int nSeed);
	unsigned int Next(int minValue, int maxValue);
	unsigned int Next(unsigned int maxValue);
	unsigned int Next();
protected:
	unsigned int state[16];
	unsigned int index;

public:
	// CBase을(를) 통해 상속됨
	virtual void Free(void) override;

};

END
#endif // FontMgr_h__
