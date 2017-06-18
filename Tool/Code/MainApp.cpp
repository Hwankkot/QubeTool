#include "stdafx.h"
#include "MainApp.h"

#include "Engine_Function.h"
#include "Engine_Macro.h"
#include "Enum.h"
#include "Export_Resources.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "MapEditSubject.h"
#include "MouseSubject.h"
#include "SelectScene.h"
#include "Value.h"

//-----------------------------------------------------
// ���� ��������
//-----------------------------------------------------

// ������ â ũ�� ����
const WORD WINCX = 1280;			// ����̽� ��� â ũ��
const WORD WINCY = 720;				
	
const WORD CTRLCX = WINCX;			// ��Ʈ�� �޴� â ũ��
const WORD CTRLCY = 200;

const WORD FRAMECX = WINCX;			// ��ü ������ ũ��
const WORD FRAMECY = WINCY + CTRLCY;


// ���� ��ġ
const float FLOATMAX = 3.402823466e+38f;		// float Max��
const D3DXVECTOR3 g_vLook(0.f, 0.f, -1.f);		// �⺻ Look����
//-----------------------------------------------------


CMainApp::CMainApp(void)
: m_pDevice(NULL)
, m_pFontMgr(NULL)
, m_pGraphicDev(NULL)
, m_pInput(NULL)
, m_pManagement(NULL)
, m_pTimeMgr(NULL)
{

}

CMainApp::~CMainApp(void)
{
	Release();
}

CMainApp* CMainApp::Create(void)
{
	CMainApp* pMainApp = new CMainApp;

	if(FAILED(pMainApp->InitMainApp()))
		Engine::Safe_Delete(pMainApp);

	return pMainApp;
}

HRESULT CMainApp::InitMainApp(void)
{
	m_pGraphicDev = Engine::Get_GraphicDev();

	HRESULT hr = NULL;

	hr = m_pGraphicDev->InitGraphicDev(Engine::CGraphicDev::MODE_WIN, g_hWnd, WINCX, WINCY);
	
	m_pDevice = m_pGraphicDev->GetDevice();
	
	m_pInput = Engine::Get_Input();
	m_pFontMgr = Engine::Get_FontMgr();
	m_pManagement = Engine::Get_Management();
	m_pTimeMgr = Engine::Get_TimeMgr();
	Engine::Get_NavMgr()->InitNavMeshManager(m_pDevice, 100);

	// �ΰ� ȭ�鿡 ��� �̹����� �ε��Ѵ�.
	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pDevice
		, Engine::BUFFER_RCTEX
		, RESOURCEID::TEXBUFFER_RECTANGLE);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"Failed to create Buffer_RcTex");

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, Engine::TEXTURE_NORMAL
		, RESOURCEID::RECTTEXTURE_LOGO
		, L"../../Data/Resources/Texture/Logo.tga"
		, 1);		
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"Failed to create Logo.tga");


	// �Է���ġ �ʱ�ȭ
	m_pInput->InitInputDevice(AfxGetInstanceHandle(), g_hWnd);

	hr = m_pManagement->InitManagement(m_pDevice);
	hr = m_pManagement->ChangeScene(CSelectScene(CSelectScene::SCENE_LOGO));
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"Failed to Change Scene ");

	hr = m_pFontMgr->AddFont(m_pDevice, L"Cambria", 16, 12, FW_LIGHT);
	FAILED_CHECK(hr);

	m_pTimeMgr->InitTime();

	return S_OK;
}

void CMainApp::Update(void)
{
	// ��ġ(���콺, Ű����) �Է��� �����Ѵ�.
	m_pInput->UpdateKey();

	// Clock �ð��� �� �����Ѵ�.
	m_pTimeMgr->SetTime();

	// Management�� ������ �ִ� Scene�� ������Ʈ�Ѵ�.
	m_pManagement->Update();
}

void CMainApp::Render(void)
{
	// Clock �ð��� ������ �´�.
	float fTime = m_pTimeMgr->GetTime();

	// ������ ���� �ð��� �������� �������Ѵ�.
	m_pManagement->Render(fTime);
}

void CMainApp::Release(void)
{
	// System
	Engine::Safe_Single_Destory(m_pGraphicDev); /// ����̽�
	Engine::Safe_Single_Destory(m_pTimeMgr);	/// Ÿ�̸�
	Engine::Safe_Single_Destory(m_pManagement);
	Engine::Safe_Single_Destory(m_pFontMgr);	/// ��Ʈ
	Engine::Safe_Single_Destory(m_pInput);		/// �Է���ġ

	// Utility
	Engine::Get_LightMgr()->DestroyInstance();
	Engine::Get_NavMgr()->DestroyInstance();

	// Resources
	Engine::Get_ResourceMgr()->DestroyInstance();

	// Tool
	CMapEditSubject::GetInstance()->DestroyInstance();
	CMouseSubject::GetInstance()->DestroyInstance();

}