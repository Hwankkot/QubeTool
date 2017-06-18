#include "stdafx.h"
#include "Logo.h"

#include "Enum.h"
#include "Engine_Function.h"
#include "Export_Resources.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Layer.h"
#include "Loading.h"
#include "Scenery.h"
#include "SelectScene.h"
#include "VIBuffer.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pDevice)
: Engine::CScene(pDevice)
, m_pManagement(NULL)
, m_pLoading(NULL)
{

}

CLogo::~CLogo(void)
{
	Release();
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLogo* pScene = new CLogo(pDevice);
	
	if(FAILED(pScene->InitLogo()))
		Engine::Safe_Delete(pScene);

	return pScene;
}

HRESULT CLogo::InitLogo(void)
{
	m_pManagement = Engine::Get_Management();

	/// ���� ���� Stage���� ���� �ٽ� �θ���.
	m_pDevice->SetRenderState(D3DRS_LIGHTING, false);

	/// �޽��� �ε��� �����带 �����.
	m_pLoading = CLoading::Create();
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	HRESULT hr = NULL;


	// Scenery �߰�
	AddEnvironmentLayer();

	return S_OK;
}

void CLogo::Update(void)
{
	Engine::CScene::Update();

	if(m_pLoading->GetComplete() == true)
	{
		 // TODO: Change to KeyMgr
		if(GetAsyncKeyState(VK_RETURN))
		{
			m_pManagement->GetRenderer()->ResetRenderlist(Engine::RENDER_END);
			HRESULT hr = m_pManagement->ChangeScene(CSelectScene(CSelectScene::SCENE_STAGE));
			FAILED_CHECK_RETURN_MSG(hr, , L"Failed to change Scene");
			
			/// Scene�� �ٲ� ���Ŀ� Render�� �ٷ� �����ϰ� �ǹǷ� Update�� ���������� �����ش�.
			/// Logo�� �ִ� m_pManagement �����ʹ� �ҽǵǹǷ� ���� �Լ��� ȣ���ؼ� Update�Ѵ�.
			Engine::Get_Management()->Update();
			return ;
		}
	}

	m_pManagement->GetRenderer()->SetMessage(m_pLoading->GetLoadingMessage());
}

HRESULT CLogo::AddEnvironmentLayer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CScenery::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Failed to create Scenery");

	m_pManagement->GetRenderer()->AddRenderObject(Engine::RENDER_PRIORITY, pGameObject);
	pLayer->AddObject(GAMEOBJECTID::ENV_SCENERY, pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_ENVIRONMENT, pLayer));

	return S_OK;
}

void CLogo::Release(void)
{
	Engine::Safe_Delete(m_pLoading);
}
