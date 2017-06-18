/*!
 * \file OrangeCube.h
 * \date 2016/05/02 23:45
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
#ifndef OrangeCube_h__
#define OrangeCube_h__

#include "Cube.h"

class CMapEditObserver;
class CMouseObserver;
class COrangeCube: public CCube
{
protected:
	explicit COrangeCube(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit COrangeCube(const COrangeCube&);
	COrangeCube& operator = (const COrangeCube&);

public:
	virtual ~COrangeCube(void);

// �⺻ �޼ҵ�
public:
	static COrangeCube* Create(LPDIRECT3DDEVICE9 pDevice, Engine::CLayer* pLayer, const DWORD& dwCubeType);
	HRESULT InitCube(Engine::CLayer* pLayer, const DWORD& dwCubeType);
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT AddComponent(void);
	void Release(void);

// ��ȣ�ۿ� ������Ʈ ����
	void UpdateCollision(void);  // �浹 ������Ʈ5
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

#endif // OrangeCube_h__