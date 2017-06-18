#include "NavMgr.h"
#include "NavCell.h"

#include "Engine_Functor.h"
#include "Line2D.h"

IMPLEMENT_SINGLETON(Engine::CNavMgr)

Engine::CNavMgr::CNavMgr(void)
: m_pDevice(NULL)
, m_pLine(NULL)
, m_dwReservedSize(0)
, m_dwIdxCnt(0)
{

}

Engine::CNavMgr::~CNavMgr(void)
{
	Release();
}

HRESULT Engine::CNavMgr::InitNavMeshManager(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwContainerSize)
{
	m_pDevice = pDevice;
	m_dwReservedSize = dwContainerSize;
	m_NavMesh.reserve(m_dwReservedSize);
	FAILED_CHECK(D3DXCreateLine(m_pDevice, &m_pLine));

	return S_OK;
}

HRESULT Engine::CNavMgr::AddCell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC)
{
	CNavCell* pNavCell = CNavCell::Create(m_pDevice, pPointA, pPointB, pPointC, m_dwIdxCnt);
	NULL_CHECK_RETURN(pNavCell, E_FAIL);

	// 다음 셀의 Index.
	++m_dwIdxCnt;

	m_NavMesh.push_back(pNavCell);

	return S_OK;
}

void Engine::CNavMgr::LinkCell(void)
{
	NAVMESH::iterator	iter = m_NavMesh.begin();
	NAVMESH::iterator	iter_end = m_NavMesh.end();

	if(iter == iter_end)
		return;

	for(; iter != iter_end; ++iter)
	{
		NAVMESH::iterator	iter_Target = m_NavMesh.begin();
		while(iter_Target != iter_end)
		{
			if(iter == iter_Target)
			{
				++iter_Target;
				continue;
			}

			if((*iter_Target)->Compare_Point((*iter)->GetPoint(CNavCell::POINT_A), (*iter)->GetPoint(CNavCell::POINT_B), (*iter)))
				(*iter)->SetNeighbor(CNavCell::NEIGHBOR_AB, (*iter_Target));

			else if((*iter_Target)->Compare_Point((*iter)->GetPoint(CNavCell::POINT_B), (*iter)->GetPoint(CNavCell::POINT_C), (*iter)))
				(*iter)->SetNeighbor(CNavCell::NEIGHBOR_BC, (*iter_Target));

			else if((*iter_Target)->Compare_Point((*iter)->GetPoint(CNavCell::POINT_C), (*iter)->GetPoint(CNavCell::POINT_A), (*iter)))
				(*iter)->SetNeighbor(CNavCell::NEIGHBOR_CA, (*iter_Target));
			++iter_Target;
		}
	}
}

void Engine::CNavMgr::RenderNavMesh(void)
{
	size_t iNavMeshSize = m_NavMesh.size();

	if(iNavMeshSize == 0)
		return ;

	for(size_t i = 0; i < iNavMeshSize; ++i)
		m_NavMesh[i]->Render(m_pLine);
}

void Engine::CNavMgr::Release(void)
{
	for_each(m_NavMesh.begin(), m_NavMesh.end(), CDeleteObj());
	m_NavMesh.clear();
}

DWORD Engine::CNavMgr::MoveOnNavMesh(D3DXVECTOR3* pPos, D3DXVECTOR3* pDir, const DWORD& dwCurrentIdx)
{
	// 현재 셀의 포인터를 받는다.
	CNavCell* pNavCell = m_NavMesh[dwCurrentIdx];
	
	// 이웃 번호을 저장할 변수
	CNavCell::NEIGHBOR  eNeighbor;
	
	// 현재 셀의 인덱스 정보를 받는다.  
	DWORD  dwNextIndex = dwCurrentIdx;

	// 네비 메쉬의 전체 사이즈보다 넘어가면 좌표를 수정해주는 것으로 해결이 될 것 같음! 굳굳!
	if(pPos->x < 0)
		pPos->x = 0;

	if(pPos->x > 4)
		pPos->x = 4;

	if(pPos->z > 4)
		pPos->z = 4;

	// 가로가 넘어가는 부분에 버그 있음!

	// 현재 플레이어의 위치를 받는다.
	D3DXVECTOR3	vPlayerPos = *pPos;
	D3DXVECTOR3 vCurrentDir = *pDir;
	
	// 라인을 지나갔을 때
	// 인자 값으로 현재 위치, 이동할 방향 벡터(속도 * 방향), 이웃변수를 넣는다.
 	if(pNavCell->IsPassed(vPlayerPos, vCurrentDir, &eNeighbor))
	{
		// 지나간 변과 연결되어 있는 이웃을 받는다.
		CNavCell* pNeighbor = m_NavMesh[dwCurrentIdx]->GetNeighbor(eNeighbor);

		// 만약 이웃이 없으면,
		if(pNeighbor == NULL)
		{
			// 노말벡터를 담을 변수
			D3DXVECTOR2 vNorm(0.f, 0.f);
			
			// 플레이어의 2D상의 방향
			D3DXVECTOR2 vPlayerDir(vCurrentDir.x, vCurrentDir.z);

			// 이웃에 따른 Norm Vector를 받는다.
			vNorm = m_NavMesh[dwCurrentIdx]->GetLine(CNavCell::LINE(eNeighbor)).GetNormalVector();

			// 노말벡터의 방향을 바꿔준다.
			vNorm *= -1.f;
			
			// 슬라이딩 벡터를 만든다.
			D3DXVECTOR2 vFixedDir = vPlayerDir - (vNorm * D3DXVec2Dot(&vPlayerDir, &vNorm));

			// 현재 벡터의 방향을 슬라이딩 벡터의 방향으로 바꿔준다.
			vCurrentDir.x = vFixedDir.x;
			vCurrentDir.z = vFixedDir.y;

			// 수정된 방향을 검사해서 
			if(m_NavMesh[dwCurrentIdx]->IsPassed(vPlayerPos, vCurrentDir, &eNeighbor))
			{

				vPlayerDir = D3DXVECTOR2(vCurrentDir.x, vCurrentDir.z);

				// 수정된 방향으로 이동했을 때 넘어간 이웃을 다시 확인하고 
				pNeighbor = m_NavMesh[dwCurrentIdx]->GetNeighbor(eNeighbor);

				// 수정된 방향에서 이웃이없으면
				if(pNeighbor == NULL)
				{

					// 다시 수정한다? 그냥 멈춰버리는게 낫나..?

					vNorm = m_NavMesh[dwCurrentIdx]->GetLine(CNavCell::LINE(eNeighbor)).GetNormalVector();
					vNorm *= -1.f;

					vFixedDir = vPlayerDir - (vNorm * D3DXVec2Dot(&vPlayerDir, &vNorm));

					vCurrentDir.x = vFixedDir.x;
					vCurrentDir.z = vFixedDir.y;

					*pPos += vCurrentDir;

				}
				else // 이웃이 있으면, 현재 위치가 이웃을 넘어갔는지 다시 확인하기 위해서 함수를 호출해라 
				{
					dwNextIndex = pNeighbor->GetIndex();
					//dwNextIndex = m_NavMesh[dwNextIndex]->IsPassed(vPlayerPos, vCurrentDir, &eNeighbor);
					*pPos += vCurrentDir;
				}
			}
			else // 아무데도 못 지나 갔을 때는 
			{
				*pPos += *pDir;
			}

		}
		else
		{
			dwNextIndex = pNeighbor->GetIndex();
			*pPos += *pDir;
		}
	}
	else // 못지나갔을 때
	{
		*pPos += *pDir;
	}

	return dwNextIndex;
}	

std::vector<Engine::CNavCell*>* Engine::CNavMgr::GetNavMesh(void)
{
	return &m_NavMesh;
}
