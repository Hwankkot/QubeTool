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
	CMapEdit(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapEdit();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MAPEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	DECLARE_MESSAGE_MAP()

/// Customized
private:
	virtual BOOL OnInitDialog();			// Dialog �ʱ�ȭ
	virtual void CMapEdit::OnOK();			// OnOk ����
	virtual void CMapEdit::OnCancel();		// OnCancel ����

// �Է� ��ġ ����
public:
	void SetInputUnit(CInputUnit* pInputUnit);				// �Է� Ŭ���� ������ ����
	void SetCubeDataStorage(Engine::CLayer* pCubeDataLayer);	// ť�갡 ����� Layer ������ ����

// ť�� ����
public:	
	afx_msg void OnBnClickedAdd();			// ť�� �߰� ��� ����
	afx_msg void OnBnClickedEdit();			// ť�� ���� ��� ����
	afx_msg void OnBnClickedCubeType();		// ť�� Ÿ�� ����
	afx_msg void OnBnClickedCreateCube();	// ť�� ���� ����
	afx_msg void OnBnClickedDeleteCube();	// ť�� ���� ����
	afx_msg void OnBnClickedSaveCubes();	// ť�� ����
	afx_msg void OnBnClickedLoadCubes();	// ť�� �ε�

// �׺���̼� �޽� ���� 
public:
	afx_msg void OnBnClickedRecordNavCell();	// �޽� ���� ���
	afx_msg void OnBnClickedEditNavCell();		// �޽� ���� ���
	afx_msg void OnBnClickedLinkNavCell();		// �޽� ��ũ
	afx_msg void OnBnClickedDeleteNavCell();	// �޽� ����
	afx_msg void OnBnClickedReviseNavCell();	// �޽� ����
	afx_msg void OnBnClickedSaveNavMesh();
	afx_msg void OnBnClickedLoadNavMesh();

// �Է� Ŭ���� ����
private:
	CInputUnit* m_pInputUnit;	
	Engine::CLayer* m_pCubeDataLayer;	// ť�� �����Ͱ� ����� ������

// ť�� ����
private:
	BOOL m_chkAdd;				// ť�� �߰�
	BOOL m_chkEdit;				// ť�� ����
	BOOL m_IsCubeType;			// ť�� ����
	CButton m_Cubetype[9];		// ť�� ���� ���� ���� ��ư
	CMapEditSubject* m_pMapEditSubject; // �� ������ ���� ��Ʈ��

// �׺���̼� �޽� ����	
private:
	BOOL m_chkRecordNavCell;	// �׺� ���� 
	BOOL m_chkEditNavCell;		// �׺� ���� 
	BOOL m_chkReviseNavCell;	// �׺� ����
	Engine::CNavMgr*	m_pNavMgr;
};