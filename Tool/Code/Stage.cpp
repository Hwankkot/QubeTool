#include "stdafx.h"
#include "Stage.h"

#include "ControlView.h"
#include "Engine_Function.h"
#include "Enum.h"
#include "Export_System.h" /// for using font
#include "Export_Utility.h"
#include "FreeCam.h"
#include "Layer.h"
#include "MainFrm.h"
#include "MapEdit.h"
#include "InputUnit.h"
#include "Renderer.h"

// Cube
#include "WhiteCube.h"
#include "CubeWall.h"


CStage::CStage(LPDIRECT3DDEVICE9 pDevice)
: Engine::CScene(pDevice)
, m_pCubeDataLayer(NULL)
, m_pRenderer(NULL)
, m_pInputUnit(NULL)
{

}

CStage::~CStage(void)
{

}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStage* pScene = new CStage(pDevice);

	if(FAILED(pScene->InitScene()))
		Engine::Safe_Delete(pScene);

	return pScene;
}

HRESULT CStage::InitScene(void)
{
	m_pRenderer = Engine::Get_Management()->GetRenderer();

	FAILED_CHECK(AddUI());
	FAILED_CHECK(AddGamelogic());
	FAILED_CHECK(AddLight());

	((CMainFrame*)AfxGetMainWnd())->GetControlView()->GetMapEdit()->SetInputUnit(m_pInputUnit);
	((CMainFrame*)AfxGetMainWnd())->GetControlView()->GetMapEdit()->SetCubeDataStorage(m_pCubeDataLayer);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, false);

	return S_OK;
}

void CStage::Update(void)
{
	Engine::CScene::Update();
}

HRESULT CStage::AddUI(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CFreeCam::Create(m_pDevice, &D3DXVECTOR3(0.f, 5.f, -25.f), &D3DXVECTOR3(0.f, 0.f, 0.f));
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Failed to create John");
	pLayer->AddObject(GAMEOBJECTID::UI_FREECAM, pGameObject);

	pGameObject = m_pInputUnit = CInputUnit::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Failed to create Mouse");
	m_pRenderer->AddRenderObject(Engine::RENDER_PRIORITY, pGameObject);
	pLayer->AddObject(GAMEOBJECTID::UI_INPUTUNIT , pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));
	return S_OK;
}

HRESULT CStage::AddGamelogic(void)
{	
	Engine::CLayer*	pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = NULL;
	m_pInputUnit->SetCubeLayer(pLayer);

	pGameObject = CWhiteCube::Create(m_pDevice, pLayer, GAMEOBJECTID::CUBE_WHITE);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Failed to create Cube");
	m_pRenderer->AddRenderObject(Engine::RENDER_ALPHA, pGameObject);
	pLayer->AddObject(GAMEOBJECTID::CUBE_WHITE , pGameObject);

	m_pCubeDataLayer = pLayer;

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));

	return S_OK;
}

HRESULT CStage::AddLight(void)
{
	D3DLIGHT9		LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Direction = D3DXVECTOR3(1.f, -10.f, 1.f);

	FAILED_CHECK(Engine::Get_LightMgr()->AddLight(m_pDevice, &LightInfo, 0));


	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Direction = D3DXVECTOR3(1.f, 10.f, 1.f);

	FAILED_CHECK(Engine::Get_LightMgr()->AddLight(m_pDevice, &LightInfo, 1));

	////Α΅Α¶Έν
	//ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));
	//LightInfo.Type = D3DLIGHT_POINT;
	//LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//LightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//LightInfo.Range = 10.f;
	//LightInfo.Position = D3DXVECTOR3(0.f, 3.f, 0.f);
	//LightInfo.Attenuation0 = 1.f;

	//FAILED_CHECK(Engine::Get_LightMgr()->AddLight(m_pDevice, &LightInfo, 1));

	//ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));
	//LightInfo.Type = D3DLIGHT_POINT;
	//LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
	//LightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//LightInfo.Range = 10.f;
	//LightInfo.Position = D3DXVECTOR3(10.f, 3.f, 10.f);
	//LightInfo.Attenuation0 = 1.f;

	//FAILED_CHECK(Engine::Get_LightMgr()->AddLight(m_pDevice, &LightInfo, 2));

	return S_OK;
}
