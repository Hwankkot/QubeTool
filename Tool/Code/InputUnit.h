/*!
 * \file InputUnit.h
 * \date 2016/04/29 2:12
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

#ifndef Inputunit_h__
#define Inputunit_h__

#include "GameObject.h"
#include "Engine_Struct.h"

namespace Engine
{
	class CInput;
	class CFontMgr;
	class CLayer;
	class CNavMgr;
	class CRenderer;
}

class CCube;
class CMapEditObserver;
class CMouseSubject;
class CInputUnit : public Engine::CGameObject
{
private:
	explicit CInputUnit(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit CInputUnit(const CInputUnit&);
	CInputUnit& operator = (const CInputUnit&);

public:
	virtual ~CInputUnit(void);


// 기본 메소드
public:
	static CInputUnit* Create(LPDIRECT3DDEVICE9 pDevice);	// 생성
	HRESULT InitInputUnit(void);	// 초기화,  pLayer: Cube Object들을 저장하는 Layer
	virtual void Update(void);				
	virtual void Render(void);				
private: 
	void Release(void);	


// 저장 공간 관련
public:
	void SetCubeLayer(Engine::CLayer* pLayer);


// 마우스 관련
private:
	void UpdateMousePos(void);				// 마우스 위치 업데이트
	void ResetMousePos(void);				// 마우스 aaaaaaa위치 화면 가운데로 리셋


// 큐브 관련
private:
	void UpdateObjectInteraction(void);		// 큐브 상호작용 업데이트
	void SelectObject(void);				// 큐브 선택
	void TranslationMousePosToWorld(void);	// Ray를 월드 매트릭스로 내림
public:
	void SetCreateCube(BOOL bCubeType);		// 큐브 생성 설정
	void SetDeleteCube(void);				// 큐브 삭제 설정
private:
	void CreateCube(void);					// 큐브 생성
	void DeleteCube(void);					// 큐브 삭제
	void ResetCubeCache(void);				// 큐브 설정 초기화


// 네비게이션 셀 관련
private: 
	void RecordNavMesh(void);					// 네비게이션 셀 생성을 위한 정점 연산
public:
	void SetDeleteNavCell(void);

private:
	bool CreateNavCell(void);					// 네비게이션 셀 생성(실질적인 생성)
	void DeleteNavCell(void);					// 네비게이션 셀 삭제
	bool SelectNavCell(void);					// 네비게이션 셀 선택
	void ReviseNavCell(void);					// 네비게이션 셀 수정
	void ResetNavCellCache(void);				// 네비게이션 셀 설정 초기화

public:
	void ResetData(void);					// 현재 정보 초기화

// Singleton 관련
private:
	Engine::CInput*		m_pInput;			// 입력 장치
	Engine::CFontMgr*	m_pFont;			// 폰트 출력
	Engine::CLayer*		m_pLayer;			// Cube들을 저장한 Layer
	Engine::CNavMgr*	m_pNavMgr;			// 네비게이션 셀 매니저
	Engine::CRenderer*	m_pRenderer;		// 렌더러


// Observer 관련	
private:
	CMapEditObserver*	m_pMapEditObserver;	// MapEdit Dialog  옵저버
	CMouseSubject*		m_pMouseSubject;	// 마우스 정보 컨트롤

// 마우스 관련
private:
	bool	m_IsFixingMouse;			// 마우스 위치 수정 필요 확인
	CPoint	m_ptCenterPos;				// 화면 가운데 위치
	CPoint	m_ptMousePos;				// 현재 마우스 위치
	Engine::RAY3 m_Ray;					// 광선 정보
		

// 큐브 관련
private:
	CCube*	m_pCubeCache;				// 선택된 큐브
	bool	m_IsCreatingCube;			// 큐브 생성 필요 확인
	bool	m_IsDeletingCube;			// 큐브 삭제 필요 확인
	WORD	m_wCubeEdge;				// 충돌한 큐브의 면 위치
	DWORD	m_dwCubeType;				// 충돌한 큐브의 타입
	D3DXVECTOR3 m_vIntersectedPosCache;	// 큐브와 충돌한 점


// 렌더 메세지
private:
	TCHAR	m_szMessage[256];

// 네비게이션 셀 관련
private:
	bool		m_IsRecordingNavCell;	// 네비게이션 셀 추가 확인
	bool		m_IsDeletingNavCell;	// 네비게이션 셀 삭제 확인
	WORD		m_wNavCount;			// 저장되는 네비게이션 셀 인덱스
	DWORD		m_dwNavIndexCache;		// 선택한 네비게이션 셀 인덱스
	DWORD		m_dwBaseNavCellIndex;	// 기준 NavCell
	DWORD		m_dwFixingNavCellIndex;	// 수정할 NavCell
	D3DXVECTOR3 m_vNavPoints[3];		// 저장되는 네비게이션 셀 정점	
};

#endif // Inputunit_h__