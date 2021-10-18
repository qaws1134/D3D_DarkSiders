
#ifndef RayPickManager_h__
#define RayPickManager_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CRayPickManager : public CBase
{
	DECLARE_SINGLETON(CRayPickManager)
private:
	explicit CRayPickManager(void);
	virtual ~CRayPickManager(void);

public:
	void	Set_Device(LPDIRECT3DDEVICE9 pDevice);
	/* 광선 만드는 함수 */
	RAY		Create_MouseRay(const _vec2& MousePos, const _vec2& WindowSize);
	/* 월드광선을 로컬까지 내리는 함수 */


private:
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;

public:
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
};
END

#endif // RayPickManager_h__
