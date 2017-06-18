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
// 공통 전역변수
//-----------------------------------------------------

// 윈도우 창 크기 관련
const WORD WINCX = 1280;			// 디바이스 출력 창 크기
const WORD WINCY = 720;				
	
const WORD CTRLCX = WINCX;			// 컨트롤 메뉴 창 크기
const WORD CTRLCY = 200;

const WORD FRAMECX = WINCX;			// 전체 프레임 크기
const WORD FRAMECY = WINCY + CTRLCY;


// 고정 수치
const float FLOATMAX = 3.402823466e+38f;		// float Max값
const D3DXVECTOR3 g_vLook(0.f, 0.f, -1.f);		// 기본 Look벡터
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

	// 로고 화면에 띄울 이미지를 로딩한다.
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


	// 입력장치 초기화
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
	// 장치(마우스, 키보드) 입력을 갱신한다.
	m_pInput->UpdateKey();

	// Clock 시간을 재 설정한다.
	m_pTimeMgr->SetTime();

	// Management가 가지고 있는 Scene을 업데이트한다.
	m_pManagement->Update();
}

void CMainApp::Render(void)
{
	// Clock 시간을 가지고 온다.
	float fTime = m_pTimeMgr->GetTime();

	// 위에서 받은 시간을 기준으로 렌더링한다.
	m_pManagement->Render(fTime);
}

void CMainApp::Release(void)
{
	// System
	Engine::Safe_Single_Destory(m_pGraphicDev); /// 디바이스
	Engine::Safe_Single_Destory(m_pTimeMgr);	/// 타이머
	Engine::Safe_Single_Destory(m_pManagement);
	Engine::Safe_Single_Destory(m_pFontMgr);	/// 폰트
	Engine::Safe_Single_Destory(m_pInput);		/// 입력장치

	// Utility
	Engine::Get_LightMgr()->DestroyInstance();
	Engine::Get_NavMgr()->DestroyInstance();

	// Resources
	Engine::Get_ResourceMgr()->DestroyInstance();

	// Tool
	CMapEditSubject::GetInstance()->DestroyInstance();
	CMouseSubject::GetInstance()->DestroyInstance();

}