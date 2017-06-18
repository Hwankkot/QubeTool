#include "Subject.h"

#include "Engine_Function.h"
#include "Engine_Functor.h"
#include "Observer.h"

Engine::CSubject::CSubject(void)
{

}

Engine::CSubject::~CSubject(void)
{
	Release();
}

void Engine::CSubject::Subscribe(CObserver* pObserver)
{
	/// ������ �����Ͱ� �ִ��� Ȯ��
	NULL_CHECK_MSG(pObserver, L"The Observer doesn't exist");

	/// �ε����� �������� ���� ��ġ�� ã�� ���ؼ� ���δ�.
	/// �� �������� �ε����� �߰��Ǳ� ���� ���� ������� �ڵ� �Էµȴ�.
	pObserver->SetIndex(m_ObserversList.size());

	/// �������� �߰��Ѵ�.
	m_ObserversList.push_back(pObserver);
}

void Engine::CSubject::Unsubscribe(const DWORD& dwIndex, const CObserver* pObserver)
{
	/// ���� �ε����� �����ϴ� �������� �Է¹��� �������� ������ Ȯ��
	if(m_ObserversList[dwIndex] != pObserver)
		FAILED_CHECK_RETURN_MSG(E_FAIL, , L"The index is not matched");

	/// ������ �������� ��ġ�� ����
	DWORD dwLastIndex = m_ObserversList.size() - 1;

	/// �����Ϸ��� ��ġ�� ������ �������� ����
	m_ObserversList[dwIndex] = m_ObserversList[dwLastIndex];
	
	/// �ε��� ����
	m_ObserversList[dwIndex]->SetIndex(dwIndex);

	/// ������ �������� �ִ� ����Ҹ� ����
	m_ObserversList.pop_back();
}

void Engine::CSubject::Notify(const DWORD& dwMessage)
{
	/// Push �������� �����ϴ� ���α׷� ����
	/// ���� Pull �������� �����ϰ� ������ ����� �޾Ƽ� �ڵ带 �����ϸ� �ȴ�.
	/// �ƴϸ� Observer�� ���ŵ� �� � ���� ����Ǿ��� ���� üũ�� �ؼ�
	/// ����� ���� ���� ��� Subject���� ���� ���������� �ڵ��� �� �ִ�.
	OBSERVERLIST::iterator	iterlist  = m_ObserversList.begin();
	OBSERVERLIST::iterator	iterlist_end = m_ObserversList.end();

	for(; iterlist != iterlist_end; ++iterlist)
	{
		(*iterlist)->Update(dwMessage);
	}
}


void Engine::CSubject::Release(void)
{
	m_ObserversList.clear();
	m_ObserversList.swap(OBSERVERLIST());
}

