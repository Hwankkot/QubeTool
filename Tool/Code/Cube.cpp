#include "stdafx.h"
#include "Cube.h"

const float CCube::CUBESCALE = 0.0625f;
const float CCube::CUBERADIUS = 2.f;

CCube::CCube(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pInput(NULL)
, m_pLayer(NULL)
, m_pRayCollision(NULL)
, m_pRenderer(NULL)
, m_pResourceMgr(NULL)
, m_pInfo(NULL)
, m_pMesh(NULL)
, m_pSphere(NULL)
, m_IsIntersectedtoSelect(false)
, m_IsSelected(false)
{
	
}

CCube::~CCube(void)
{

}

DWORD CCube::GetCubeType(void)
{
	return m_dwCubeType;
}
