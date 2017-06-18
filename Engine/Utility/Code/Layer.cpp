#include "Layer.h"
#include "GameObject.h"

#include "Engine_Function.h"
#include "Engine_Functor.h"
#include "Engine_Macro.h"
#include "GameObject.h"

Engine::CLayer::CLayer(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
, m_IsReset(false)
{

}

Engine::CLayer::~CLayer(void)
{
	Release();
}

Engine::CLayer* Engine::CLayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	return new CLayer(pDevice);
}

void Engine::CLayer::Update(void)
{
	MAPOBJECTLIST::iterator	mapiter		= m_mapObjectlist.begin();
	MAPOBJECTLIST::iterator	mapiter_end = m_mapObjectlist.end();

	for (; mapiter != mapiter_end; ++mapiter)
	{
		size_t iSize = mapiter->second.size();

		OBJECTLIST& Objectlist = mapiter->second;

		for(size_t i = 0; i < iSize; ++i)
		{
			Objectlist[i]->Update();
		}
	}	
}

void Engine::CLayer::Render(void)
{
	MAPOBJECTLIST::iterator	mapiter		= m_mapObjectlist.begin();
	MAPOBJECTLIST::iterator	mapiter_end = m_mapObjectlist.end();

	for (; mapiter != mapiter_end; ++mapiter)
	{
		OBJECTLIST::iterator	iterlist	 = mapiter->second.begin();
		OBJECTLIST::iterator	iterlist_end = mapiter->second.end();

		for (; iterlist != iterlist_end; ++iterlist)		
		{
			(*iterlist)->Render();
		}		
	}	
}

void Engine::CLayer::Release(void)
{
	MAPOBJECTLIST::iterator	mapiter		= m_mapObjectlist.begin();
	MAPOBJECTLIST::iterator	mapiter_end = m_mapObjectlist.end();

	for (; mapiter != mapiter_end; ++mapiter)
	{
		OBJECTLIST::iterator	iterlist	 = mapiter->second.begin();
		OBJECTLIST::iterator	iterlist_end = mapiter->second.end();

		for (; iterlist != iterlist_end; ++iterlist)		
			Engine::Safe_Delete(*iterlist);

		mapiter->second.clear();
	}
	m_mapObjectlist.clear();
}

HRESULT Engine::CLayer::AddObject(const DWORD& dwObjectKey, CGameObject* pGameObject)
{
	if(pGameObject)
	{
		if(NULL == m_mapObjectlist.count(dwObjectKey))
		{ 
			m_mapObjectlist[dwObjectKey] = OBJECTLIST();
			m_mapObjectlist[dwObjectKey].reserve(100);
		}
		m_mapObjectlist[dwObjectKey].push_back(pGameObject);
	}

	return S_OK;
}

HRESULT Engine::CLayer::DeleteObJect(const DWORD& dwObjectKey, CGameObject* pGameObject)
{
	if(pGameObject)
	{
		if(NULL == m_mapObjectlist.count(dwObjectKey))
			NULL_CHECK_RETURN_MSG(NULL, E_FAIL, L"The object storage doesn't exist.");

		OBJECTLIST::iterator	iterlist	 = m_mapObjectlist[dwObjectKey].begin();
		OBJECTLIST::iterator	iterlist_end = m_mapObjectlist[dwObjectKey].end();
		for(; iterlist != iterlist_end;)
		{
			if( (*iterlist) == pGameObject )
			{
				Safe_Delete(*iterlist);
				m_mapObjectlist[dwObjectKey].erase(iterlist);
				break;
			}
			else
				++iterlist;
		}
	}

	return S_OK;
}

std::map<DWORD, std::vector<Engine::CGameObject*>>& Engine::CLayer::GetMapObjectlist(void)
{
	return m_mapObjectlist;
}

