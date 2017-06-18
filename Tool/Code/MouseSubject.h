/*!
 * \file MouseSubject.h
 * \date 2016/04/18 22:41
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
#ifndef MouseSubject_h__
#define MouseSubject_h__

#include "Engine_Macro.h"
#include "Subject.h"

class CMouseSubject : public Engine::CSubject
{
	DECLARE_SINGLETON(CMouseSubject)

public:
	enum DATATYPE {	DATATYPE_INT, DATATYPE_FLOAT };
	enum DATAMESSAGE { DATAMESSAGE_RAY, DATAMESSAGE_END};

private:
	CMouseSubject(void);
	~CMouseSubject(void);

public:
	virtual void AddData(const DWORD& dwMessage, const DWORD& dwDataType, const DWORD& dwDataSize, const DWORD& dwContainerSize = 0);
	void GetData(const DWORD& dwMessage, void* pDataStorage, const DWORD& dwDataSize);
	void NotifyMouseInfo(const DWORD& dwMessage, void* pDataSource, const DWORD& dwDataSize);

private:
	void Release(void);

private:
	typedef std::map<DWORD, void*> DATAMAP;
	DATAMAP	m_mapDataContainer;
};


#endif // MouseSubject_h__