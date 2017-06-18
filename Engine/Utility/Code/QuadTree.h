/*!
 * \file QuadTree.h
 * \date 2016/04/06 19:15
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
#ifndef QuadTree_h__
#define QuadTree_h__

/// Engine
#include "Engine_Include.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"

BEGIN(Engine)

class ENGINE_DLL CQuadTree
{
	DECLARE_SINGLETON(CQuadTree)

public:
	/// QuadTree�� ������ Child�� Key Value.
	enum CHILD { CHILD_LT, CHILD_RT, CHILD_LB, CHILD_RB, CHILD_END };

	/// QuadTree�� Vertex���� ��Ÿ���� Index array�� Key value. 
	enum CORNER { CORNER_LT, CORNER_RT, CORNER_LB, CORNER_RB, CORNER_END };
	
	/// �ڽ��� �ֺ��� �ִ� QuadTree���� �����ϴ� array�� Ű ��.
	enum NEIGHBOR { NEIGHBOR_LEFT, NEIGHBOR_TOP, NEIGHBOR_RIGHT, NEIGHBOR_BOTTOM, NEIGHBOR_END };

private:
	CQuadTree(void);

public:
	~CQuadTree(void);

/// QuadTree
public:
	/// QuadTree�� ù ��° Node�� �����ϰ�,  
	/// QuadTree�� �����ϴ� �� �������� Index���� m_wCorner�� �����Ѵ�.
	HRESULT InitQuadTree(const VTXTEX* pTerrainVtx, const WORD& wCntX, const WORD& wCntZ);

	/// Frustum �ȿ� �ִ����� �Ǻ��ϱ� ���ؼ� 
	/// Root Node�� QuadTree�� Frustum �ȿ� �ִ����� SphereInFrustum(CFrustum ���� �Լ�)��
	/// ȣ�� �ؼ� Ȯ���Ѵ�. 
	/// �Լ��� ���ڰ�����
	/// TerrainVtx�� �Է¹޾Ƽ� Quadtree�� ������ �ִ� Center�� ���� Index�� �޾�
	/// Terrain������ ��ġ�� ã��, �������� �Է����ش�. 
	/// ���� Frustum �ȿ� Quadtree�� �����Ѵٸ� �� Quadtree�� ��Ӻ��� m_isIn�� ����
	/// True�� �����Ѵ�.
	void IsinFrustum(const VTXTEX* pTerrainVtx);

	/// ���� ���� true�� Quadtree�� ���� �ڽ� Quadtree���� �����ؼ� �ﰢ���� �����Ѵ�.
	/// �����ϴ� �ﰢ������ �ڽ� ��尡 ���� Child�� �����ϴ�.
	void CullingToQuadTree(const VTXTEX* pTerrainVtx, INDEX32* pIndex, DWORD* pTriCnt);

private:
	/// Radius�� ���ϰ� m_fRadius�� ä���ش�. Child Quadtree�� �����Ѵ�.
	void SetChild(const VTXTEX* pTerrainVtx);

	/// Child Quadtree�� �����Ѵ�.
	CQuadTree* MakeChild(CHILD eChildID);
	


/// Level of Detail
public:
	/// �ֺ��� �̿��� �����. 
	HRESULT InitNeighbor(const VTXTEX* pTerrainVtx
		, CQuadTree* pParentQuad, const WORD& wCntX, const WORD& wCntZ);
	void Culling_LOD(const VTXTEX* pTerrainVtx, INDEX32* pIndex, DWORD* pTriCnt);

private:
	/// ī�޶��� ��ġ�� �� �ﰢ���� �������� �Ÿ��� ���ؼ�
	/// ���� �Լ��� �ҷ��� Quadtree�� ���� ���̺��� ��� �ڼ��ϰ� �׸����� ������ ������ش�.
	/// Bool ������ �� ���̸� ���ؼ� Ȯ���� �� �ִ�.
	bool LevelofDetail(const VTXTEX* pTerrainVtx);

	/// �����ϴ� �̿����� �ּ� ���� �޾Ƽ� �����Ѵ�.
	bool MakeNeighbor(NEIGHBOR eNeighborID
		, int &iLT, int &iRT, int &iLB, int &iRB
		, const WORD& wCntX, const WORD& wCntZ);

	/// ���� ���� Neighbor Quadtree�� ã�Ƽ� pointer�� ��ȯ�Ѵ�.
	CQuadTree* FindNeighbor(const VTXTEX* pTerrainVtx
		, const int &iLT, const int &iRT, const int &iLB, const int &iRB
		, const int& iCenter);

private:
	void Release(void);

private:
	CQuadTree*		m_pChild[CHILD_END];
	CQuadTree*		m_pNeighbor[NEIGHBOR_END];
	WORD			m_wCorner[CORNER_END];
	int				m_iCenter;
	float			m_fRadius;
	bool			m_isIn;
};

END


#endif // QuadTree_h__