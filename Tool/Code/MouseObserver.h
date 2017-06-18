/*!
 * \file MouseObserver.h
 * \date 2016/04/18 23:08
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
#ifndef MouseObserver_h__
#define MouseObserver_h__

#include "Observer.h"
#include "MouseSubject.h"

class CMouseObserver : public Engine::CObserver
{
private:
	CMouseObserver(void);

public:
	~CMouseObserver(void);

public:
	static CMouseObserver* Create(CMouseSubject* pMouseSubject);

private:
	HRESULT InitMouseObserver(CMouseSubject* pMouseSubject);

public:
	virtual void Update(const DWORD& dwMessage);
	void GetData(const DWORD& dwMessage, void* const pDataStorage, const DWORD& dwDataType, const DWORD& dwDataSize);

public:
	bool GetCheckDataChanged(const DWORD& dwMessage);

private:
	void Release(void);

private:
	CMouseSubject* m_pMouseSubject;

private:
	bool m_IsDataChanged[CMouseSubject::DATAMESSAGE_END];
	void* m_pData[CMouseSubject::DATAMESSAGE_END];
};

#endif // MouseObserver_h__