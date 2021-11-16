#ifndef Layer_h__
#define Layer_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer(void);
	virtual ~CLayer(void);

public:
	CComponent*		Get_Component(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
	CGameObject*	Get_GameObject(const _tchar* pObjTag);

	map<const _tchar*, CGameObject*>&GetMapObj() { return m_mapObject; }
public:
	HRESULT		Add_GameObject(const _tchar* pObjTag, CGameObject* pInstance);

	HRESULT		Ready_Layer(void);
	_int		Update_Layer(const _float& fTimeDelta);
	void		Render_Layer(void);

private:
	map<const _tchar*, CGameObject*>		m_mapObject;



public:
	static CLayer*		Create(void);
	virtual void		Free(void);

};


END
#endif // Layer_h__
