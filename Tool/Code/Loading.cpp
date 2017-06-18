#include "stdafx.h"
#include "Loading.h"

#include <process.h> // for using Thread

#include "Cube.h"
#include "Engine_Enum.h"
#include "Engine_Function.h"
#include "Engine_Include.h"
#include "Enum.h"
#include "Export_Resources.h"
#include "Export_System.h"

CLoading::CLoading(void)
: m_pDevice(NULL)
, m_hThread(NULL)
, m_bComplete(false)
{
	ZeroMemory(&m_CSKey, sizeof(CRITICAL_SECTION));
	ZeroMemory(m_szLoadingMessage, sizeof(TCHAR) * 128);
}

CLoading::~CLoading(void)
{
	Release();
}

CLoading* CLoading::Create(void)
{
	CLoading* pLoad = new CLoading;
	
	if(FAILED(pLoad->InitLoading()))
		Engine::Safe_Delete(pLoad);

	return pLoad;
}

HRESULT CLoading::InitLoading(void)
{
	InitializeCriticalSection(&m_CSKey);

	m_pDevice = Engine::Get_GraphicDev()->GetDevice();
	NULL_CHECK_RETURN(m_pDevice, E_FAIL);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, LoadingFunction, this, 0, NULL);
	
	if(m_hThread == NULL)
		return E_FAIL;

	return S_OK;
}

UINT WINAPI CLoading::LoadingFunction(void* pArg)
{   
	CLoading*		pLoading = (CLoading*)pArg;

	/// 현재는 스레드가 1개 이기 때문에
	/// 크리티컬 섹션 키를 받아서 들어올 필요가 없다. 
	//EnterCriticalSection(&pLoading->m_CSKey);

	/// 포인터를 통해 함수를 호출
	pLoading->LoadResources();

	//LeaveCriticalSection(&pLoading->m_CSKey);

	return 0;
}

void CLoading::LoadResources(void)
{
	HRESULT hr = NULL;

	///  Loading Buffer
	lstrcpy(m_szLoadingMessage, L"Loading Buffer...");
	
	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pDevice
		, Engine::BUFFER_SPHERECOL
		, RESOURCEID::COLBUFFER_SPHERE
		, CCube::CUBERADIUS);
	FAILED_CHECK_RETURN_MSG(hr, , L"Sphere")

	/// Static Mesh
		
		hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, Engine::MESH_STATIC
		, RESOURCEID::STATICMESH_CUBE_WHITE
		, L"../../Data/Resources/Mesh/StaticMesh/WhiteCube/"
		, L"WhiteCube.X");		
	FAILED_CHECK_RETURN_MSG(hr, , L"STATICMESH_CUBE_WHITE")


		/*
		hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, Engine::MESH_STATIC
		, RESOURCEID::STATICMESH_CUBE_GREEN
		, L"../../Data/Resources/Mesh/StaticMesh/GreenCube/"
		, L"GreenCube.X");		
	FAILED_CHECK_RETURN_MSG(hr, , L"STATICMESH_CUBE_GREEN")

		hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, Engine::MESH_STATIC
		, RESOURCEID::STATICMESH_CUBE_PURPLE
		, L"../../Data/Resources/Mesh/StaticMesh/PurpleCube/"
		, L"PurpleCube.X");		
	FAILED_CHECK_RETURN_MSG(hr, , L"STATICMESH_CUBE_PURPLE")

		hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, Engine::MESH_STATIC
		, RESOURCEID::STATICMESH_CUBE_ORANGE
		, L"../../Data/Resources/Mesh/StaticMesh/OrangeCube/"
		, L"OrangeCube.X");		
	FAILED_CHECK_RETURN_MSG(hr, , L"STATICMESH_CUBE_ORANGE")


	lstrcpy(m_szLoadingMessage, L"Loading Static Mesh...");
*/
		hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, Engine::MESH_STATIC
		, RESOURCEID::STATICMESH_CUBE_BLUE
		, L"../../Data/Resources/Mesh/StaticMesh/BlueCube/"
		, L"BlueCube.X");		
	FAILED_CHECK_RETURN_MSG(hr, , L"STATICMESH_CUBE_BLUE")

		hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, Engine::MESH_STATIC
		, RESOURCEID::STATICMESH_CUBE_RED	
		, L"../../Data/Resources/Mesh/StaticMesh/RedCube/"
		, L"RedCube.X");		
	FAILED_CHECK_RETURN_MSG(hr, , L"STATICMESH_CUBE_RED")

		hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, Engine::MESH_STATIC
		, RESOURCEID::STATICMESH_CUBE_YELLOW
		, L"../../Data/Resources/Mesh/StaticMesh/YellowCube/"
		, L"YellowCube.X");		
		FAILED_CHECK_RETURN_MSG(hr, , L"STATICMESH_CUBE_YELLOW")

		hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, Engine::MESH_STATIC
		, RESOURCEID::STATICMESH_CUBE_WALL
		, L"../../Data/Resources/Mesh/StaticMesh/CubeWall/"
		, L"CubeWall.X");		
	FAILED_CHECK_RETURN_MSG(hr, , L"STATICMESH_CUBE_WALL")

	
		

	// Dynamic Mesh

	lstrcpy(m_szLoadingMessage, L"Loading Complete");

	m_bComplete = true;
}

const TCHAR* CLoading::GetLoadingMessage(void)
{
	return m_szLoadingMessage;
}

bool CLoading::GetComplete(void)
{
	return m_bComplete;
}

void CLoading::Release(void)
{
	DeleteCriticalSection(&m_CSKey);
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
}