/*!
 * \file Stage.h
 * \date 2016/04/10 15:02
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
#ifndef Stage_h__
#define Stage_h__

#include "Scene.h"

namespace Engine
{ 
	class CGameObject;
	class CRenderer;
}
class CInputUnit;
class CStage : public Engine::CScene
{
public:
	enum LAYER { LAYER_UI, LAYER_GAMELOGIC, LAYER_END };
private:
	explicit CStage(LPDIRECT3DDEVICE9 pDevice);
public:
	~CStage(void);

public:
	static CStage* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual HRESULT InitScene(void);
	virtual void Update(void);

private:
	HRESULT AddGamelogic(void);
	HRESULT AddUI(void);
	HRESULT AddLight(void);

private:
	Engine::CLayer*	m_pCubeDataLayer;
	Engine::CRenderer* m_pRenderer;
	CInputUnit*	m_pInputUnit;
};

#endif // Stage_h__