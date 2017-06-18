/*!
 * \file MapEditSubject.h
 * \date 2016/04/28 21:43
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

#ifndef MapEditSubject_h__
#define MapEditSubject_h__

#include "Engine_Macro.h"
#include "Subject.h"

class CMapEditSubject : public Engine::CSubject
{
	DECLARE_SINGLETON(CMapEditSubject)

public:
	enum DATATYPE {	DATATYPE_BOOL };
	enum DATAMESSAGE { DATAMESSAGE_ADD, DATAMESSAGE_EDIT, DATAMESSAGE_RECORDNAVCELL, DATAMESSAGE_EDITNAVCELL, DATAMESSAGE_REVISENAVCELL, DATAMESSAGE_END};

private:
	CMapEditSubject(void);
	~CMapEditSubject(void);

public:
	virtual void AddData(const DWORD& dwMessage, const DWORD& dwDataType, const DWORD& dwDataSize, const DWORD& dwContainerSize = 0);
	void GetData(const DWORD& dwMessage, void* pDataStorage, const DWORD& dwDataSize);
	void NotifyMapEditInfo(const DWORD& dwMessage, void* pDataSource, const DWORD& dwDataSize);

private:
	void Release(void);

private:
	typedef std::map<DWORD, void*> DATAMAP;
	DATAMAP	m_mapDataContainer;
};

#endif // MapEditSubject_h__
