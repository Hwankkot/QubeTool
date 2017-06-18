#include "Management.h"

#include "Engine_Function.h"
#include "Scene.h"

IMPLEMENT_SINGLETON(Engine::CManagement)

Engine::CManagement::CManagement(void)
: m_pScene(NULL)
, m_pRenderer(NULL)
, m_pDevice(NULL)
{

}

Engine::CManagement::~CManagement(void)
{
	Release();
}

HRESULT Engine::CManagement::InitManagement(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
	m_pRenderer = CRenderer::Create(pDevice);
	NULL_CHECK_RETURN_MSG(m_pRenderer, E_FAIL, L"Failed to create Renderer");

	return S_OK;
}

void Engine::CManagement::Update(void)
{
	if(m_pScene != NULL)
		m_pScene->Update();
}

void Engine::CManagement::Render(const float& fTime)
{
	if(m_pRenderer != NULL)
		m_pRenderer->Render(fTime);
}

Engine::CRenderer* Engine::CManagement::GetRenderer(void) const
{
	return m_pRenderer;
}

void Engine::CManagement::Release(void)
{
	Engine::Safe_Delete(m_pRenderer);  
	Engine::Safe_Delete(m_pScene);
}

