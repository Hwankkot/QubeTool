/*!
 * \file ControlView.h
 * \date 2016/04/10 2:31
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
#ifndef ControlView_h__
#define ControlView_h__
#include "afxcmn.h"
#include "Resource.h"

namespace Engine
{
	class CInput;
}

class CMapEdit;
class CControlView : public CFormView
{
	DECLARE_DYNCREATE(CControlView)

protected:
	CControlView();           
	virtual ~CControlView();

public:
	enum { IDD = IDD_CONTROLVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()

/// Customized
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	
public:
	CMapEdit*	GetMapEdit(void);

private:
	void Release(void);

private:
	CTabCtrl	m_Tap;
	CMapEdit*	m_pMapEdit;
	CWnd*       m_pWndShow;
};

#endif // ControlView_h__

