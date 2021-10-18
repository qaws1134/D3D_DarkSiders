#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "ProtoMgr.h"

#include "TriCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "CubeTex.h"

#include "Texture.h"
#include "Transform.h"
#include "Renderer.h"
#include "Calculator.h"

#include "LightMgr.h"

#include "Camera.h"

#include "StaticMesh.h"
#include "Collider.h"
#include "ColliderSphere.h"
#include "RayPickManager.h"


BEGIN(Engine)


// Management
inline CComponent*	Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
inline CGameObject*	Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);

inline HRESULT		Create_Management(CManagement** ppManagement);
inline HRESULT		Set_Scene(CScene* pScene);
inline _int			Update_Scene(const _float& fTimeDelta);
inline void			Render_Scene(LPDIRECT3DDEVICE9& pGraphicDev);

// ProtoMgr
inline HRESULT		Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance);
inline CComponent* Clone_Prototype(const _tchar* pProtoTag);

// Renderer
inline CRenderer*		Get_Renderer(void);
inline void		Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
inline void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
inline void		Clear_RenderGroup(void);

// LightMgr
inline HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLightInfo,
	const _uint& iIndex);

//RayPickMgr
inline RAY CreateMouseRay(const _vec2 & v2MousePos, const _vec2& WindowSize);
inline void SetRaycastDevice(LPDIRECT3DDEVICE9 pDevice);

inline void			Release_Utility(void);

#include "Export_Utility.inl"

END
#endif // Export_Utility_h__
