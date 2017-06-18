#include "RayCollision.h"
#include "Engine_Struct.h"

Engine::CRayCollision::CRayCollision(void)
{

}

Engine::CRayCollision::~CRayCollision(void)
{

}


Engine::CRayCollision* Engine::CRayCollision::Create(void)
{
	return new CRayCollision;
}

bool Engine::CRayCollision::IsRayIntersectedSphere(const RAY3& Ray3, const D3DXVECTOR3& vSphereCenter, const float& fRadius, D3DXVECTOR3* pPos)
{
	D3DXVECTOR3 vRaytoSphere = vSphereCenter - Ray3.pos;

	float fAdjacentSide = D3DXVec3Dot(&vRaytoSphere, &Ray3.dir);
	
	float fRaytoSphereLength = D3DXVec3Length(&vRaytoSphere);

	if(fAdjacentSide < 0 && fRaytoSphereLength > fRadius)
		return false;

	float fOppositeSide_2Squared = (fRaytoSphereLength * fRaytoSphereLength) - (fAdjacentSide * fAdjacentSide);
	
	float fRadius_2Squared = fRadius * fRadius;

	if(fOppositeSide_2Squared > fRadius_2Squared)
		return false;

	return true;
}

bool Engine::CRayCollision::IsRayIntersectedBox(const STANDARD_VERTEX3& StdVtx3, const RAY3& Ray3, D3DXVECTOR3* pPos, WORD* pEdge)
{	
	bool IsIntersected = false;
	float fU, fV, fDist, fDistTmp = 3.402823466e+38f;
	D3DXVECTOR3 vPos(0.f, 0.f, 0.f);
	WORD wEdge(6);

	D3DXVECTOR3 aVertex[8] = { D3DXVECTOR3(0.f, 0.f, 0.f) };

	aVertex[0] = D3DXVECTOR3(StdVtx3._min.x, StdVtx3._max.y, StdVtx3._min.z);
	aVertex[1] = D3DXVECTOR3(StdVtx3._max.x, StdVtx3._max.y, StdVtx3._min.z);
	aVertex[2] = D3DXVECTOR3(StdVtx3._max.x, StdVtx3._min.y, StdVtx3._min.z);
	aVertex[3] = D3DXVECTOR3(StdVtx3._min.x, StdVtx3._min.y, StdVtx3._min.z);
	aVertex[4] = D3DXVECTOR3(StdVtx3._min.x, StdVtx3._max.y, StdVtx3._max.z);
	aVertex[5] = D3DXVECTOR3(StdVtx3._max.x, StdVtx3._max.y, StdVtx3._max.z);
	aVertex[6] = D3DXVECTOR3(StdVtx3._max.x, StdVtx3._min.y, StdVtx3._max.z);
	aVertex[7] = D3DXVECTOR3(StdVtx3._min.x, StdVtx3._min.y, StdVtx3._max.z);


	// TOP 0 , BOT 1, RIGHT 2, LEFT 3, FRONT 4, BACK 5


	// Top 0

	// Right - Up
	if(D3DXIntersectTri(&aVertex[5], &aVertex[4], &aVertex[1], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[5] + (aVertex[4] - aVertex[5]) * fU + (aVertex[1] - aVertex[5]) * fV;
			wEdge = 0;
			IsIntersected = true;
		}
	}

	// Left - Down
	if(D3DXIntersectTri(&aVertex[0], &aVertex[1], &aVertex[4], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[0] + (aVertex[1] - aVertex[0]) * fU + (aVertex[4] - aVertex[0]) * fV;
			wEdge = 0;
			IsIntersected = true;
		}
	}

	// Bot 1

	// Right - Up
	if(D3DXIntersectTri(&aVertex[6], &aVertex[7], &aVertex[2], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[6] + (aVertex[7] - aVertex[6]) * fU + (aVertex[2] - aVertex[6]) * fV;
			wEdge = 1;
			IsIntersected = true;
		}
	}

	// Left - Down
	if(D3DXIntersectTri(&aVertex[3], &aVertex[2], &aVertex[3], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[3] + (aVertex[2] - aVertex[3]) * fU + (aVertex[7] - aVertex[3]) * fV;
			wEdge = 1;
			IsIntersected = true;
		}
	}

	// Left 2

	// Right - Up
	if(D3DXIntersectTri(&aVertex[0], &aVertex[4], &aVertex[3], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[0] + (aVertex[4] - aVertex[0]) * fU + (aVertex[3] - aVertex[0]) * fV;
			wEdge = 2;
			IsIntersected = true;
		}
	}

	// Left - Down
	if(D3DXIntersectTri(&aVertex[7], &aVertex[3], &aVertex[4], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[7] + (aVertex[3] - aVertex[7]) * fU + (aVertex[4] - aVertex[7]) * fV;
			wEdge = 2;
			IsIntersected = true;
		}
	}
	 
	// RIGHT 3

	// Right - Up
	if(D3DXIntersectTri(&aVertex[5], &aVertex[1], &aVertex[6], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[5] + (aVertex[1] - aVertex[5]) * fU + (aVertex[6] - aVertex[5]) * fV;
			wEdge = 3;
			IsIntersected = true;
		}
	}

	// Left - Down
	if(D3DXIntersectTri(&aVertex[2], &aVertex[6], &aVertex[1], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[2] + (aVertex[6] - aVertex[2]) * fU + (aVertex[1] - aVertex[2]) * fV;
			wEdge = 3;
			IsIntersected = true;
		}
	}

	// Front 4

	// Right - Up
	if(D3DXIntersectTri(&aVertex[1], &aVertex[0], &aVertex[2], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[1] + (aVertex[0] - aVertex[1]) * fU + (aVertex[2] - aVertex[1]) * fV;
			wEdge = 4;
			IsIntersected = true;
		}
	}

	// Left - Down
	if(D3DXIntersectTri(&aVertex[3], &aVertex[2], &aVertex[0], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[3] + (aVertex[2] - aVertex[3]) * fU + (aVertex[0] - aVertex[3]) * fV;
			wEdge = 4;
			IsIntersected = true;
		}
	}

	// Back 5

	// Right - Up
	if(D3DXIntersectTri(&aVertex[5], &aVertex[4], &aVertex[6], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[5] + (aVertex[4] - aVertex[5]) * fU + (aVertex[6] - aVertex[5]) * fV;
			wEdge = 5;
			IsIntersected = true;
		} 
	}

	// Left - Down
	if(D3DXIntersectTri(&aVertex[7], &aVertex[6], &aVertex[4], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[7] + (aVertex[6] - aVertex[7]) * fU + (aVertex[4] - aVertex[7]) * fV;
			wEdge = 5;
			IsIntersected = true;
		}
	}

	*pPos = vPos;
	*pEdge = wEdge;

	return IsIntersected;
}



bool Engine::CRayCollision::IsRayIntersectedtoMakeNavMesh(const STANDARD_VERTEX3& StdVtx3, const RAY3& Ray3, D3DXVECTOR3* pPos)
{

	bool IsIntersected = false;
	float fU, fV, fDist, fDistTmp = 3.402823466e+38f, fEpsilon = 40.f;
	D3DXVECTOR3 vPos(0.f, 0.f, 0.f);
  	D3DXVECTOR3 aVertex[4] = { D3DXVECTOR3(0.f, 0.f, 0.f) };

	aVertex[0] = D3DXVECTOR3(StdVtx3._min.x, StdVtx3._max.y, StdVtx3._min.z); // 0
	aVertex[1] = D3DXVECTOR3(StdVtx3._max.x, StdVtx3._max.y, StdVtx3._min.z); // 1
	aVertex[2] = D3DXVECTOR3(StdVtx3._min.x, StdVtx3._max.y, StdVtx3._max.z); // 4
	aVertex[3] = D3DXVECTOR3(StdVtx3._max.x, StdVtx3._max.y, StdVtx3._max.z); // 5


	// Right - Up
	if(D3DXIntersectTri(&aVertex[3], &aVertex[2], &aVertex[1], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[3] + (aVertex[2] - aVertex[3]) * fU + (aVertex[1] - aVertex[3]) * fV;
			IsIntersected = true;

			if(D3DXVec3Length(&(aVertex[3] - vPos)) < fEpsilon)
				vPos = aVertex[3];

			if(D3DXVec3Length(&(aVertex[2] - vPos)) < fEpsilon)
				vPos = aVertex[2]; 

			if(D3DXVec3Length(&(aVertex[1] - vPos)) < fEpsilon)
				vPos = aVertex[1];
		}
	}

	// Left - Down
	if(D3DXIntersectTri(&aVertex[0], &aVertex[1], &aVertex[2], &Ray3.pos, &Ray3.dir, &fU, &fV, &fDist))
	{
		if(fDistTmp > fDist)
		{
			fDistTmp = fDist;
			vPos = aVertex[0] + (aVertex[1] - aVertex[0]) * fU + (aVertex[2] - aVertex[0]) * fV;
			IsIntersected = true;

			if(D3DXVec3Length(&(aVertex[0] - vPos)) < fEpsilon)
				vPos = aVertex[0];

			if(D3DXVec3Length(&(aVertex[2] - vPos)) < fEpsilon)
				vPos = aVertex[2];

			if(D3DXVec3Length(&(aVertex[1] - vPos)) < fEpsilon)
				vPos = aVertex[1];
		}
	}


		*pPos = vPos;

	return IsIntersected;
}


bool Engine::CRayCollision::IsRayIntersectedOBB(const RAY3& Ray3
												, const STANDARD_VERTEX3& StdVtx3, const D3DXMATRIX& matWorld)
{
	D3DXMATRIX matInvWorld;

	/// OBB의 World행렬의 역행렬을 구한다.
	D3DXMatrixInverse(&matInvWorld, NULL, &matWorld);

	/// 아마 OBB vMin, vMax값을 그대로 쓰면 오류가 날 수도 있다. 

	RAY3 FixedRay3;
	D3DXVec3TransformCoord(&FixedRay3.pos, &Ray3.pos, &matInvWorld);
	D3DXVec3TransformCoord(&FixedRay3.dir, &Ray3.dir, &matInvWorld);

	return IsRayIntersectedAABB(FixedRay3, StdVtx3);
}

bool Engine::CRayCollision::IsRayIntersectedAABB(const RAY3& Ray3, const STANDARD_VERTEX3& StdVtx3)
{
	/// 선분의 방정식 R(t) = A + t(B - A)
	///R(t) = A + td. 기본형으로 변환

	D3DXVECTOR3 vMin = StdVtx3._min;
	D3DXVECTOR3 vMax = StdVtx3._max;

	D3DXVECTOR3 d = Ray3.dir - Ray3.pos;  
	float tmin, tmax;

	float EPSILON = 0.00001f;
	tmin = 0;					// 교차하는 범위 t 의 값, 초기치는 가능한한 여유있게 잡아둔다
	tmax = 3.402823466e+38f;   //  직선의 경우엔 무한


	/*
	// 이건 옵션인데, 만약 선분이 완전히 박스안에 잠겨버렸을때 교차하지 않는다고 판정하기 위해선 다음 내용이 필요해진다.
	bool IsInBox1 = Ray3.pos.x > vMin.x 
	&& Ray3.pos.x < vMax.x
	&& Ray3.pos.y > vMin.y
	&& Ray3.pos.y < vMax.y
	&& Ray3.pos.z > vMin.z
	&& Ray3.pos.z < vMax.z;           // 선분 점1의 좌표가 박스 안에 들어있는지를 체크
	bool IsInBox2 = Ray3.dir.x > vMin.x 
	&& Ray3.dir.x < vMax.x 
	&& Ray3.dir.y > vMin.y 
	&& Ray3.dir.y < vMax.y 
	&& Ray3.dir.z > vMin.z 
	&& Ray3.dir.z < vMax.z;			 // 선분 점2의 좌표가 박스 안에 들어있나 체크



	if(IsInBox1 && IsInBox2)                                                 // 만약에 둘 다 들어있으면
	return false;                                                        // 교차판정 실패

	else if(IsInBox1 != IsInBox2)                                            // 한놈은 안에, 한놈은 밖에 있다.
	return true;                                                         // 성공

	*/

	// 여기서부터 각 성분별 판정 x축 부터 시작
	// 만일 x성분이 0에 가까울경우
	if(abs(d.x) < EPSILON)						     // 0에 가까운 절대치를 구한다.
	{					                             // 얼마나 0에 가까운지를 알아보는 것. 입실론치는 대충 작은 숫자를 설정 해 둔다.
		// 성분이 대부분 0이다. 그럼 시작점이 슬랩안에 위치하는지를 확인한다.
		if(Ray3.pos.x < vMin.x || Ray3.pos.x > vMax.x) // 슬랩의 바깥에 시작점이 위치한다.
			return false;                            // 실패
	}
	/// 중요한 부분
	else
	{

		float t1, t2, temp;
		float denom = 1.f / d.x;
		// t = (di - Ai) / (Bi - Ai)  B-A를 d로 치환

		t1 = (vMin.x - Ray3.pos.x) * denom;  // Slab입구
		t2 = (vMax.x - Ray3.pos.x) * denom;  // Slab출구

		// 양수인지 음수인지를 여기서 판단해서 순서대로 배열한다.
		if(t1 > t2)
		{
			temp = t1;
			t1  = t2;
			t2  = temp;        // 작은숫자를 앞으로 스왑
		}
		// 슬랩 평면을 각 각 통과하는 t의 수치가 정해졌다. 이제 그 범위를 필터링과 함께 좁혀나가면 된다.
		if(t1 > tmin)               // 숫자가 작은 변수가 최소치보다 크다
			tmin = t1;              // 그럼 데이터 갱신

		if(t2 < tmax)               // 숫자가 큰 변수가 최대치보다 작다
			tmax = t2;              // 역시 데이터 갱신

		if(tmin > tmax)             // 작아야될 값이 더 큼
			return false;           // 다른축과 동시에 교차하지 않는다. 교차 실패


		// 선분이 아니라서 생략

		/*
		// 선분 t 의 범위(0 <= t <= 1)를 만족시키는지 필터링.
		if(tmax < 0.0f)             // t 가 0보다 작다
		return false;           // 교차실패

		if(tmin > 1.0f)             // t 가 1보다 크다
		return false;           // 교차실패

		*/
	}


	if(abs(d.y) < EPSILON)						     
	{					                             

		if(Ray3.pos.y < vMin.y || Ray3.pos.y > vMax.y) 
			return false;                            
	}

	/// 중요한 부분
	else
	{

		float t1, t2, temp;
		float denom = 1.f / d.y;

		t1 = (vMin.y - Ray3.pos.y) * denom;  // Slab입구
		t2 = (vMax.y - Ray3.pos.y) * denom;  // Slab출구

		if(t1 > t2)
		{
			temp = t1;
			t1  = t2;
			t2  = temp;        
		}

		if(t1 > tmin)          
			tmin = t1;         

		if(t2 < tmax)          
			tmax = t2;         

		if(tmin > tmax)        
			return false;      
	}

	if(abs(d.z) < EPSILON)						     
	{					                             

		if(Ray3.pos.z < vMin.z || Ray3.pos.z > vMax.z) 
			return false;                            
	}

	/// 중요한 부분
	else
	{

		float t1, t2, temp;
		float denom = 1.f / d.z;

		t1 = (vMin.z - Ray3.pos.z) * denom;  // Slab입구
		t2 = (vMax.z - Ray3.pos.z) * denom;  // Slab출구

		if(t1 > t2)
		{
			temp = t1;
			t1  = t2;
			t2  = temp;        
		}

		if(t1 > tmin)          
			tmin = t1;         

		if(t2 < tmax)          
			tmax = t2;         

		if(tmin > tmax)        
			return false;      
	}

	D3DXVECTOR3 vStartPoint = Ray3.pos + tmin * d;

	// 완전히 동일한 방법으로 y축, z축에 대해서도 같은 검사를 실시해서 점점 t의 범위를 좁혀나간다.
	// 끝까지 t가 0~1의 범위 안에서 살아남아 있다면 교차가 성공한것이고 중간에 한번이라도 어긋나면 여지없이 실패다
	// 다시한번 상기하지만, 슬랩의 가치는, 모든 슬랩에서 동시에 만족해야 교차가 성공한다, 라는 조건에 있다고 해도 과언이 아니다.

	// 카피&페이즈하면 되지만 일부러 y축, z축 설정은 생략한다.
	// 글이 너무 길어질 뿐더러, 여기까지 따라왔다면 제아무리 내용이 이해가 안가더라도 붙여넣기 정도는 가능하리라
	// 생각하기 때문이다.

	// 모든 조건을 만족하고 여기까지 살아남았다면
	return true;     // 교차성공
}

DWORD Engine::CRayCollision::Release(void)
{
	return 0;
}