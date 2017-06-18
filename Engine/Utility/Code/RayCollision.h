/*!
 * \file RayCollision.h
 * \date 2016/04/19 1:18
 *
 * \author Architect
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef RayCollision_h__
#define RayCollision_h__

#include "Collision.h"
#include "Engine_Struct.h"

BEGIN(Engine)

class ENGINE_DLL CRayCollision : public CComponent
{
private:
	CRayCollision(void);

public:
	~CRayCollision(void);

public:
	static CRayCollision* Create(void);

public:
	bool IsRayIntersectedSphere(const RAY3& Ray3, const D3DXVECTOR3& vSphereCenter, const float& fRadius, D3DXVECTOR3* pPos);
	bool IsRayIntersectedBox(const STANDARD_VERTEX3& StdVtx3, const RAY3& Ray3, D3DXVECTOR3* pPos, WORD* pEdge);
	bool IsRayIntersectedtoMakeNavMesh(const STANDARD_VERTEX3& StdVtx3, const RAY3& Ray3, D3DXVECTOR3* pPos);
	bool IsRayIntersectedOBB(const RAY3& Ray3, const STANDARD_VERTEX3& StdVtx3, const D3DXMATRIX& matWorld);
	bool IsRayIntersectedAABB(const RAY3& Ray3, const STANDARD_VERTEX3& StdVtx3);

public:
	virtual DWORD Release(void);

};

END

#endif // RayCollision_h__