/*!
 * \file MapEditObserver.h
 * \date 2016/04/28 21:47
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
#ifndef MapEditObserver_h__
#define MapEditObserver_h__

#include "Observer.h"
#include "MapEditSubject.h"

class CMapEditObserver : public Engine::CObserver
{
private:
	CMapEditObserver(void);

private:
	explicit CMapEditObserver(const CMapEditObserver&);
	CMapEditObserver& operator = (const CMapEditObserver&);

public:
	~CMapEditObserver(void);

public:
	static CMapEditObserver* Create(CMapEditSubject* pMapEditSubject);

private:
	HRESULT InitMapEditObserver(CMapEditSubject* pMapEditSubject);

public:
	virtual void Update(const DWORD& dwMessage);
	void GetData(const DWORD& dwMessage, void* const pDataStorage
		, const DWORD& dwDataType, const DWORD& dwDataSize);

public:
	bool GetCheckDataChanged(const DWORD& dwMessage);

private:
	void Release(void);

private:
	CMapEditSubject* m_pMapEditSubject;

private:
	bool m_IsDataChanged[CMapEditSubject::DATAMESSAGE_END];
	void* m_pData[CMapEditSubject::DATAMESSAGE_END];
};

#endif // MapEditObserver_h__
