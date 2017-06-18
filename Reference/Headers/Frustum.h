/*!
 * \file Frustum.h
 * \date 2016/03/30 16:33
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

/// TODO: change frustum to component from singleton


#ifndef Frustum_h__
#define Frustum_h__

/// Engine
#include "Engine_Include.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"

BEGIN(Engine)

class ENGINE_DLL CFrustum
{
	DECLARE_SINGLETON(CFrustum)

private:
	CFrustum(void);
	~CFrustum(void);

public:
	/// Frustum�� Vertex���� �����ϰ� Projection Matrix�� ����ķ� �����ؼ�
	/// View ��ǥ�� ������ �Լ�
	HRESULT InitFrustum(LPDIRECT3DDEVICE9 pDevice);

	/// View�� ������� ���ؼ� ���� ��ǥ�� ������ �Լ�
	void Transform_Frustum_ToWorld(void);

	/// ������ ������ǥ�� �̿��� Frustum�� ��ǥ���� ���÷� ���� �Ŀ� ����� �����.
	void Transform_Frustum_ToLocal(const D3DXMATRIX* pWorld);

	/// ���÷� ���� ����ü�� Terrain Vtx(���� ��ǥ ������ ������)�� ��ġ�� ���ؼ�
	/// ����ü ���� ������ �˻��� �Ŀ� �ﰢ���� �׷��ش�. 
	void CullingForTerrain(INDEX32* pIndex, DWORD* pTriCnt,
		const VTXTEX* pTerrainVtx, const WORD& wCntX, const WORD& wCntZ);

	/// CullingforTerrain���� ����ϴ� �Լ�
	/// �˻��� Vertex Point�� ���ڷ� �޾Ƽ� ���� ����ü�� �� 6���� �� �����ؼ�
	/// ������ ��µ� �� �ִ����� bool �����ǰ����� ����.
	bool VertexInFrustum(const D3DXVECTOR3* pPoint);
	
	/// Quadtree�� Frustum �ȿ� �ִ����� Ȯ���ϱ� ���� �߰��� �Լ�
	/// Quadtree�� �ۼ��ϸ鼭 �߰��Ǿ���.
	bool SphereInFrustum(const D3DXVECTOR3* pCenter, const float& fRadius);

private:
	/// Local Space�� ���� Vertex���� ������ Plane�� �����Ѵ�.
	void MakePlane(void);

private:
	LPDIRECT3DDEVICE9		m_pDevice;

private:
	D3DXVECTOR3			m_vPoint[8];
	D3DXVECTOR3			m_vConPoint[8];
	D3DXPLANE			m_Plane[6];
};

END

#endif // Frustum_h__