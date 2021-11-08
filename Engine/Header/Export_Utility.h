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
#include "Collider.h"
#include "ColliderSphere.h"
#include "RayPickManager.h"
#include "Optimization.h"


#include "LightMgr.h"

#include "Camera.h"

#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "NaviMesh.h"

#include "Shader.h"
#include "RenderTargetMgr.h"

BEGIN(Engine)


// Management
inline CComponent*	Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
inline CGameObject*	Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);
inline HRESULT		Create_Management(LPDIRECT3DDEVICE9& pGraphicDev,CManagement** ppManagement);
inline HRESULT		Set_Scene(CScene* pScene);
inline _int			Update_Scene(const _float& fTimeDelta);
inline void			Render_Scene(LPDIRECT3DDEVICE9& pGraphicDev);

// ProtoMgr
inline HRESULT		Ready_Renderer(LPDIRECT3DDEVICE9& pGraphicDev);
inline HRESULT		Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance);
inline CComponent* Clone_Prototype(const _tchar* pProtoTag);

// Renderer
inline CRenderer*	Get_Renderer(void);
inline void			Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
inline void			Render_MFCGameObject(LPDIRECT3DDEVICE9& pGraphicDev);
inline void			Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
inline void			Clear_RenderGroup(void);

// LightMgr
inline HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLightInfo,
	const _uint& iIndex);     
inline void						Render_Light(LPD3DXEFFECT& pEffect);
inline const D3DLIGHT9*			Get_Light(const _uint& iIndex = 0);

//RayPickMgr
inline RAY			CreateMouseRay(const _vec2 & v2MousePos, const _vec2& WindowSize);
inline void			SetRaycastDevice(LPDIRECT3DDEVICE9 pDevice);


// RenderTarget
inline HRESULT		Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTargetTag, const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR Color);
inline HRESULT		Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);
inline HRESULT		Begin_MRT(const _tchar* pMRTTag);
inline HRESULT		End_MRT(const _tchar* pMRTTag);

inline HRESULT		Ready_DebugBuffer(const _tchar* pTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
inline void			Render_DebugBuffer(const _tchar* pMRTTag);
inline void			Get_RenderTargetTexture(LPD3DXEFFECT& pEffect, const _tchar* pTargetTag, const char* pConstantTable);


inline void			Release_Utility(void);  

#include "Export_Utility.inl"

END
#endif // Export_Utility_h__
