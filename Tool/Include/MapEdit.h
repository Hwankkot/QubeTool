#pragma once
#include "afxwin.h"

namespace Engine
{
	class CLayer;
	class CNavMgr;
}
class CInputUnit;
class CMapEditSubject;
class CMapEdit : public CDialog
{
	DECLARE_DYNAMIC(CMapEdit)

public:
	CMapEdit(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapEdit();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAPEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

/// Customized
private:
	virtual BOOL OnInitDialog();			// Dialog 초기화
	virtual void CMapEdit::OnOK();			// OnOk 막음
	virtual void CMapEdit::OnCancel();		// OnCancel 막음

// 입력 장치 관련
public:
	void SetInputUnit(CInputUnit* pInputUnit);				// 입력 클래스 포인터 저장
	void SetCubeDataStorage(Engine::CLayer* pCubeDataLayer);	// 큐브가 저장된 Layer 포인터 저장

// 큐브 관련
public:	
	afx_msg void OnBnClickedAdd();			// 큐브 추가 모드 설정
	afx_msg void OnBnClickedEdit();			// 큐브 수정 모드 설정
	afx_msg void OnBnClickedCubeType();		// 큐브 타입 설정
	afx_msg void OnBnClickedCreateCube();	// 큐브 생성 예약
	afx_msg void OnBnClickedDeleteCube();	// 큐브 삭제 예약
	afx_msg void OnBnClickedSaveCubes();	// 큐브 저장
	afx_msg void OnBnClickedLoadCubes();	// 큐브 로드

// 네비게이션 메시 관련 
public:
	afx_msg void OnBnClickedRecordNavCell();	// 메시 생성 모드
	afx_msg void OnBnClickedEditNavCell();		// 메시 수정 모드
	afx_msg void OnBnClickedLinkNavCell();		// 메시 링크
	afx_msg void OnBnClickedDeleteNavCell();	// 메시 삭제
	afx_msg void OnBnClickedReviseNavCell();	// 메시 보정
	afx_msg void OnBnClickedSaveNavMesh();
	afx_msg void OnBnClickedLoadNavMesh();

// 입력 클래스 관련
private:
	CInputUnit* m_pInputUnit;	
	Engine::CLayer* m_pCubeDataLayer;	// 큐브 데이터가 저장된 포인터

// 큐브 관련
private:
	BOOL m_chkAdd;				// 큐브 추가
	BOOL m_chkEdit;				// 큐브 수정
	BOOL m_IsCubeType;			// 큐브 종류
	CButton m_Cubetype[9];		// 큐브 종류 설정 라디오 버튼
	CMapEditSubject* m_pMapEditSubject; // 맵 에디터 정보 컨트롤

// 네비게이션 메시 관련	
private:
	BOOL m_chkRecordNavCell;	// 네비 저장 
	BOOL m_chkEditNavCell;		// 네비 수정 
	BOOL m_chkReviseNavCell;	// 네비 보정
	Engine::CNavMgr*	m_pNavMgr;
};