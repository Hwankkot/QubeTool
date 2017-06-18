
#include "stdafx.h"
#include "Tool.h"

#include "ToolDoc.h"
#include "MainView.h"

#include "MainFrm.h"
#include "Value.h" // for Getting windows size

#include "Export_System.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainView
IMPLEMENT_DYNCREATE(CMainView, CView)

BEGIN_MESSAGE_MAP(CMainView, CView)
END_MESSAGE_MAP()

CMainView::CMainView()
{

}

CMainView::~CMainView()
{

}

BOOL CMainView::PreCreateWindow(CREATESTRUCT& cs)
{
	

	return CView::PreCreateWindow(cs);
}

// CMainView 그리기
void CMainView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}


// CMainView 진단
#ifdef _DEBUG
void CMainView::AssertValid() const
{
	CView::AssertValid();
}

void CMainView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CMainView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMainView 메시지 처리기
void CMainView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CMainFrame*	pMainFrm = ((CMainFrame*)AfxGetMainWnd());

	RECT rcWindow = {};
	pMainFrm->GetWindowRect(&rcWindow);

	SetRect(&rcWindow, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);

	RECT rcMainView = {};
	GetClientRect(&rcMainView);

	float	fRowFrm = float(rcWindow.right - rcMainView.right);
	float	fColFrm = float(rcWindow.bottom - rcMainView.bottom - CTRLCY);

	pMainFrm->SetWindowPos(NULL, 100, 100, WORD(FRAMECX + fRowFrm), WORD(FRAMECY + fColFrm), SWP_NOZORDER);
}
