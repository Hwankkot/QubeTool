/*!
 * \file Management.h
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

#ifndef Management_h__
#define Management_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"
#include "Renderer.h"

BEGIN(Engine)

class CScene;

class ENGINE_DLL CManagement
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement(void);
	~CManagement(void);

public:
	HRESULT InitManagement(LPDIRECT3DDEVICE9 pDevice);
	void Update(void);
	void Render(const float& fTime);

public:
	CRenderer* GetRenderer(void) const;

public:
	template <typename T>
	HRESULT ChangeScene(T& Functor);

private:
	void Release(void);

private:
	CScene*		m_pScene;
	CRenderer*	m_pRenderer;

private:
	LPDIRECT3DDEVICE9  m_pDevice;
};

template <typename T>
HRESULT Engine::CManagement::ChangeScene(T& Functor)
{
	if(m_pScene != NULL)
		Engine::Safe_Delete(m_pScene);

	FAILED_CHECK_RETURN(Functor(&m_pScene, m_pDevice), E_FAIL);

	m_pRenderer->ChangeCurrentScene(m_pScene);
	return S_OK;
}

END

#endif // Management_h__