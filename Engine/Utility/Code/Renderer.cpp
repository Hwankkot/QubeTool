#include "Renderer.h"

#include "Engine_Include.h"
#include "Engine_Function.h"
#include "Export_System.h"
#include "GameObject.h"
#include "Scene.h"

Engine::CRenderer::CRenderer(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
, m_pInput(NULL)
, m_dwCount(0)
, m_fTime(0.f)
, m_pMessage(NULL)
, m_IsChengedAlphRenderSize(false)
{
	ZeroMemory(m_szFps, sizeof(TCHAR) * 128);
	ZeroMemory(m_szKeyState, sizeof(TCHAR) * 256);
}

Engine::CRenderer::~CRenderer(void)
{

}

Engine::CRenderer* Engine::CRenderer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRenderer* pRenderer = new CRenderer(pDevice);
	
	if(FAILED(pRenderer->InitRenderer()))
		Engine::Safe_Delete(pRenderer);
	
	return pRenderer;
}


HRESULT Engine::CRenderer::InitRenderer(void)
{
	m_pInput = Get_Input();

	return S_OK;
}

bool Compare_ViewZ(Engine::CGameObject* pSour, Engine::CGameObject* pDest)
{
	return pSour->Get_ViewZ() > pDest->Get_ViewZ();
}

void Engine::CRenderer::Render(const float& fTime)
{
	m_pDevice->Clear(0, NULL, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(180, 180, 180), 1.f, 0);
	m_pDevice->BeginScene();

	Render_Priority();
	// Render_NoneAlpha();
	 Render_Alpha();
	// Render_UI();

	if(m_pMessage != NULL)
		RenderMessage();
	
	Render_FPS(fTime);

	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, NULL, NULL);
}

void Engine::CRenderer::Render_Priority(void)
{
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	RENDERLIST::iterator	iter = m_Renderlist[RENDER_PRIORITY].begin();
	RENDERLIST::iterator	iter_end = m_Renderlist[RENDER_PRIORITY].end();

	for( ; iter != iter_end; ++iter)
		(*iter)->Render();

	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void Engine::CRenderer::Render_NoneAlpha(void)
{
	RENDERLIST::iterator	iter = m_Renderlist[RENDER_NONEALPHA].begin();
	RENDERLIST::iterator	iter_end = m_Renderlist[RENDER_NONEALPHA].end();

	for( ; iter != iter_end; ++iter)
		(*iter)->Render();
}

void Engine::CRenderer::Render_Alpha(void)
{
	if(m_Renderlist[RENDER_ALPHA].size() == 0)
		return;

	sort(m_Renderlist[RENDER_ALPHA].begin(), m_Renderlist[RENDER_ALPHA].end(), Compare_ViewZ);

	/*m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);*/

	RENDERLIST::iterator	iter = m_Renderlist[RENDER_ALPHA].begin();
	RENDERLIST::iterator	iter_end = m_Renderlist[RENDER_ALPHA].end();

	for( ; iter != iter_end; ++iter)
	{
		(*iter)->Render();	
	}

	//m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void Engine::CRenderer::Render_UI(void)
{
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	RENDERLIST::iterator	iter = m_Renderlist[RENDER_UI].begin();
	RENDERLIST::iterator	iter_end = m_Renderlist[RENDER_UI].end();

	for( ; iter != iter_end; ++iter)
		(*iter)->Render();

	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void Engine::CRenderer::SetMessage(const TCHAR* pMessage)
{
	m_pMessage = pMessage;
}

void Engine::CRenderer::RenderMessage(void)
{
	Get_FontMgr()->Render(L"Cambria", D3DXVECTOR3(0.f, 12.f, 0.f), m_pMessage, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	m_pMessage = NULL;
}

void Engine::CRenderer::Render_FPS(const float& fTime)
{
	m_fTime += fTime;
	++m_dwCount;

	if(m_fTime >= 1.f)
	{
		wsprintf(m_szFps, L"FPS: %d", m_dwCount);
		m_fTime = 0.f;
		m_dwCount = 0;
	}

	Get_FontMgr()->Render(L"Cambria", D3DXVECTOR3(0.f, 0.f, 0.f), m_szFps, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

void Engine::CRenderer::AddRenderObject(RENDERID eRenderID, CGameObject* pGameObject)
{
	if(pGameObject == NULL)
		return;

	m_IsChengedAlphRenderSize = true;

	m_Renderlist[eRenderID].push_back(pGameObject);
}

void Engine::CRenderer::DeleteRenderObject(RENDERID eRenderID, CGameObject* pGameObject)
{
	size_t iSize = m_Renderlist[eRenderID].size();
	size_t iRenderIndex = 0;

	for(; iRenderIndex < iSize; ++iRenderIndex)
	{
		if(m_Renderlist[eRenderID][iRenderIndex] == pGameObject)
			break;
	}

	m_Renderlist[eRenderID][iRenderIndex] = m_Renderlist[eRenderID][iSize - 1];
	m_Renderlist[eRenderID].pop_back();

	int i = 0;
}

void Engine::CRenderer::ChangeCurrentScene(CScene* pScene)
{
	m_pScene = pScene;
}

void Engine::CRenderer::ResetRenderlist(RENDERID eRenderID)
{
	if(eRenderID == RENDER_END)
	{
		Release();
		return ;
	}

	m_Renderlist[eRenderID].clear();
}

std::vector<Engine::CGameObject*>& Engine::CRenderer::GetGameObjectlist(RENDERID eRenderID)
{
	return m_Renderlist[eRenderID];
}

void Engine::CRenderer::Release(void)
{
	for(WORD i = 0; i < RENDER_END; ++i)
		m_Renderlist[i].clear();
}