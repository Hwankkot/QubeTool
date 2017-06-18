/*!
 * \file Loading.h
 * \date 2016/04/16 11:09
 *
 * \author Architect
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Loading_h__
#define Loading_h__

class CLoading
{
private:
	CLoading(void);
	
	/// 복사 생성자 및 대입연산자로 생성을 막음
	explicit CLoading(const CLoading&);
	CLoading& operator = (const CLoading&);

public:
	~CLoading(void);

public:
	static CLoading* Create(void);

public:
	HRESULT InitLoading(void);

public:
	
	static UINT WINAPI LoadingFunction(void* pArg);

private:
	void LoadResources(void);

public:
	/// 현재 불러오고 있는 리소스가 어떤 종류인지를 적어놓은 로딩 메세지를 불러온다.
	const TCHAR* GetLoadingMessage(void);

	/// 로딩 완료 확인을 위한 함수.
	/// m_bComplete 변수에 저장된 true, false를 반환한다.
	bool GetComplete(void);

private:
	void Release(void);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	CRITICAL_SECTION		m_CSKey;
	HANDLE					m_hThread;

private:
	TCHAR					m_szLoadingMessage[128];
	bool					m_bComplete;
};
#endif // Loading_h__