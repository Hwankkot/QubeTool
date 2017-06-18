/*!
 * \file MainApp.h
 * \date 2016/04/10 14:42
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
#ifndef MainApp_h__
#define MainApp_h__

namespace Engine
{
	class CFontMgr;
	class CGraphicDev;
	class CInput;
	class CManagement;
	class CTimeMgr;
}

class CControlView;
class CMapEdit;
class CMainApp
{
private:
	explicit CMainApp(void);
public:
	~CMainApp(void);

public:
	/// MainApp 생성
	static CMainApp* Create(void);

public:
	/// MainApp 초기화
	/// 디바이스를 초기화하고, Management를 초기화 한다.
	/// Tool 프로그램의 시작점이다.
	HRESULT InitMainApp(void);
	void Update(void);
	void Render(void);

private:
	void Release(void);

private:
	LPDIRECT3DDEVICE9  m_pDevice;

private:
	Engine::CFontMgr*		m_pFontMgr;
	Engine::CGraphicDev*	m_pGraphicDev;
	Engine::CInput*			m_pInput;
	Engine::CManagement*	m_pManagement;
	Engine::CTimeMgr*		m_pTimeMgr;
};

#endif // MainApp_h__