/*!
 * \file MainView.h
 * \date 2016/04/10 1:42
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

#ifndef MainView_h__
#define MainView_h__

class CToolDoc;
class CMainView : public CView
{
// Default
protected:
	CMainView();
	DECLARE_DYNCREATE(CMainView)

public:
	CToolDoc* GetDocument() const;

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDraw(CDC* pDC); 

protected:

public:
	virtual ~CMainView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

// Customized
public:
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // MainView.cpp의 디버그 버전
inline CToolDoc* CMainView::GetDocument() const
{ return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

#endif // MainView_h__

