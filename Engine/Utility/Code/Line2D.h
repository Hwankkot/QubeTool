/*!
 * \file Line2D.h
 * \date 2016/04/07 17:46
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
#ifndef Line2D_h__
#define Line2D_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"

BEGIN(Engine)

class ENGINE_DLL CLine2D
{
public:
	enum DIR { DIR_LEFT, DIR_RIGHT };

private:
	CLine2D(void);
public:
	~CLine2D(void);

public:
	// ���� �ÿ� ���� ���� �� ���� Parameter(���ڰ�)�� �޴´�.
	// Create �ȿ��� InitLine2D�� ȣ���ؼ� �� ������ ���Ϳ� �������͸� ���Ѵ�.
	static CLine2D* Create(const D3DXVECTOR3* pStartPoint, const D3DXVECTOR3* pEndPoint);

public:
	// �� ������ ���Ϳ� ���� ���͸� ���ؼ� ��� ������ �־��ش�.
	HRESULT InitLine2D(const D3DXVECTOR3* pStartPoint, const D3DXVECTOR3* pEndPoint);
	// \return �����̰� �� ���� ���� �޾Ƽ� ���͸� ���� �� �������Ϳ� ���ؼ�
	// �����ʿ� �ִ��� ���ʿ� �ִ� ���� �Ǵ� �� �����Ѵ�. 
	DIR Check_Dir(const D3DXVECTOR2* pMovedPoint);
	
	// NormalVector�� ��ȯ�Ѵ�.
	const D3DXVECTOR2& GetNormalVector(void) const;

private:
	// ������ ������.
	D3DXVECTOR2 m_vStartPoint;
	// ������ ����.
	D3DXVECTOR2 m_vEndPoint;
	// ��������.
	D3DXVECTOR2 m_vNormal;
};

END

#endif // Line2D_h__