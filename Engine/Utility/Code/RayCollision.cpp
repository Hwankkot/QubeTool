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

	/// OBB�� World����� ������� ���Ѵ�.
	D3DXMatrixInverse(&matInvWorld, NULL, &matWorld);

	/// �Ƹ� OBB vMin, vMax���� �״�� ���� ������ �� ���� �ִ�. 

	RAY3 FixedRay3;
	D3DXVec3TransformCoord(&FixedRay3.pos, &Ray3.pos, &matInvWorld);
	D3DXVec3TransformCoord(&FixedRay3.dir, &Ray3.dir, &matInvWorld);

	return IsRayIntersectedAABB(FixedRay3, StdVtx3);
}

bool Engine::CRayCollision::IsRayIntersectedAABB(const RAY3& Ray3, const STANDARD_VERTEX3& StdVtx3)
{
	/// ������ ������ R(t) = A + t(B - A)
	///R(t) = A + td. �⺻������ ��ȯ

	D3DXVECTOR3 vMin = StdVtx3._min;
	D3DXVECTOR3 vMax = StdVtx3._max;

	D3DXVECTOR3 d = Ray3.dir - Ray3.pos;  
	float tmin, tmax;

	float EPSILON = 0.00001f;
	tmin = 0;					// �����ϴ� ���� t �� ��, �ʱ�ġ�� �������� �����ְ� ��Ƶд�
	tmax = 3.402823466e+38f;   //  ������ ��쿣 ����


	/*
	// �̰� �ɼ��ε�, ���� ������ ������ �ڽ��ȿ� ��ܹ������� �������� �ʴ´ٰ� �����ϱ� ���ؼ� ���� ������ �ʿ�������.
	bool IsInBox1 = Ray3.pos.x > vMin.x 
	&& Ray3.pos.x < vMax.x
	&& Ray3.pos.y > vMin.y
	&& Ray3.pos.y < vMax.y
	&& Ray3.pos.z > vMin.z
	&& Ray3.pos.z < vMax.z;           // ���� ��1�� ��ǥ�� �ڽ� �ȿ� ����ִ����� üũ
	bool IsInBox2 = Ray3.dir.x > vMin.x 
	&& Ray3.dir.x < vMax.x 
	&& Ray3.dir.y > vMin.y 
	&& Ray3.dir.y < vMax.y 
	&& Ray3.dir.z > vMin.z 
	&& Ray3.dir.z < vMax.z;			 // ���� ��2�� ��ǥ�� �ڽ� �ȿ� ����ֳ� üũ



	if(IsInBox1 && IsInBox2)                                                 // ���࿡ �� �� ���������
	return false;                                                        // �������� ����

	else if(IsInBox1 != IsInBox2)                                            // �ѳ��� �ȿ�, �ѳ��� �ۿ� �ִ�.
	return true;                                                         // ����

	*/

	// ���⼭���� �� ���к� ���� x�� ���� ����
	// ���� x������ 0�� �������
	if(abs(d.x) < EPSILON)						     // 0�� ����� ����ġ�� ���Ѵ�.
	{					                             // �󸶳� 0�� ��������� �˾ƺ��� ��. �ԽǷ�ġ�� ���� ���� ���ڸ� ���� �� �д�.
		// ������ ��κ� 0�̴�. �׷� �������� �����ȿ� ��ġ�ϴ����� Ȯ���Ѵ�.
		if(Ray3.pos.x < vMin.x || Ray3.pos.x > vMax.x) // ������ �ٱ��� �������� ��ġ�Ѵ�.
			return false;                            // ����
	}
	/// �߿��� �κ�
	else
	{

		float t1, t2, temp;
		float denom = 1.f / d.x;
		// t = (di - Ai) / (Bi - Ai)  B-A�� d�� ġȯ

		t1 = (vMin.x - Ray3.pos.x) * denom;  // Slab�Ա�
		t2 = (vMax.x - Ray3.pos.x) * denom;  // Slab�ⱸ

		// ������� ���������� ���⼭ �Ǵ��ؼ� ������� �迭�Ѵ�.
		if(t1 > t2)
		{
			temp = t1;
			t1  = t2;
			t2  = temp;        // �������ڸ� ������ ����
		}
		// ���� ����� �� �� ����ϴ� t�� ��ġ�� ��������. ���� �� ������ ���͸��� �Բ� ���������� �ȴ�.
		if(t1 > tmin)               // ���ڰ� ���� ������ �ּ�ġ���� ũ��
			tmin = t1;              // �׷� ������ ����

		if(t2 < tmax)               // ���ڰ� ū ������ �ִ�ġ���� �۴�
			tmax = t2;              // ���� ������ ����

		if(tmin > tmax)             // �۾ƾߵ� ���� �� ŭ
			return false;           // �ٸ���� ���ÿ� �������� �ʴ´�. ���� ����


		// ������ �ƴ϶� ����

		/*
		// ���� t �� ����(0 <= t <= 1)�� ������Ű���� ���͸�.
		if(tmax < 0.0f)             // t �� 0���� �۴�
		return false;           // ��������

		if(tmin > 1.0f)             // t �� 1���� ũ��
		return false;           // ��������

		*/
	}


	if(abs(d.y) < EPSILON)						     
	{					                             

		if(Ray3.pos.y < vMin.y || Ray3.pos.y > vMax.y) 
			return false;                            
	}

	/// �߿��� �κ�
	else
	{

		float t1, t2, temp;
		float denom = 1.f / d.y;

		t1 = (vMin.y - Ray3.pos.y) * denom;  // Slab�Ա�
		t2 = (vMax.y - Ray3.pos.y) * denom;  // Slab�ⱸ

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

	/// �߿��� �κ�
	else
	{

		float t1, t2, temp;
		float denom = 1.f / d.z;

		t1 = (vMin.z - Ray3.pos.z) * denom;  // Slab�Ա�
		t2 = (vMax.z - Ray3.pos.z) * denom;  // Slab�ⱸ

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

	// ������ ������ ������� y��, z�࿡ ���ؼ��� ���� �˻縦 �ǽ��ؼ� ���� t�� ������ ����������.
	// ������ t�� 0~1�� ���� �ȿ��� ��Ƴ��� �ִٸ� ������ �����Ѱ��̰� �߰��� �ѹ��̶� ��߳��� �������� ���д�
	// �ٽ��ѹ� ���������, ������ ��ġ��, ��� �������� ���ÿ� �����ؾ� ������ �����Ѵ�, ��� ���ǿ� �ִٰ� �ص� ������ �ƴϴ�.

	// ī��&�������ϸ� ������ �Ϻη� y��, z�� ������ �����Ѵ�.
	// ���� �ʹ� ����� �Ӵ���, ������� ����Դٸ� ���ƹ��� ������ ���ذ� �Ȱ����� �ٿ��ֱ� ������ �����ϸ���
	// �����ϱ� �����̴�.

	// ��� ������ �����ϰ� ������� ��Ƴ��Ҵٸ�
	return true;     // ��������
}

DWORD Engine::CRayCollision::Release(void)
{
	return 0;
}