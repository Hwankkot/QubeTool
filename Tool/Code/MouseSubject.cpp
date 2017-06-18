#include "stdafx.h"
#include "MouseSubject.h"

#include "Engine_Function.h"
#include "Engine_Functor.h"

IMPLEMENT_SINGLETON(CMouseSubject)

CMouseSubject::CMouseSubject(void)
{

}

CMouseSubject::~CMouseSubject(void)
{
	Release();
}

void CMouseSubject::AddData(const DWORD& dwMessage, const DWORD& dwDataType
							, const DWORD& dwDataSize, const DWORD& dwContainerSize /* = 0 */)
{
	/// ���� ���� �޼����� ��ȯ
	DATAMESSAGE eDataMessage = DATAMESSAGE(dwMessage);

	/// Ű ������ �����Ͱ� �ʿ� �����ϴ��� ã�´�.
	DATAMAP::iterator mapiter = m_mapDataContainer.find(eDataMessage);

	/// �����Ͱ� �����ϸ� �����Ѵ�.
	if(mapiter != m_mapDataContainer.end())
	{
		MSG_BOX(L"The Data already exists");
		return ;
	}

	void* pData = NULL;
	DWORD dwSize = 0;

	switch(dwDataType)
	{
	case DATATYPE_INT:
		dwSize = DWORD(dwDataSize / sizeof(int));
		pData = new int[dwSize];
		ZeroMemory(pData, dwDataSize);
		break;

	case DATATYPE_FLOAT:
		dwSize = DWORD(dwDataSize / sizeof(float));
		pData = new float[dwSize];
		ZeroMemory(pData, dwDataSize);
		break;

	default:
		NULL_CHECK_RETURN_MSG(pData, , L"The datatype which user inputed is wrong.");
		break;
	}

	/// �߰��ϴ� �����͸� ������ ���������� ������ ����
	if(dwContainerSize != 0)
		m_ObserversList.reserve(dwContainerSize);

	/// ������ �������� ������ �ּҰ��� �޴´�.
	m_mapDataContainer.insert(DATAMAP::value_type(eDataMessage, pData));
}

void CMouseSubject::GetData(const DWORD& dwMessage, void* pDataStorage, const DWORD& dwDataSize)
{
	/// ���� ���� �޼����� ��ȯ
	DATAMESSAGE eDataMessage = DATAMESSAGE(dwMessage);

	/// Ű ������ �����Ͱ� �ʿ� �����ϴ��� ã�´�.
	DATAMAP::iterator mapiter = m_mapDataContainer.find(eDataMessage);

	/// �����Ͱ� ������ 
	if(mapiter == m_mapDataContainer.end())
	{
		MSG_BOX(L"The Data doesn't exists");
		return ;
	}

	memcpy(pDataStorage, mapiter->second, dwDataSize);
}

void CMouseSubject::NotifyMouseInfo(const DWORD& dwMessage, void* pDataSource, const DWORD& dwDataSize)
{
	/// ������ ��������� ���������
	/// ������ �����Ͱ� �߰��� �Ǿ� ���� ������ ���Ѵ�. (AddData���� �����Ѵ�.)
	if(m_mapDataContainer.find(dwMessage) == m_mapDataContainer.end())
	{
		MSG_BOX(L"The reference data doesn't exist");
		return ;
	}
	memcpy(m_mapDataContainer[dwMessage], pDataSource, dwDataSize);

	CSubject::Notify(dwMessage);
}

void CMouseSubject::Release(void)
{
	DATAMAP::iterator mapiter = m_mapDataContainer.begin();
	DATAMAP::iterator mapiter_end = m_mapDataContainer.end();

	for(; mapiter != mapiter_end; ++mapiter)
	{
		Engine::Safe_Delete(mapiter->second);
	}
}
