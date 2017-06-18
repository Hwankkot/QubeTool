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
	/// MainApp ����
	static CMainApp* Create(void);

public:
	/// MainApp �ʱ�ȭ
	/// ����̽��� �ʱ�ȭ�ϰ�, Management�� �ʱ�ȭ �Ѵ�.
	/// Tool ���α׷��� �������̴�.
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