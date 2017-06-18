/*!
 * \file MainFrm.h
 * \date 2016/04/10 1:38
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

#ifndef MainFrm_h__
#define MainFrm_h__

class CSplitterX : public CSplitterWnd
{
public:
	CSplitterX() ;
	~CSplitterX() ;
protected:
	afx_msg LRESULT OnNcHitTest(CPoint point) ;
	DECLARE_MESSAGE_MAP() ;
};

class CMainApp;
class CMainView;
class CControlView;
class CMainFrame : public CFrameWnd
{
// Default
protected: 
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

// Customized
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

public:
	CMainApp* GetMainApp(void);
	CMainView*	GetMainView(void);
	CControlView* GetControlView(void);

private:
	void Release(void);

private:
	CSplitterX		m_MainSplitter;

private:
	CMainApp*		m_pMainApp;

private:
	CMainView*		m_pMainView;
	CControlView*	m_pControlView;


};

#endif // MainFrm_h__




