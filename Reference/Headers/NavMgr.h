/*!
 * \file NavMgr.h
 * \date 2016/04/07 19:35
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

// �⺻ Progress
// NavMgr�� ����� Export_Utility�� inline �Լ� �߰�.
// Stage���� Add_NavCell �Լ��� �����
// Cell�� �߰��Ѵ�.
// Player�� �߰������� ����� Player ������ NavCell�� �����Ų��.

#ifndef NavMgr_h__
#define NavMgr_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"

BEGIN(Engine)

class CNavCell;

class ENGINE_DLL CNavMgr
{
	DECLARE_SINGLETON(CNavMgr);

private:
	CNavMgr(void);
	~CNavMgr(void);

public:
	// Vector ���� NavCell ����
	void Release(void);

public:
	// STL������ ũ�⸦ dwSize ��ŭ �̸� �Ҵ��ϰ� Line com ��ü�� �����.
	HRESULT InitNavMeshManager(LPDIRECT3DDEVICE9 pDevice, const DWORD& dwContainerSize);
	// ���ڰ����� ���� ������ ������ CNavCell�� �����Ѵ�.
	HRESULT AddCell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC);
	// ��� ������ �̿��� �����Ѵ�.
	void LinkCell(void);

	DWORD MoveOnNavMesh(D3DXVECTOR3* pPos, D3DXVECTOR3* pDir, const DWORD& dwCurrentIdx);
	
	// ������ ��� NavCell���� �������Ѵ�.
	void RenderNavMesh(void);

	std::vector<CNavCell*>* GetNavMesh(void);



private:
	LPDIRECT3DDEVICE9	m_pDevice;
	LPD3DXLINE			m_pLine;
	DWORD				m_dwReservedSize;
	DWORD				m_dwIdxCnt;

private:
	typedef std::vector<CNavCell*>  NAVMESH;
	NAVMESH  m_NavMesh;
};

END

#endif // NavMgr_h__