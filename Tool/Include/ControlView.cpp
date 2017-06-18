#include "stdafx.h"
#include "Tool.h"
#include "ControlView.h"

#include "Engine_Function.h"
#include "Export_System.h"
#include "MapEdit.h"


// CControlView

IMPLEMENT_DYNCREATE(CControlView, CFormView)

CControlView::CControlView()
: CFormView(CControlView::IDD)
, m_pWndShow(NULL)
, m_pMapEdit(NULL)
{

}

CControlView::~CControlView()
{
	Release();
}

void CControlView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tap);
}

BEGIN_MESSAGE_MAP(CControlView, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CControlView::OnTcnSelchangeTab1)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CControlView 진단입니다.
#ifdef _DEBUG
void CControlView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CControlView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

#endif
#endif //_DEBUG


// CControlView 메시지 처리기입니다.

void CControlView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CString strFirst  = _T("MAP");
	CString strSecond = _T("Camera");

	m_Tap.InsertItem(0, strFirst);
	m_Tap.InsertItem(1, strSecond);

	m_Tap.SetCurSel(0);

	CRect Rect;
	m_pMapEdit = new CMapEdit;
	m_pMapEdit->Create( IDD_MAPEDIT, &m_Tap );
	m_pMapEdit->GetWindowRect(&Rect);
	m_pMapEdit->SetWindowPos(NULL, 2, 26, Rect.Width(), Rect.Height(), SWP_SHOWWINDOW | SWP_NOZORDER);
	
	m_pMapEdit->ShowWindow(SW_SHOW);

	UpdateData(FALSE);
}

void CControlView::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(m_pWndShow != NULL)
	{
		m_pWndShow->ShowWindow(SW_HIDE);
		m_pWndShow = NULL;
	}

	int iSelect = m_Tap.GetCurSel();

	switch(iSelect)
	{
	case 0:
		m_pMapEdit->ShowWindow(SW_SHOW);
		m_pWndShow = m_pMapEdit;
		break;

	case 1:
		break;
	}

	*pResult = 0;
}

CMapEdit* CControlView::GetMapEdit(void)
{
	return m_pMapEdit;
}

void CControlView::Release(void)
{
	Engine::Safe_Delete(m_pMapEdit);
}