#include "stdafx.h"
#include "Tool.h"

#include "MainFrm.h"

#include "ControlView.h"
#include "Engine_Function.h"
#include "Export_System.h"
#include "MainApp.h"
#include "MainView.h"
#include "Value.h"				// for Getting Windows Size.

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CSplitterX, CSplitterWnd)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()
CSplitterX::CSplitterX() {}
CSplitterX::~CSplitterX() {}

LRESULT CSplitterX::OnNcHitTest(CPoint point) 
{
	return HTNOWHERE;
}

HWND g_hWnd;

// CMainFrame
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
END_MESSAGE_MAP()

// CMainFrame Structure/Destructor.
CMainFrame::CMainFrame()
: m_pMainApp(NULL)
, m_pMainView(NULL)
, m_pControlView(NULL)
{

}

CMainFrame::~CMainFrame()
{
	Release();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style &= ~FWS_ADDTOTITLE ;
	LPCTSTR TitleName = _T("Tool") ;
	SetTitle(TitleName) ;

	if(cs.hMenu != NULL)
	{
		DestroyMenu(cs.hMenu);
		cs.hMenu = NULL;
	}
	
	cs.x = 10;
	cs.y = 10;
	cs.cx = FRAMECX;
	cs.cy = FRAMECY;

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

// CMainFrame 메시지 처리기
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// 화면 분할
	m_MainSplitter.CreateStatic(this, 2, 1);
	m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CControlView), CSize(0, CTRLCY), pContext);
	m_MainSplitter.CreateView(1, 0, RUNTIME_CLASS(CMainView), CSize(WINCX, WINCY), pContext);

	// 메인 프레임 핸들 값 저장
	g_hWnd = m_MainSplitter.GetPane(1, 0)->m_hWnd;

	// 메인 뷰 저장
	m_pMainView = (CMainView*)m_MainSplitter.GetPane(1, 0);
	m_pControlView = (CControlView*)m_MainSplitter.GetPane(0,0);
	
	// 메인 앱 생성
	m_pMainApp = CMainApp::Create();
	NULL_CHECK_RETURN(m_pMainApp, FALSE);

	return TRUE;
}

CMainApp* CMainFrame::GetMainApp(void)
{
	return m_pMainApp;
}

CMainView* CMainFrame::GetMainView(void)
{
	return m_pMainView;
}

CControlView* CMainFrame::GetControlView(void)
{
	return m_pControlView;
}

void CMainFrame::Release(void)
{
	Engine::Safe_Delete(m_pMainApp);
}

