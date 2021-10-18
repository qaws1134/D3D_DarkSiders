#ifndef ProtoMgr_h__
#define ProtoMgr_h__

#include "Engine_Define.h"
#include "Base.h"

#include "Component.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CProtoMgr : public CBase
{
	DECLARE_SINGLETON(CProtoMgr)

private:
	explicit CProtoMgr(void);
	virtual ~CProtoMgr(void);

public:
	HRESULT		Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance);
	CComponent* Clone_Prototype(const _tchar* pProtoTag);

private:
	CComponent*	Find_Prototype(const _tchar* pProtoTag);

private:
	map<const _tchar*, CComponent*>		m_mapPrototype;

public:
	virtual void	Free(void);
};

END
#endif // ProtoMgr_h__
