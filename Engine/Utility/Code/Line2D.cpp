#include "Line2D.h"

#include "Engine_Function.h"

Engine::CLine2D::CLine2D(void)
{

}

Engine::CLine2D::~CLine2D(void)
{

}

Engine::CLine2D* Engine::CLine2D::Create(const D3DXVECTOR3* pStartPoint, const D3DXVECTOR3* pEndPoint)
{
	CLine2D* pLine2D = new CLine2D;

	if(FAILED(pLine2D->InitLine2D(pStartPoint, pEndPoint)))
		Safe_Delete(pLine2D);

	return pLine2D;
}

HRESULT Engine::CLine2D::InitLine2D(const D3DXVECTOR3* pStartPoint, const D3DXVECTOR3* pEndPoint)
{
	// 3D������ ���͸� �޾Ƽ� D3DXVECTOR2�� ���·� �����Ѵ�.
	// �׷������� 2D ������ �ﰢ�� ���� ���͸� ���� �� �ִ�.
	m_vStartPoint = D3DXVECTOR2(pStartPoint->x, pStartPoint->z);
	m_vEndPoint = D3DXVECTOR2(pEndPoint->x, pEndPoint->z);

	// ���� �̷�� ���͸� ���Ѵ�.
	D3DXVECTOR2	vDir = m_vEndPoint - m_vStartPoint;

	// ���� ���͸� ������ ���� ���͸� ���Ѵ�.
	// ���� ���͸� ���ϴ� ������ �����ϴ�.
	// ���� ���ʹ� ������ ���Ϳ� 90���� �̷�� �����̴�.
	// �׷��� ������ ������ ������� 0���� �������� �ϸ� �ȴ�.
	// ������ ���� V�� ������� 0���� ������ ���ؼ��� V1 = V2�� ���� ����.
	// 0 = (V1.x * (V2.y * -1)) + (V1.y * (V2.x)

	m_vNormal = D3DXVECTOR2(vDir.y * -1, vDir.x);
	
	D3DXVec2Normalize(&m_vNormal, &m_vNormal);

	return S_OK;
}

Engine::CLine2D::DIR Engine::CLine2D::Check_Dir(const D3DXVECTOR2* pMovedPoint)
{
	D3DXVECTOR2 vDir = *pMovedPoint - m_vStartPoint;

	float	fDotResult = D3DXVec2Dot(&vDir, &m_vNormal);

	if(fDotResult > 0)
		return DIR_LEFT;
	else
		return DIR_RIGHT;
}

const D3DXVECTOR2& Engine::CLine2D::GetNormalVector(void) const
{
	return m_vNormal;
}

