#include "Export_Utility.h"




CComponent*		Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	return CManagement::GetInstance()->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

CGameObject * Get_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	return CManagement::GetInstance()->Get_GameObject(pLayerTag, pObjTag);
}

HRESULT		Create_Management(LPDIRECT3DDEVICE9& pGraphicDev,CManagement** ppManagement)
{
	CManagement*		pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, E_FAIL);

	FAILED_CHECK_RETURN(pManagement->Ready_Shader(pGraphicDev), E_FAIL);


	*ppManagement = pManagement;

	return S_OK;
}

HRESULT	Set_Scene(CScene* pScene)
{
	return CManagement::GetInstance()->Set_Scene(pScene);
}
_int	Update_Scene(const _float& fTimeDelta)
{
	CManagement::GetInstance()->Update_Scene(fTimeDelta);
}
void			Render_Scene(LPDIRECT3DDEVICE9& pGraphicDev)
{
	CManagement::GetInstance()->Render_Scene(pGraphicDev);
}

 void Add_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag, CGameObject * pInstance)
{
	 CManagement::GetInstance()->Add_GameObject(pLayerTag, pObjTag, pInstance);
}



HRESULT		Ready_Renderer(LPDIRECT3DDEVICE9& pGraphicDev)
{
	return CRenderer::GetInstance()->Ready_Renderer(pGraphicDev);
}


HRESULT		Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance)
{
	return CProtoMgr::GetInstance()->Ready_Prototype(pProtoTag, pInstance);
}
CComponent* Clone_Prototype(const _tchar* pProtoTag)
{
	return CProtoMgr::GetInstance()->Clone_Prototype(pProtoTag);
}

CRenderer*		Get_Renderer(void)
{
	return CRenderer::GetInstance();
}

void		Add_RenderGroup(RENDERID eID, CGameObject* pGameObject)
{
	CRenderer::GetInstance()->Add_RenderGroup(eID, pGameObject);
}
 void Render_MFCGameObject(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	 CRenderer::GetInstance()->Render_MFCGameObject(pGraphicDev);
}
void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	CRenderer::GetInstance()->Render_GameObject(pGraphicDev);
}
void		Clear_RenderGroup(void)
{
	CRenderer::GetInstance()->Clear_RenderGroup();
}

// LightMgr
HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLightInfo,
	const _uint& iIndex)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
}

 void Render_Light(LPD3DXEFFECT & pEffect)
{
	CLightMgr::GetInstance()->Render_Light(pEffect);
}

 const D3DLIGHT9 * Get_Light(const _uint & iIndex)
{
	return CLightMgr::GetInstance()->Get_Light(iIndex);
}

 RAY CreateMouseRay(const _vec2 & v2MousePos, const _vec2 & WindowSize)
{
	return CRayPickManager::GetInstance()->Create_MouseRay(v2MousePos, WindowSize);
}

 void SetRaycastDevice(LPDIRECT3DDEVICE9 pDevice)
{
	CRayPickManager::GetInstance()->Set_Device(pDevice);
}


HRESULT		Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTargetTag, const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR Color)
{
	return CRenderTargetMgr::GetInstance()->Ready_RenderTarget(pGraphicDev, pTargetTag, iWidth, iHeight, Format, Color);
}
HRESULT		Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag)
{
	return CRenderTargetMgr::GetInstance()->Ready_MRT(pMRTTag, pTargetTag);
}

HRESULT		Begin_MRT(const _tchar* pMRTTag)
{
	return CRenderTargetMgr::GetInstance()->Begin_MRT(pMRTTag);
}
HRESULT		End_MRT(const _tchar* pMRTTag)
{
	return CRenderTargetMgr::GetInstance()->End_MRT(pMRTTag);
}

HRESULT			Ready_DebugBuffer(const _tchar* pTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY)
{
	return CRenderTargetMgr::GetInstance()->Ready_DebugBuffer(pTargetTag, fX, fY, fSizeX, fSizeY);
}
void			Render_DebugBuffer(const _tchar* pMRTTag)
{
	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(pMRTTag);
}
void			Get_RenderTargetTexture(LPD3DXEFFECT& pEffect, const _tchar* pTargetTag, const char* pConstantTable)
{
	CRenderTargetMgr::GetInstance()->Get_RenderTargetTexture(pEffect, pTargetTag, pConstantTable);
}




void		Release_Utility(void)
{
	CRenderTargetMgr::GetInstance()->DestroyInstance();
	CLightMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CRayPickManager::GetInstance()->DestroyInstance();
	CProtoMgr::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
}