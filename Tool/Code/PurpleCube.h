/*!
 * \file PurpleCube.h
 * \date 2016/05/02 23:31
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

#ifndef PurpleCube_h__
#define PurpleCube_h__

#include "Cube.h"

class CMapEditObserver;
class CMouseObserver;
class CPurpleCube: public CCube
{
protected:
	explicit CPurpleCube(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit CPurpleCube(const CPurpleCube&);
	CPurpleCube& operator = (const CPurpleCube&);

public:
	virtual ~CPurpleCube(void);

// �⺻ �޼ҵ�
public:
	static CPurpleCube* Create(LPDIRECT3DDEVICE9 pDevice, Engine::CLayer* pLayer, const DWORD& dwCubeType);
	HRESULT InitCube(Engine::CLayer* pLayer, const DWORD& dwCubeType);
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT AddComponent(void);
	void Release(void);

// ��ȣ�ۿ� ������Ʈ ����
	void UpdateCollision(void);  // �浹 ������Ʈ
	void UpdateMovement(void);	 // �Էµ� ������ ���� ������ ������Ʈ

// ���콺 ��ȣ �ۿ� ����
public:
	virtual bool IsIntersectedtoSelect(D3DXVECTOR3* pPos, WORD* pEdge);			// ������Ʈ ��ŷ
	virtual bool IsIntersectedtoMakeNavCell(D3DXVECTOR3* pPos);					// �׺� �� ����
	virtual void ReleaseSelect(void);

// ������Ʈ ���� ����
public:
	virtual Engine::CTransform* GetCubeData(void);			// Transform ��ȯ

private:
	CMapEditObserver*		m_pMapEditObserver;
	CMouseObserver*			m_pMouseObserver;	
};

#endif // PurpleCube_h__