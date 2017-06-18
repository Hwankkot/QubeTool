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

	// ���� ���� Index.
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
	// ���� ���� �����͸� �޴´�.
	CNavCell* pNavCell = m_NavMesh[dwCurrentIdx];
	
	// �̿� ��ȣ�� ������ ����
	CNavCell::NEIGHBOR  eNeighbor;
	
	// ���� ���� �ε��� ������ �޴´�.  
	DWORD  dwNextIndex = dwCurrentIdx;

	// �׺� �޽��� ��ü ������� �Ѿ�� ��ǥ�� �������ִ� ������ �ذ��� �� �� ����! ����!
	if(pPos->x < 0)
		pPos->x = 0;

	if(pPos->x > 4)
		pPos->x = 4;

	if(pPos->z > 4)
		pPos->z = 4;

	// ���ΰ� �Ѿ�� �κп� ���� ����!

	// ���� �÷��̾��� ��ġ�� �޴´�.
	D3DXVECTOR3	vPlayerPos = *pPos;
	D3DXVECTOR3 vCurrentDir = *pDir;
	
	// ������ �������� ��
	// ���� ������ ���� ��ġ, �̵��� ���� ����(�ӵ� * ����), �̿������� �ִ´�.
 	if(pNavCell->IsPassed(vPlayerPos, vCurrentDir, &eNeighbor))
	{
		// ������ ���� ����Ǿ� �ִ� �̿��� �޴´�.
		CNavCell* pNeighbor = m_NavMesh[dwCurrentIdx]->GetNeighbor(eNeighbor);

		// ���� �̿��� ������,
		if(pNeighbor == NULL)
		{
			// �븻���͸� ���� ����
			D3DXVECTOR2 vNorm(0.f, 0.f);
			
			// �÷��̾��� 2D���� ����
			D3DXVECTOR2 vPlayerDir(vCurrentDir.x, vCurrentDir.z);

			// �̿��� ���� Norm Vector�� �޴´�.
			vNorm = m_NavMesh[dwCurrentIdx]->GetLine(CNavCell::LINE(eNeighbor)).GetNormalVector();

			// �븻������ ������ �ٲ��ش�.
			vNorm *= -1.f;
			
			// �����̵� ���͸� �����.
			D3DXVECTOR2 vFixedDir = vPlayerDir - (vNorm * D3DXVec2Dot(&vPlayerDir, &vNorm));

			// ���� ������ ������ �����̵� ������ �������� �ٲ��ش�.
			vCurrentDir.x = vFixedDir.x;
			vCurrentDir.z = vFixedDir.y;

			// ������ ������ �˻��ؼ� 
			if(m_NavMesh[dwCurrentIdx]->IsPassed(vPlayerPos, vCurrentDir, &eNeighbor))
			{

				vPlayerDir = D3DXVECTOR2(vCurrentDir.x, vCurrentDir.z);

				// ������ �������� �̵����� �� �Ѿ �̿��� �ٽ� Ȯ���ϰ� 
				pNeighbor = m_NavMesh[dwCurrentIdx]->GetNeighbor(eNeighbor);

				// ������ ���⿡�� �̿��̾�����
				if(pNeighbor == NULL)
				{

					// �ٽ� �����Ѵ�? �׳� ��������°� ����..?

					vNorm = m_NavMesh[dwCurrentIdx]->GetLine(CNavCell::LINE(eNeighbor)).GetNormalVector();
					vNorm *= -1.f;

					vFixedDir = vPlayerDir - (vNorm * D3DXVec2Dot(&vPlayerDir, &vNorm));

					vCurrentDir.x = vFixedDir.x;
					vCurrentDir.z = vFixedDir.y;

					*pPos += vCurrentDir;

				}
				else // �̿��� ������, ���� ��ġ�� �̿��� �Ѿ���� �ٽ� Ȯ���ϱ� ���ؼ� �Լ��� ȣ���ض� 
				{
					dwNextIndex = pNeighbor->GetIndex();
					//dwNextIndex = m_NavMesh[dwNextIndex]->IsPassed(vPlayerPos, vCurrentDir, &eNeighbor);
					*pPos += vCurrentDir;
				}
			}
			else // �ƹ����� �� ���� ���� ���� 
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
	else // ���������� ��
	{
		*pPos += *pDir;
	}

	return dwNextIndex;
}	

std::vector<Engine::CNavCell*>* Engine::CNavMgr::GetNavMesh(void)
{
	return &m_NavMesh;
}
