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

// 기본 메소드
public:
	static CPurpleCube* Create(LPDIRECT3DDEVICE9 pDevice, Engine::CLayer* pLayer, const DWORD& dwCubeType);
	HRESULT InitCube(Engine::CLayer* pLayer, const DWORD& dwCubeType);
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT AddComponent(void);
	void Release(void);

// 상호작용 업데이트 관련
	void UpdateCollision(void);  // 충돌 업데이트
	void UpdateMovement(void);	 // 입력된 정보에 따른 데이터 업데이트

// 마우스 상호 작용 관련
public:
	virtual bool IsIntersectedtoSelect(D3DXVECTOR3* pPos, WORD* pEdge);			// 오브젝트 픽킹
	virtual bool IsIntersectedtoMakeNavCell(D3DXVECTOR3* pPos);					// 네비 셀 생성
	virtual void ReleaseSelect(void);

// 오브젝트 정보 관련
public:
	virtual Engine::CTransform* GetCubeData(void);			// Transform 반환

private:
	CMapEditObserver*		m_pMapEditObserver;
	CMouseObserver*			m_pMouseObserver;	
};

#endif // PurpleCube_h__