#ifndef Scene_h__
#define Scene_h__

#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene(void);

public:
	CComponent*		Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
	CGameObject*	Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);
	void			Add_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag,CGameObject* pInstance);
public:
	virtual HRESULT		Ready_Scene(void);
	virtual _int		Update_Scene(const _float& fTimeDelta);
	virtual void		Render_Scene(void);
	virtual void		Begin_Scene(void);

	map<const _tchar*, CGameObject*> *  GetMapObj(wstring LayerTag);

	
	//void Release_SaveLayer();
	//map<const _tchar*, CLayer*> GetSaveLayer() { return m_mapSaveLayer; }
	//void SetSaveLayer(map<const _tchar*, CLayer*> mapSaveLayer) { m_mapSaveLayer = mapSaveLayer; }

protected:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	map<const _tchar*, CLayer*>		m_mapLayer;
	//map<const _tchar*, CLayer*>		m_mapSaveLayer;

	_bool							m_bBegin =false;
public:
	virtual void Free(void);
};

END
#endif // Scene_h__
