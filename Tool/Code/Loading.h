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
	
	/// ���� ������ �� ���Կ����ڷ� ������ ����
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
	/// ���� �ҷ����� �ִ� ���ҽ��� � ���������� ������� �ε� �޼����� �ҷ��´�.
	const TCHAR* GetLoadingMessage(void);

	/// �ε� �Ϸ� Ȯ���� ���� �Լ�.
	/// m_bComplete ������ ����� true, false�� ��ȯ�Ѵ�.
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