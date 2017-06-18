#include "stdafx.h"
#include "MapEditObserver.h"

#include "Engine_Function.h"
#include "MapEditSubject.h"

CMapEditObserver::CMapEditObserver(void)
: m_pMapEditSubject(NULL)
{

}

CMapEditObserver::~CMapEditObserver(void)
{
	Release();
}

CMapEditObserver* CMapEditObserver::Create(CMapEditSubject* pMapEditSubject)
{
	CMapEditObserver* pMouseObserver = new CMapEditObserver;

	if(FAILED(pMouseObserver->InitMapEditObserver(pMapEditSubject)))
		Engine::Safe_Delete(pMouseObserver);

	return pMouseObserver;
}


HRESULT CMapEditObserver::InitMapEditObserver(CMapEditSubject* pMapEditSubject)
{
	m_pMapEditSubject = pMapEditSubject;

	memset(m_IsDataChanged, true, sizeof(bool) * CMapEditSubject::DATAMESSAGE_END);

	ZeroMemory(m_pData, sizeof(void*) * CMapEditSubject::DATAMESSAGE_END);

	return S_OK;
}


void CMapEditObserver::Update(const DWORD& dwMessage)
{
	m_IsDataChanged[dwMessage] = true;
}

void CMapEditObserver::GetData(const DWORD& dwMessage, void* const pDataStorage
							 , const DWORD& dwDataType, const DWORD& dwDataSize)
{
	/// ������ ��������� ��������� �����Ҵ����� ���� ������ �����.
	if(m_pData[dwMessage] == NULL)
	{
		DWORD dwSize = 0;

		switch(dwDataType)
		{
		case CMapEditSubject::DATATYPE_BOOL:
			dwSize = DWORD(dwDataSize / sizeof(int));
			m_pData[dwMessage] = new int[dwSize];
			ZeroMemory(m_pData[dwMessage], dwDataSize);
			break;

		default:
			NULL_CHECK_RETURN_MSG(m_pData[dwMessage], , L"The datatype which user inputed is wrong.");
			break;
		}
	}

	/// ���� �����Ͱ� ������ �Ǿ������� true���� �̹Ƿ� �����͸� �ٽ� �޾ƿ´�.
	if(m_IsDataChanged[dwMessage] == true)
	{
		m_pMapEditSubject->GetData(dwMessage, m_pData[dwMessage], dwDataSize);
		m_IsDataChanged[dwMessage] = false;
	}

	memcpy(pDataStorage, m_pData[dwMessage], dwDataSize);
}

bool CMapEditObserver::GetCheckDataChanged(const DWORD& dwMessage)
{
	return m_IsDataChanged[dwMessage];
}

void CMapEditObserver::Release(void)
{
	/// ������ ���� ���� ����
	for(DWORD i = 0; i < CMapEditSubject::DATAMESSAGE_END; ++i)
		Engine::Safe_Delete(m_pData[i]);
}

