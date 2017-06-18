
/*!
 * \file Renderer.h
 * \date 2016/04/10 15:35
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"
#include "Engine_Enum.h"

BEGIN(Engine)

class CScene;
class CGameObject;
class CInput;

class ENGINE_DLL CRenderer
{
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pDevice);
	
private:
	explicit CRenderer(const CRenderer&);
	CRenderer& operator = (const CRenderer&);

public:
	~CRenderer(void);

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	HRESULT InitRenderer(void);
	void Render(const float& fTime);

private:
	void Render_Priority(void);
	void Render_NoneAlpha(void);
	void Render_Alpha(void);
	void Render_UI(void);

private:
	void RenderMessage(void);
	void Render_FPS(const float& fTime);

public:
	void SetMessage(const TCHAR* pMessage);
	void AddRenderObject(RENDERID eRenderID, CGameObject* pGameObject);
	void DeleteRenderObject(RENDERID eRenderID, CGameObject* pGameObject);
	void ChangeCurrentScene(CScene* pScene);

public:
	void ResetRenderlist(RENDERID eRenderID);

public:
	std::vector<CGameObject*>& GetGameObjectlist(RENDERID eRenderID);

private:
	void Release(void);

private:
	CScene*		m_pScene;
	CInput*		m_pInput;

private:
	LPDIRECT3DDEVICE9	m_pDevice;
	DWORD	m_dwCount;
	float	m_fTime;
	const TCHAR*	m_pMessage;
	TCHAR	m_szFps[128];
	TCHAR	m_szKeyState[256];

private:
	bool m_IsChengedAlphRenderSize;
	
private:
	typedef std::vector<CGameObject*>	RENDERLIST;
	RENDERLIST	m_Renderlist[RENDER_END];
};

END

#endif // Renderer_h__