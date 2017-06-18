#include "stdafx.h"
#include "MouseObserver.h"

#include "Engine_Function.h"
#include "MouseSubject.h"

CMouseObserver::CMouseObserver(void)
: m_pMouseSubject(NULL)
{

}

CMouseObserver::~CMouseObserver(void)
{
	Release();
}

CMouseObserver* CMouseObserver::Create(CMouseSubject* pMouseSubject)
{
	CMouseObserver* pMouseObserver = new CMouseObserver;

	if(FAILED(pMouseObserver->InitMouseObserver(pMouseSubject)))
		Engine::Safe_Delete(pMouseObserver);

	return pMouseObserver;
}

HRESULT CMouseObserver::InitMouseObserver(CMouseSubject* pMouseSubject)
{
	m_pMouseSubject = pMouseSubject;

	ZeroMemory(m_IsDataChanged, sizeof(bool) * CMouseSubject::DATAMESSAGE_END);
	ZeroMemory(m_pData, sizeof(void*) * CMouseSubject::DATAMESSAGE_END);
	
	return S_OK;
}

void CMouseObserver::Update(const DWORD& dwMessage)
{
	m_IsDataChanged[dwMessage] = true;
}

void CMouseObserver::GetData(const DWORD& dwMessage, void* const pDataStorage
							, const DWORD& dwDataType, const DWORD& dwDataSize)
{
	/// 데이터 저장공간이 비어있으면 동적할당으로 저장 공간을 만든다.
	if(m_pData[dwMessage] == NULL)
	{
		DWORD dwSize = 0;

		switch(dwDataType)
		{
		case CMouseSubject::DATATYPE_INT:
			dwSize = DWORD(dwDataSize / sizeof(int));
			m_pData[dwMessage] = new int[dwSize];
			ZeroMemory(m_pData[dwMessage], dwDataSize);
			break;

		case CMouseSubject::DATATYPE_FLOAT:
			dwSize = DWORD(dwDataSize / sizeof(float));
			m_pData[dwMessage] = new float[dwSize];
			ZeroMemory(m_pData[dwMessage], dwDataSize);
			break;

		default:
			NULL_CHECK_RETURN_MSG(m_pData[dwMessage], , L"The datatype which user inputed is wrong.");
			break;
		}
	}

	/// 만약 데이터가 변경이 되어있으면 true상태 이므로 데이터를 다시 받아온다.
	if(m_IsDataChanged[dwMessage] == true)
	{
		m_pMouseSubject->GetData(dwMessage, m_pData[dwMessage], dwDataSize);
		m_IsDataChanged[dwMessage] = false;
	}

	memcpy(pDataStorage, m_pData[dwMessage], dwDataSize);
}

bool CMouseObserver::GetCheckDataChanged(const DWORD& dwMessage)
{
	return m_IsDataChanged[dwMessage];
}

void CMouseObserver::Release(void)
{
	/// 데이터 저장 공간 삭제
	for(DWORD i = 0; i < CMouseSubject::DATAMESSAGE_END; ++i)
		Engine::Safe_Delete(m_pData[i]);
}

