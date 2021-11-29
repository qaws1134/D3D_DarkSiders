#include "Export_Utility.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

Engine::CManagement::CManagement(void)
{

}

Engine::CManagement::~CManagement(void)
{
	Free();
}

HRESULT Engine::CManagement::Set_Scene(CScene* pScene)
{

	// m_mapSaveLayer = m_pScene->GetSaveLayer();

	ReleaseScene();
	m_pScene = pScene;
	if (!m_mapSaveLayer.empty())
		m_pScene->Add_Layer(m_mapSaveLayer);
	//Safe_Release(m_pScene);	// �����սô�!!!!!!!!!!!!

	//Clear_RenderGroup();

	//m_pScene = pScene;

	return S_OK;
}

Engine::_int Engine::CManagement::Update_Scene(const _float& fTimeDelta)
{
	if (nullptr == m_pScene)
		return -1;

	m_pScene->Update_Scene(fTimeDelta);

	return 0;
}

void Engine::CManagement::Render_Scene(LPDIRECT3DDEVICE9& pGraphicDev)
{
	Render_GameObject(pGraphicDev);

	if (nullptr == m_pScene)
		return;

	m_pScene->Render_Scene();
}

HRESULT Engine::CManagement::Ready_Shader(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	CShader*		pShader = nullptr;

	D3DVIEWPORT9		ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	// ���� Ÿ�� ����
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Albedo", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Albedo", 0.f, 0.f, 200.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Normal", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Normal", 0.f, 200.f, 200.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Depth", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Depth", 0.f, 400.f, 200.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Shade", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Shade", 200.f, 0.f, 200.f, 200.f), E_FAIL);

	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Specular", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Specular", 200.f, 200.f, 200.f, 200.f), E_FAIL);



	// ��Ƽ ���� �׷� ��
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Deferred", L"Target_Albedo"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Deferred", L"Target_Normal"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Deferred", L"Target_Depth"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_LightAcc", L"Target_Shade"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_LightAcc", L"Target_Specular"), E_FAIL);


	// shader_sample
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Sample.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Shader_Sample", pShader), E_FAIL);

	// shader_terrain
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Terrain.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Shader_Terrain", pShader), E_FAIL);

	// shader_mesh
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Mesh.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Shader_Mesh", pShader), E_FAIL);

	// shader_shade
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Shade.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Shader_Shade", pShader), E_FAIL);

	// shader_shade
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Blend.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Shader_Blend", pShader), E_FAIL);

	// shader_shade
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_UI.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Shader_UI", pShader), E_FAIL);
	// shader_shade
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Effect.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Shader_Effect", pShader), E_FAIL);

	pShader = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_MeshEffect.hpp");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Shader_MeshEffect", pShader), E_FAIL);

	return S_OK;
}

void CManagement::ReleaseScene()
{
	
	//SaveLayer(L"Player");
	SaveLayer(L"UI");
	SaveLayer(L"Effect");
	SaveLayer(L"Bullet_Enemy");
	SaveLayer(L"Bullet_Player");
	SaveLayer(L"Particle");
	//SaveLayer(L"Item");


	Safe_Release(m_pScene);

	Clear_RenderGroup();

}

void CManagement::SaveLayer(wstring LayerTag)
{
	if (!m_pScene)
		return;

	USES_CONVERSION;

	const _tchar* pConvLayerTag = W2BSTR(LayerTag.c_str());

	auto& iter_find = find_if(m_mapSaveLayer.begin(), m_mapSaveLayer.end(), CTag_Finder(LayerTag.c_str()));
	if (iter_find == m_mapSaveLayer.end())
	{
		if (m_pScene->GetLayer(LayerTag.c_str()))
			m_mapSaveLayer.emplace(pConvLayerTag, m_pScene->GetLayer(LayerTag.c_str()));
	}
	m_pScene->EraseMapObj(LayerTag.c_str());


}

void Engine::CManagement::Free(void)
{
	//m_pScene->Release_SaveLayer();

	for_each(m_mapSaveLayer.begin(), m_mapSaveLayer.end(), CDeleteMap());
	Safe_Release(m_pScene);
	m_mapSaveLayer.clear();
}

Engine::CComponent* Engine::CManagement::Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

CGameObject * CManagement::Get_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_GameObject(pLayerTag, pObjTag);
}

void CManagement::Add_GameObject(const _tchar * pLayerTag, const _tchar* pObjTag, CGameObject* pInstance)
{
	if (nullptr == m_pScene)
		return ;

	m_pScene->Add_GameObject(pLayerTag, pObjTag, pInstance);
}

