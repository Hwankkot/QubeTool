#include "stdafx.h"
#include "InputUnit.h"

#include "Engine_Function.h"
#include "Enum.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Layer.h"
#include "MapEditObserver.h"
#include "MapEditSubject.h"
#include "MouseSubject.h"
#include "NavCell.h"
#include "Transform.h"
#include "Value.h"

// Cube
#include "WhiteCube.h"
#include "GreenCube.h"
#include "PurpleCube.h"
#include "OrangeCube.h"
#include "BlueCube.h"
#include "RedCube.h"
#include "YellowCube.h"
#include "GrayCube.h"
#include "CubeWall.h"

CInputUnit::CInputUnit(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pInput(NULL)
, m_pFont(NULL)
, m_pLayer(NULL)
, m_pNavMgr(NULL)
, m_pRenderer(NULL)
, m_pMapEditObserver(NULL)
, m_pMouseSubject(NULL)
, m_IsFixingMouse(true)
, m_ptCenterPos(0, 0)
, m_ptMousePos(0, 0)
, m_pCubeCache(NULL)
, m_IsCreatingCube(false)
, m_IsDeletingCube(false)
, m_wCubeEdge(-1)
, m_dwCubeType(-1)
, m_vIntersectedPosCache(0.f, 0.f, 0.f)
, m_IsRecordingNavCell(false)
, m_IsDeletingNavCell(false)
, m_wNavCount(0)
, m_dwNavIndexCache(-1)
, m_dwBaseNavCellIndex(-1)
, m_dwFixingNavCellIndex(-1)
{
	ZeroMemory(&m_Ray, sizeof(Engine::RAY3));
	ZeroMemory(m_szMessage, sizeof(TCHAR) * 128);
	ZeroMemory(m_vNavPoints, sizeof(D3DXVECTOR3) * 3);
}

CInputUnit::~CInputUnit(void)
{
	Release();
}

CInputUnit* CInputUnit::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CInputUnit* pMouse = new CInputUnit(pDevice);

	if(FAILED(pMouse->InitInputUnit()))
		Engine::Safe_Delete(pMouse);

	return pMouse;
}

HRESULT CInputUnit::InitInputUnit(void)
{
	m_pInput = Engine::Get_Input();
	m_pFont  = Engine::Get_FontMgr();
	m_pNavMgr = Engine::Get_NavMgr();
	m_pRenderer = Engine::Get_Management()->GetRenderer();

	m_pMapEditObserver = CMapEditObserver::Create(CMapEditSubject::GetInstance());
	CMapEditSubject::GetInstance()->Subscribe(m_pMapEditObserver);
	m_pMouseSubject = CMouseSubject::GetInstance();

	CPoint ptMouse(WINCX >> 1, WINCY >> 1); // 마우스 위치 초기화
	
	m_ptMousePos = ptMouse; // 초기 위치 설정

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);

	m_ptCenterPos = ptMouse; // 중앙 위치 저장

	// Subject에 Ray정보 입력
	m_pMouseSubject->AddData(CMouseSubject::DATAMESSAGE_RAY, CMouseSubject::DATATYPE_FLOAT, sizeof(Engine::RAY3), 300);

	return S_OK;
}

void CInputUnit::Update(void)
{
	if(m_pInput->GetDIMouseState(Engine::CInput::DIM_RBUTTON) & 0x80) // 마우스 포지션 리셋
		ResetMousePos(); 
	
	else // 마우스 위치 업데이트
		UpdateMousePos(); 

	// 마우스와 오브젝트 상호작용 업데이트
	UpdateObjectInteraction();
	
	CGameObject::Update();
}

void CInputUnit::Render(void)
{
	long lX = 0;
	long lY = 0;

	lX = m_ptMousePos.x;
	lY = m_ptMousePos.y;
	wsprintf(m_szMessage, L"Mouse ( %d, %d )", lX, lY); // 마우스 좌표
	m_pFont->Render(L"Cambria", D3DXVECTOR3(0.f, 12.f, 0.f), m_szMessage, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	lX = m_pRenderer->GetGameObjectlist(Engine::RENDER_ALPHA).size();
	wsprintf(m_szMessage, L"Cube Count: %d", lX); // 큐브 개수
	m_pFont->Render(L"Cambria", D3DXVECTOR3(180.f, 12.f, 0.f), m_szMessage, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	lX = m_wNavCount;
	wsprintf(m_szMessage, L"NaviMeshCount: %d", lX); // NaviMeshCount
	m_pFont->Render(L"Cambria", D3DXVECTOR3(340.f, 12.f, 0.f), m_szMessage, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	if( m_pCubeCache != NULL )
	{
		float fX;
		float fY;
		float fZ;

		D3DXVECTOR3 vPos = m_pCubeCache->GetCubeData()->m_vPos;
		fX = vPos.x;
		fY = vPos.y;
		fZ = vPos.z;

		_stprintf_s(m_szMessage, L"Selected Cube Pos: %.f, %.f, %.f", fX, fY, fZ); // 선택된 큐브 위치
		m_pFont->Render(L"Cambria", D3DXVECTOR3(0.f, 24.f, 0.f), m_szMessage, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		float* pAngle = m_pCubeCache->GetCubeData()->m_fAngle;
		fX = D3DXToDegree(pAngle[Engine::ANGLE_X]);
		fY = D3DXToDegree(pAngle[Engine::ANGLE_Y]);
		fZ = D3DXToDegree(pAngle[Engine::ANGLE_Z]);

		_stprintf_s(m_szMessage, L"Selected Cube Angle: %.f, %.f, %.f", fX, fY, fZ); // 선택된 큐브 앵글
		m_pFont->Render(L"Cambria", D3DXVECTOR3(0.f, 36.f, 0.f), m_szMessage, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}

	if(m_dwBaseNavCellIndex != -1)
	{
		wsprintf(m_szMessage, L"Base NavMesh is Active"); 
		m_pFont->Render(L"Cambria", D3DXVECTOR3(0.f, 48.f, 0.f), m_szMessage, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}

	if(m_dwFixingNavCellIndex != -1)
	{
		wsprintf(m_szMessage, L"Fixing NavMesh is Active"); 
		m_pFont->Render(L"Cambria", D3DXVECTOR3(0.f, 60.f, 0.f), m_szMessage, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	
	m_pNavMgr->RenderNavMesh();
}

void CInputUnit::Release(void)
{
	CMapEditSubject::GetInstance()->Unsubscribe(m_pMapEditObserver->GetIndex(), m_pMapEditObserver);
	Engine::Safe_Delete(m_pMapEditObserver);
}

void CInputUnit::SetCubeLayer(Engine::CLayer* pLayer)
{
	m_pLayer = pLayer;
}

void CInputUnit::UpdateMousePos(void)
{	
	if( !m_IsFixingMouse )
	{
		ShowCursor(true);
		SetCursorPos(m_ptCenterPos.x, m_ptCenterPos.y);
		m_IsFixingMouse = !m_IsFixingMouse;
	}

	m_ptMousePos.x += m_pInput->GetDIMouseMovement(Engine::CInput::DIM_X);
	m_ptMousePos.y += m_pInput->GetDIMouseMovement(Engine::CInput::DIM_Y);
}

void CInputUnit::ResetMousePos(void)
{
	if( m_IsFixingMouse )
	{
		CPoint ptMouse(WINCX >> 1, WINCY >> 1);
		ClientToScreen(g_hWnd, &ptMouse);
		SetCursorPos(ptMouse.x, ptMouse.y);

		m_ptCenterPos = ptMouse;

		m_ptMousePos = CPoint(WINCX >> 1, WINCY >> 1);
		m_IsFixingMouse = !m_IsFixingMouse;
		ShowCursor(false);
	} 
}

void CInputUnit::UpdateObjectInteraction(void)
{
	if(m_IsCreatingCube)
	{
		if (m_pCubeCache == NULL)
			m_IsCreatingCube = false;
		
		else
			CreateCube();
	}

	if(m_IsDeletingCube)
	{
		DeleteCube();
	}

	if(m_IsDeletingNavCell)
	{
		DeleteNavCell();
	}
	
	// 화면을 벗어난 후의 마우스 메세지를 처리하지 않는다.
	if(m_ptMousePos.x < 0 || m_ptMousePos.x > WINCX || m_ptMousePos.y < 0 || m_ptMousePos.y > WINCY)
		return ;
	else
	{
		if(m_pInput->KeyDown(Engine::CInput::KEY_LBUTTON))
		{ 
			// 현재 마우스가 발사한 Ray를 계산한다.
			TranslationMousePosToWorld(); 	
						
			// NavCell을 추가하는지 확인한다.
			BOOL IsRecordNavCell;
			m_pMapEditObserver->GetData(CMapEditSubject::DATAMESSAGE_RECORDNAVCELL
				, &IsRecordNavCell, CMapEditSubject::DATATYPE_BOOL, sizeof(BOOL));

			// NavCell를 수정하는지 확인한다.
			BOOL IsEditNavCell;
			m_pMapEditObserver->GetData(CMapEditSubject::DATAMESSAGE_EDITNAVCELL
				, &IsEditNavCell, CMapEditSubject::DATATYPE_BOOL, sizeof(BOOL));

			// NavCell를 보정하는지 확인한다.
			BOOL IsReviseNavCell;
			m_pMapEditObserver->GetData(CMapEditSubject::DATAMESSAGE_REVISENAVCELL
				, &IsReviseNavCell, CMapEditSubject::DATATYPE_BOOL, sizeof(BOOL));

			if(IsRecordNavCell)
			{
				m_IsRecordingNavCell = true;
				RecordNavMesh();
			}
			else if(IsEditNavCell)
			{
				if(IsReviseNavCell)
					ReviseNavCell();
				else
					SelectNavCell();
			}
			else
			{
				SelectObject();
				
				if(m_IsRecordingNavCell)
				{
					m_IsRecordingNavCell = !m_IsRecordingNavCell;
					ResetNavCellCache();
				}
			}
		} // m_pInput->KeyDown(Engine::CInput::KEY_LBUTTON)
	} // m_ptMousePos.x < 0 || m_ptMousePos.x > WINCX || m_ptMousePos.y < 0 || m_ptMousePos.y > WINCY
}

void CInputUnit::SelectObject(void)
{
	// Z버퍼로 정렬한 오브젝트 포인터 리스트 가지고 오기
	typedef std::vector<CGameObject*> OBJECTLIST;
	OBJECTLIST Objectlist = m_pRenderer->GetGameObjectlist(Engine::RENDER_ALPHA);

	size_t iSize = Objectlist.size();

	if(iSize == 0)
		return;

	CCube* pCube = NULL;
	D3DXVECTOR3 vPos;
	WORD wEdge = 6;

	// Z버퍼는 큰 순서대로 정렬되기 때문에 반대로 루프한다.
	size_t iCubeIndex = iSize - 1;
	for(; iCubeIndex != -1; --iCubeIndex)
	{
		pCube = static_cast<CCube*>(Objectlist[iCubeIndex]);

		if(pCube->IsIntersectedtoSelect(&vPos, &wEdge))
			break;
	}

	if(m_pCubeCache != NULL && m_pCubeCache != pCube)
		m_pCubeCache->ReleaseSelect();

	if(iCubeIndex == -1)
	{
		pCube = NULL;
		ResetCubeCache();
	}
	else
	{
		m_dwCubeType = pCube->GetCubeType();
		m_wCubeEdge = wEdge;
		m_vIntersectedPosCache = vPos;
		m_pCubeCache = pCube;
	}
}

void CInputUnit::TranslationMousePosToWorld(void)
{
	CPoint MousePos = m_ptMousePos;
 
	D3DXMATRIX matProj;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXVECTOR3 vSrc;
	vSrc.x = (float(MousePos.x)  / (WINCX >> 1) - 1.f) / matProj._11;
	vSrc.y = (float(-MousePos.y) / (WINCY >> 1) + 1.f) / matProj._22;
	vSrc.z = 1.f;

	m_Ray.pos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_Ray.dir = vSrc - m_Ray.pos;
	D3DXVec3Normalize(&m_Ray.dir ,&m_Ray.dir);

	D3DXMATRIX		matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&m_Ray.pos, &m_Ray.pos, &matView);
	D3DXVec3TransformNormal(&m_Ray.dir, &m_Ray.dir, &matView);

	m_pMouseSubject->NotifyMouseInfo(CMouseSubject::DATAMESSAGE_RAY, (void*)&m_Ray, sizeof(Engine::RAY3));
}

void CInputUnit::SetCreateCube(BOOL bCubeType)
{
	m_dwCubeType = bCubeType;
	m_IsCreatingCube = true;
}

void CInputUnit::SetDeleteCube(void)
{
	m_IsDeletingCube = true;
}

void CInputUnit::CreateCube(void)
{
	CCube* pCube = NULL;
	float fCubeRange = 0.f;

	switch(m_dwCubeType)
	{
	case GAMEOBJECTID::CUBE_WHITE:
		pCube = CWhiteCube::Create(m_pDevice, m_pLayer, GAMEOBJECTID::CUBE_WHITE);
		m_pLayer->AddObject(GAMEOBJECTID::CUBE_WHITE, pCube);
		fCubeRange = CCube::CUBERADIUS;
		break;

	case GAMEOBJECTID::CUBE_GREEN:
		pCube = CGreenCube::Create(m_pDevice, m_pLayer, GAMEOBJECTID::CUBE_GREEN);
		m_pLayer->AddObject(GAMEOBJECTID::CUBE_GREEN, pCube);
		fCubeRange = CCube::CUBERADIUS;
		break;
		break;

	case GAMEOBJECTID::CUBE_PURPLE:
		pCube = CPurpleCube::Create(m_pDevice, m_pLayer, GAMEOBJECTID::CUBE_PURPLE);
		m_pLayer->AddObject(GAMEOBJECTID::CUBE_PURPLE, pCube);
		fCubeRange = CCube::CUBERADIUS;
		break;

	case GAMEOBJECTID::CUBE_ORANGE:
		pCube = COrangeCube::Create(m_pDevice, m_pLayer, GAMEOBJECTID::CUBE_ORANGE);
		m_pLayer->AddObject(GAMEOBJECTID::CUBE_ORANGE, pCube);
		fCubeRange = CCube::CUBERADIUS;
		break;

	case GAMEOBJECTID::CUBE_BLUE:
		pCube = CBlueCube::Create(m_pDevice, m_pLayer, GAMEOBJECTID::CUBE_BLUE);
		m_pLayer->AddObject(GAMEOBJECTID::CUBE_BLUE, pCube);
		fCubeRange = CCube::CUBERADIUS;
		break;

	case GAMEOBJECTID::CUBE_RED:
		pCube = CRedCube::Create(m_pDevice, m_pLayer, GAMEOBJECTID::CUBE_RED);
		m_pLayer->AddObject(GAMEOBJECTID::CUBE_RED, pCube);
		fCubeRange = CCube::CUBERADIUS;
		break;

	case GAMEOBJECTID::CUBE_YELLOW:
		pCube = CYellowCube::Create(m_pDevice, m_pLayer, GAMEOBJECTID::CUBE_YELLOW);
		m_pLayer->AddObject(GAMEOBJECTID::CUBE_YELLOW, pCube);
		fCubeRange = CCube::CUBERADIUS;
		break;

	case GAMEOBJECTID::CUBE_GRAY:
		pCube = CRedCube::Create(m_pDevice, m_pLayer, GAMEOBJECTID::CUBE_GRAY);
		m_pLayer->AddObject(GAMEOBJECTID::CUBE_GRAY, pCube);
		fCubeRange = CCube::CUBERADIUS;
		break;

	case GAMEOBJECTID::CUBE_WALL:
		pCube = CCubeWall::Create(m_pDevice, m_pLayer, GAMEOBJECTID::CUBE_WALL);
		m_pLayer->AddObject(GAMEOBJECTID::CUBE_WALL, pCube);
		fCubeRange = CWhiteCube::CUBERADIUS;
		break;
	}

	fCubeRange = CCube::CUBERADIUS;
	
	switch(m_wCubeEdge)
	{
	case CCube::PLANE_TOP:
		pCube->GetCubeData()->m_vPos = m_pCubeCache->GetCubeData()->m_vPos;
		pCube->GetCubeData()->m_vPos.y += fCubeRange;
		break;

	case CCube::PLANE_BOT:
		pCube->GetCubeData()->m_vPos = m_pCubeCache->GetCubeData()->m_vPos;
		pCube->GetCubeData()->m_vPos.y -= fCubeRange;
		break;

	case CCube::PLANE_LEFT:
		pCube->GetCubeData()->m_vPos = m_pCubeCache->GetCubeData()->m_vPos;
		pCube->GetCubeData()->m_vPos.x -= fCubeRange;
		break;

	case CCube::PLANE_RIGHT:
		pCube->GetCubeData()->m_vPos = m_pCubeCache->GetCubeData()->m_vPos;
		pCube->GetCubeData()->m_vPos.x += fCubeRange;
		break;

	case CCube::PLANE_FRONT:
		pCube->GetCubeData()->m_vPos = m_pCubeCache->GetCubeData()->m_vPos;
		pCube->GetCubeData()->m_vPos.z -= fCubeRange;
		break;

	case CCube::PLANE_BACK:
		pCube->GetCubeData()->m_vPos = m_pCubeCache->GetCubeData()->m_vPos;
		pCube->GetCubeData()->m_vPos.z += fCubeRange;
		break;
	}

	m_pRenderer->AddRenderObject(Engine::RENDER_ALPHA, pCube);

	m_IsCreatingCube = false;
}


void CInputUnit::DeleteCube(void)
{
	if(m_pCubeCache == NULL)
	{
		m_pCubeCache = false;
		m_IsDeletingCube = false;
		return ;
	}

	if(m_pRenderer->GetGameObjectlist(Engine::RENDER_ALPHA).size() == 1)
	{
		m_IsDeletingCube = false;
		return ;
	}

	m_pRenderer->DeleteRenderObject(Engine::RENDER_ALPHA, m_pCubeCache);

	switch(m_dwCubeType)
	{
	case GAMEOBJECTID::CUBE_WHITE:
		m_pLayer->DeleteObJect(GAMEOBJECTID::CUBE_WHITE, m_pCubeCache);
		break;

	case GAMEOBJECTID::CUBE_GREEN:
		m_pLayer->DeleteObJect(GAMEOBJECTID::CUBE_GREEN, m_pCubeCache);
		break;

	case GAMEOBJECTID::CUBE_PURPLE:
		m_pLayer->DeleteObJect(GAMEOBJECTID::CUBE_PURPLE, m_pCubeCache);
		break;

	case GAMEOBJECTID::CUBE_ORANGE:
		m_pLayer->DeleteObJect(GAMEOBJECTID::CUBE_ORANGE, m_pCubeCache);
		break;

	case GAMEOBJECTID::CUBE_BLUE:
		m_pLayer->DeleteObJect(GAMEOBJECTID::CUBE_BLUE, m_pCubeCache);
		break;

	case GAMEOBJECTID::CUBE_RED:
		m_pLayer->DeleteObJect(GAMEOBJECTID::CUBE_RED, m_pCubeCache);
		break;

	case GAMEOBJECTID::CUBE_YELLOW:
		m_pLayer->DeleteObJect(GAMEOBJECTID::CUBE_YELLOW, m_pCubeCache);
		break;

	case GAMEOBJECTID::CUBE_GRAY:
		m_pLayer->DeleteObJect(GAMEOBJECTID::CUBE_GRAY, m_pCubeCache);
		break;

	case GAMEOBJECTID::CUBE_WALL:
		m_pLayer->DeleteObJect(GAMEOBJECTID::CUBE_WALL, m_pCubeCache);
		break;
	}

	ResetCubeCache();
	m_IsDeletingCube = false;
}

void CInputUnit::ResetCubeCache(void)
{
	m_dwCubeType = -1;
	m_wCubeEdge = -1;
	m_vIntersectedPosCache = D3DXVECTOR3(FLOATMAX, FLOATMAX, FLOATMAX);
	m_pCubeCache = NULL;
}

void CInputUnit::RecordNavMesh(void)
{
	typedef std::vector<CGameObject*> OBJECTLIST;
	OBJECTLIST Objectlist = m_pRenderer->GetGameObjectlist(Engine::RENDER_ALPHA);

	size_t iSize = Objectlist.size();

	// 오브젝트가 없으면 종료한다.
	if(iSize == 0)
		return;

	CCube* pCube = NULL;
	bool m_IsIntersected = false;
	D3DXVECTOR3 vPos(0.f, 0.f, 0.f);

	// Z버퍼는 큰 순서대로 정렬되기 때문에 반대로 루프한다.
	size_t iCubeIndex = iSize - 1;
	for(; iCubeIndex != -1; --iCubeIndex)
	{
		pCube = static_cast<CCube*>(Objectlist[iCubeIndex]);
		
		//벽 오브젝트 위의 NaviMesh를 받아온다. (Top만 충돌)
		if(m_IsIntersected = pCube->IsIntersectedtoMakeNavCell(&vPos))
			break;
	}

	if(m_IsIntersected)
	{

		vPos.x = ceilf(vPos.x - 0.5f);
		vPos.y = 0.f;
		vPos.z = ceilf(vPos.z - 0.5f);
	
		m_vNavPoints[m_wNavCount] = vPos;
		++m_wNavCount;

		if(m_wNavCount == 3)
		{
			m_wNavCount = 0;

			if(CreateNavCell() == false)
				return ; 
		}

		else if(m_wNavCount > 3)
		{
			__asm int 3; 
		}
	}	
}

void CInputUnit::SetDeleteNavCell(void)
{
	m_IsDeletingNavCell = true;
}

bool CInputUnit::CreateNavCell(void)
{
	if( (m_vNavPoints[0].x < m_vNavPoints[1].x) && (m_vNavPoints[0].z >= m_vNavPoints[0].z)
		&& (m_vNavPoints[1].x >= m_vNavPoints[2].x) && (m_vNavPoints[1].z >= m_vNavPoints[2].z)
		&& (m_vNavPoints[0].x <= m_vNavPoints[2].x) && (m_vNavPoints[0].z > m_vNavPoints[2].z) )
	{
  		m_pNavMgr->AddCell(m_vNavPoints, m_vNavPoints + 1, m_vNavPoints + 2);
	}
	else 
		return false;
	
	return true;
}

void CInputUnit::DeleteNavCell(void)
{
	m_IsDeletingNavCell = false;

	if(m_dwNavIndexCache == -1)
		return;
	
	typedef std::vector<Engine::CNavCell*> NAVMESH;
	NAVMESH* pNavMesh = m_pNavMgr->GetNavMesh();

	Engine::CNavCell* pTempNavCell = (*pNavMesh)[m_dwNavIndexCache];
	Engine::Safe_Delete(pTempNavCell);
	
	size_t iNavMeshSize = pNavMesh->size();

	if(iNavMeshSize == 0)
		return ;

	(*pNavMesh)[m_dwNavIndexCache] = (*pNavMesh)[iNavMeshSize - 1];
	pNavMesh->pop_back();

	m_dwNavIndexCache = -1;
}

bool CInputUnit::SelectNavCell(void)
{
	bool IsIntersectedWithNavCell = false;
	typedef std::vector<Engine::CNavCell*> NAVMESH;
	NAVMESH* pNavMesh = m_pNavMgr->GetNavMesh();

	size_t iNavCellsSize = pNavMesh->size();
	size_t iNavCellIndex = 0;
	for(; iNavCellIndex != iNavCellsSize; ++iNavCellIndex)
	{
		const D3DXVECTOR3* pNavCell = (*pNavMesh)[iNavCellIndex]->GetPoint(Engine::CNavCell::POINT_A);
		D3DXVECTOR3 vPos = *(pNavCell + 1);
		D3DXVECTOR3 vPos2 = *(pNavCell + 2);

		if(D3DXIntersectTri(pNavCell, pNavCell + 1, pNavCell +2, &m_Ray.pos, &m_Ray.dir, NULL, NULL, NULL))
		{
			IsIntersectedWithNavCell = true;
			m_dwNavIndexCache = iNavCellIndex;
			break;
		}
	}

	return IsIntersectedWithNavCell;
}

void CInputUnit::ReviseNavCell(void)
{
	SelectNavCell();

	if(m_dwBaseNavCellIndex == -1)
		m_dwBaseNavCellIndex = m_dwNavIndexCache;
	else
	{
		if(m_dwFixingNavCellIndex == -1)
			m_dwFixingNavCellIndex = m_dwNavIndexCache;
	}

	// 2개의 NavCell이 설정되었으면 수정 시작
	if( m_dwBaseNavCellIndex != -1 && m_dwFixingNavCellIndex != -1 )
	{
		typedef std::vector<Engine::CNavCell*> NAVMESH;
		NAVMESH* pNavMesh = m_pNavMgr->GetNavMesh();
	
		const D3DXVECTOR3* pBaseNavCell = (*pNavMesh)[m_dwBaseNavCellIndex]->GetPoint(Engine::CNavCell::POINT_A);
		const D3DXVECTOR3* pFixingNavCell = (*pNavMesh)[m_dwFixingNavCellIndex]->GetPoint(Engine::CNavCell::POINT_A);

		D3DXVECTOR3 vBasePoint[3], vFixingPoint[3];

		memcpy(vBasePoint, pBaseNavCell, sizeof(D3DXVECTOR3) * 3);
		memcpy(vFixingPoint, pFixingNavCell, sizeof(D3DXVECTOR3) * 3);

		bool IsChanged = false;
		
		// 기준 NavCell이 위, 아래로 있을 때
		if( vBasePoint[1].z == vBasePoint[2].z )
		{
			for(int i = 0; i < 3; ++i)
			{
				if( vFixingPoint[i].z == vBasePoint[1].z )
				{
					if( vFixingPoint[i].x <= vBasePoint[2].x && vFixingPoint[i].x < vBasePoint[1].x )
					{
						vFixingPoint[i] = vBasePoint[2];
					}
					else if(  vFixingPoint[i].x > vBasePoint[2].x && vFixingPoint[i].x >= vBasePoint[1].x )
					{
						vFixingPoint[i] = vBasePoint[1];
					}
					else if( vFixingPoint[i].x > vBasePoint[2].x && vFixingPoint[i].x < vBasePoint[1].x )
					{
						if( i == 0 )
						{
							vFixingPoint[i] = vBasePoint[2];
							IsChanged = true;
						}

						if( i == 1 )
						{
							vFixingPoint[i] = vBasePoint[1];
							IsChanged = true;
						}
					}
				}
			}
		}

		else if ( vBasePoint[1].x == vBasePoint[2].x ) // 기준 NavCell이 좌, 우로 있을 때
		{
			for(int i = 0; i < 3; ++i)
			{
				if( vFixingPoint[i].x == vBasePoint[1].x )
				{
					if( vFixingPoint[i].z >= vBasePoint[1].z && vFixingPoint[i].z > vBasePoint[2].z )
					{
						vFixingPoint[i] = vBasePoint[1];
						IsChanged = true;
					}
					else if(  vFixingPoint[i].z < vBasePoint[1].z && vFixingPoint[i].z <= vBasePoint[2].z )
					{
						vFixingPoint[i] = vBasePoint[2];
						IsChanged = true;
					}
					else if( vFixingPoint[i].z < vBasePoint[1].z && vFixingPoint[i].z > vBasePoint[2].z )
					{
						if( i == 0 )
						{
							vFixingPoint[i] = vBasePoint[1];
							IsChanged = true;
						}

						if( i == 2 )
						{
							vFixingPoint[i] = vBasePoint[2];
							IsChanged = true;
						}
					}
				}
			}
		}

		(*pNavMesh)[m_dwFixingNavCellIndex]->SetPoint(vFixingPoint);

		m_dwBaseNavCellIndex = -1;
		m_dwFixingNavCellIndex = -1;		
	}
}

void CInputUnit::ResetNavCellCache(void)
{
	m_wNavCount = 0;
	m_dwNavIndexCache = -1;
	ZeroMemory(m_vNavPoints, sizeof(D3DXVECTOR3) * 3);
}

void CInputUnit::ResetData(void)
{
	ResetCubeCache();
	ResetNavCellCache();
}
