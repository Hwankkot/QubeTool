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


// �⺻ �޼ҵ�
public:
	static CInputUnit* Create(LPDIRECT3DDEVICE9 pDevice);	// ����
	HRESULT InitInputUnit(void);	// �ʱ�ȭ,  pLayer: Cube Object���� �����ϴ� Layer
	virtual void Update(void);				
	virtual void Render(void);				
private: 
	void Release(void);	


// ���� ���� ����
public:
	void SetCubeLayer(Engine::CLayer* pLayer);


// ���콺 ����
private:
	void UpdateMousePos(void);				// ���콺 ��ġ ������Ʈ
	void ResetMousePos(void);				// ���콺 aaaaaaa��ġ ȭ�� ����� ����


// ť�� ����
private:
	void UpdateObjectInteraction(void);		// ť�� ��ȣ�ۿ� ������Ʈ
	void SelectObject(void);				// ť�� ����
	void TranslationMousePosToWorld(void);	// Ray�� ���� ��Ʈ������ ����
public:
	void SetCreateCube(BOOL bCubeType);		// ť�� ���� ����
	void SetDeleteCube(void);				// ť�� ���� ����
private:
	void CreateCube(void);					// ť�� ����
	void DeleteCube(void);					// ť�� ����
	void ResetCubeCache(void);				// ť�� ���� �ʱ�ȭ


// �׺���̼� �� ����
private: 
	void RecordNavMesh(void);					// �׺���̼� �� ������ ���� ���� ����
public:
	void SetDeleteNavCell(void);

private:
	bool CreateNavCell(void);					// �׺���̼� �� ����(�������� ����)
	void DeleteNavCell(void);					// �׺���̼� �� ����
	bool SelectNavCell(void);					// �׺���̼� �� ����
	void ReviseNavCell(void);					// �׺���̼� �� ����
	void ResetNavCellCache(void);				// �׺���̼� �� ���� �ʱ�ȭ

public:
	void ResetData(void);					// ���� ���� �ʱ�ȭ

// Singleton ����
private:
	Engine::CInput*		m_pInput;			// �Է� ��ġ
	Engine::CFontMgr*	m_pFont;			// ��Ʈ ���
	Engine::CLayer*		m_pLayer;			// Cube���� ������ Layer
	Engine::CNavMgr*	m_pNavMgr;			// �׺���̼� �� �Ŵ���
	Engine::CRenderer*	m_pRenderer;		// ������


// Observer ����	
private:
	CMapEditObserver*	m_pMapEditObserver;	// MapEdit Dialog  ������
	CMouseSubject*		m_pMouseSubject;	// ���콺 ���� ��Ʈ��

// ���콺 ����
private:
	bool	m_IsFixingMouse;			// ���콺 ��ġ ���� �ʿ� Ȯ��
	CPoint	m_ptCenterPos;				// ȭ�� ��� ��ġ
	CPoint	m_ptMousePos;				// ���� ���콺 ��ġ
	Engine::RAY3 m_Ray;					// ���� ����
		

// ť�� ����
private:
	CCube*	m_pCubeCache;				// ���õ� ť��
	bool	m_IsCreatingCube;			// ť�� ���� �ʿ� Ȯ��
	bool	m_IsDeletingCube;			// ť�� ���� �ʿ� Ȯ��
	WORD	m_wCubeEdge;				// �浹�� ť���� �� ��ġ
	DWORD	m_dwCubeType;				// �浹�� ť���� Ÿ��
	D3DXVECTOR3 m_vIntersectedPosCache;	// ť��� �浹�� ��


// ���� �޼���
private:
	TCHAR	m_szMessage[256];

// �׺���̼� �� ����
private:
	bool		m_IsRecordingNavCell;	// �׺���̼� �� �߰� Ȯ��
	bool		m_IsDeletingNavCell;	// �׺���̼� �� ���� Ȯ��
	WORD		m_wNavCount;			// ����Ǵ� �׺���̼� �� �ε���
	DWORD		m_dwNavIndexCache;		// ������ �׺���̼� �� �ε���
	DWORD		m_dwBaseNavCellIndex;	// ���� NavCell
	DWORD		m_dwFixingNavCellIndex;	// ������ NavCell
	D3DXVECTOR3 m_vNavPoints[3];		// ����Ǵ� �׺���̼� �� ����	
};

#endif // Inputunit_h__