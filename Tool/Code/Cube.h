/*!
 * \file Cube.h
 * \date 2016/04/17 15:19
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

#ifndef Cube_h__
#define Cube_h__

#include "GameObject.h"

namespace Engine
{
	class CInput;
	class CLayer;
	class CRayCollision;
	class CRenderer;
	class CResourceMgr;
	class CStaticMesh;
	class CTransform;
	class CVIBuffer;
};


class CCube
	: public Engine::CGameObject
{
public:
	enum PLANE { PLANE_TOP, PLANE_BOT, PLANE_LEFT, PLANE_RIGHT, PLANE_FRONT, PLANE_BACK, PLANE_END };
	
public:
	static const float CUBESCALE;
	static const float CUBERADIUS;
	
protected:
	explicit CCube(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit CCube(const CCube&);
	CCube& operator = (const CCube&);

public:
	virtual ~CCube(void);

// �⺻ �޼ҽ�
public:
	virtual HRESULT InitCube(Engine::CLayer* pLayer, const DWORD& dwCubeType) PURE;
	virtual void Update(void) PURE;
	virtual void Render(void) PURE;

// ���콺 ��ȣ �ۿ� ����
public:
	virtual bool IsIntersectedtoSelect(D3DXVECTOR3* pPos, WORD* pEdge) PURE;	// ������Ʈ ��ŷ
	virtual bool IsIntersectedtoMakeNavCell(D3DXVECTOR3* pPos) PURE;			// �׺� �� ����
	virtual void ReleaseSelect(void) PURE;				// ���õ� ����


// ������Ʈ ���� ����
public:
	virtual Engine::CTransform* GetCubeData(void) PURE;	// Transform ��ȯ
	DWORD GetCubeType(void);							// ť�� Ÿ�� ��ȯ
	
// ������Ʈ �� �Ŵ��� ������
protected:
	Engine::CInput*			m_pInput;
	Engine::CLayer*			m_pLayer;
	Engine::CRayCollision*	m_pRayCollision;
	Engine::CRenderer*		m_pRenderer;
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CTransform*		m_pInfo;
	Engine::CStaticMesh*	m_pMesh;
	Engine::CVIBuffer*		m_pSphere;

protected:
	bool m_IsIntersectedtoSelect;
	bool m_IsSelected;
	DWORD m_dwCubeType;
};

#endif // Cube_h__
