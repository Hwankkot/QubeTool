#include "stdafx.h"
#include "Tool.h"
#include "MapEdit.h"

#include "InputUnit.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Enum.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Layer.h"
#include "MapEditSubject.h"
#include "NavCell.h"
#include "Transform.h"

// Cube
#include "WhiteCube.h"
#include "GreenCube.h"
#include "PurpleCube.h"
#include "OrangeCube.h"
#include "BlueCube.h"
#include "RedCube.h"
#include "YellowCube.h"
#include "GrayCube.h"
#include "CubeWall.h"

IMPLEMENT_DYNAMIC(CMapEdit, CDialog)

CMapEdit::CMapEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CMapEdit::IDD, pParent)
	, m_pInputUnit(NULL)
	, m_pCubeDataLayer(NULL)
	, m_chkAdd(FALSE)
	, m_chkEdit(FALSE)
	, m_pMapEditSubject(NULL)
	, m_IsCubeType(0)
	, m_chkRecordNavCell(FALSE)
	, m_chkEditNavCell(FALSE)
	, m_chkReviseNavCell(FALSE)
{

}

CMapEdit::~CMapEdit()
{
	
}

void CMapEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_chkAdd);
	DDX_Check(pDX, IDC_CHECK2, m_chkEdit);
	DDX_Control(pDX, IDC_RADIO1, m_Cubetype[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Cubetype[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Cubetype[2]);
	DDX_Control(pDX, IDC_RADIO4, m_Cubetype[3]);
	DDX_Control(pDX, IDC_RADIO5, m_Cubetype[4]);
	DDX_Control(pDX, IDC_RADIO6, m_Cubetype[5]);
	DDX_Control(pDX, IDC_RADIO7, m_Cubetype[6]);
	DDX_Control(pDX, IDC_RADIO8, m_Cubetype[7]);
	DDX_Control(pDX, IDC_RADIO9, m_Cubetype[8]);
	DDX_Check(pDX, IDC_CHECK3, m_chkRecordNavCell);
	DDX_Check(pDX, IDC_CHECK5, m_chkEditNavCell);
	DDX_Check(pDX, IDC_CHECK6, m_chkReviseNavCell);
}

BEGIN_MESSAGE_MAP(CMapEdit, CDialog)
	ON_BN_CLICKED(IDC_CHECK1, &CMapEdit::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_CHECK2, &CMapEdit::OnBnClickedEdit)
	ON_BN_CLICKED(IDC_RADIO1, &CMapEdit::OnBnClickedCubeType)
	ON_BN_CLICKED(IDC_RADIO2, &CMapEdit::OnBnClickedCubeType)
	ON_BN_CLICKED(IDC_RADIO3, &CMapEdit::OnBnClickedCubeType)
	ON_BN_CLICKED(IDC_RADIO4, &CMapEdit::OnBnClickedCubeType)
	ON_BN_CLICKED(IDC_RADIO5, &CMapEdit::OnBnClickedCubeType)
	ON_BN_CLICKED(IDC_RADIO6, &CMapEdit::OnBnClickedCubeType)
	ON_BN_CLICKED(IDC_RADIO7, &CMapEdit::OnBnClickedCubeType)
	ON_BN_CLICKED(IDC_RADIO8, &CMapEdit::OnBnClickedCubeType)
	ON_BN_CLICKED(IDC_RADIO9, &CMapEdit::OnBnClickedCubeType)

	ON_BN_CLICKED(IDC_BUTTON1, &CMapEdit::OnBnClickedCreateCube)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapEdit::OnBnClickedDeleteCube)
	ON_BN_CLICKED(IDC_BUTTON3, &CMapEdit::OnBnClickedLinkNavCell)
	ON_BN_CLICKED(IDC_BUTTON4, &CMapEdit::OnBnClickedDeleteNavCell)
	ON_BN_CLICKED(IDC_CHECK3, &CMapEdit::OnBnClickedRecordNavCell)
	ON_BN_CLICKED(IDC_CHECK5, &CMapEdit::OnBnClickedEditNavCell)
	ON_BN_CLICKED(IDC_CHECK6, &CMapEdit::OnBnClickedReviseNavCell)
	ON_BN_CLICKED(IDC_BUTTON5, &CMapEdit::OnBnClickedSaveCubes)
	ON_BN_CLICKED(IDC_BUTTON6, &CMapEdit::OnBnClickedLoadCubes)
	ON_BN_CLICKED(IDC_BUTTON7, &CMapEdit::OnBnClickedSaveNavMesh)
	ON_BN_CLICKED(IDC_BUTTON8, &CMapEdit::OnBnClickedLoadNavMesh)
END_MESSAGE_MAP()

void CMapEdit::OnOK()
{
	//    CDialog::OnOK();
}

void CMapEdit::OnCancel()
{
	//    CDialog::OnCancel();
}

void CMapEdit::SetInputUnit(CInputUnit* pInputUnit)
{
	m_pInputUnit = pInputUnit;

	if(m_pInputUnit == NULL)
		MessageBox(L"m_pInputUnit is empty", L"System Error", MB_OK);
}

void CMapEdit::SetCubeDataStorage(Engine::CLayer* pCubeDataLayer)
{
	m_pCubeDataLayer = pCubeDataLayer;

	if(m_pCubeDataLayer == NULL)
		MessageBox(L"m_CubeDataLayer is empty", L"System Error", MB_OK);
}

BOOL CMapEdit::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pMapEditSubject = CMapEditSubject::GetInstance();
	m_pNavMgr =	Engine::Get_NavMgr();

	m_pMapEditSubject->AddData(CMapEditSubject::DATAMESSAGE_ADD, CMapEditSubject::DATATYPE_BOOL, sizeof(BOOL), 300);
	m_pMapEditSubject->AddData(CMapEditSubject::DATAMESSAGE_EDIT, CMapEditSubject::DATATYPE_BOOL, sizeof(BOOL));
	m_pMapEditSubject->AddData(CMapEditSubject::DATAMESSAGE_RECORDNAVCELL, CMapEditSubject::DATATYPE_BOOL, sizeof(BOOL));
	m_pMapEditSubject->AddData(CMapEditSubject::DATAMESSAGE_EDITNAVCELL, CMapEditSubject::DATATYPE_BOOL, sizeof(BOOL));
	m_pMapEditSubject->AddData(CMapEditSubject::DATAMESSAGE_REVISENAVCELL, CMapEditSubject::DATATYPE_BOOL, sizeof(BOOL));

	m_Cubetype[0].SetCheck(TRUE);
	m_IsCubeType = GAMEOBJECTID::CUBE_WHITE;

	return TRUE;
}

void CMapEdit::OnBnClickedAdd()
{
	UpdateData(TRUE);

	if(m_chkEdit)
	{
		m_chkEdit = 0;
		m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_EDIT, (void*)&m_chkEdit, sizeof(BOOL));
	}

	if(m_chkRecordNavCell)
	{
		m_chkRecordNavCell = 0;
		m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_RECORDNAVCELL, (void*)&m_chkRecordNavCell, sizeof(BOOL));
	}

	if(m_chkEditNavCell)
	{
		m_chkEditNavCell = 0;
		m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_EDITNAVCELL, (void*)&m_chkEditNavCell, sizeof(BOOL));
	}

	if(m_chkReviseNavCell)
	{
		m_chkReviseNavCell = 0;
		m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_REVISENAVCELL, (void*)&m_chkReviseNavCell, sizeof(BOOL));
	}

	m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_ADD, (void*)&m_chkAdd, sizeof(BOOL));

	UpdateData(FALSE);
}

void CMapEdit::OnBnClickedEdit()
{
	UpdateData(TRUE);
	
	if(m_chkAdd)
	{
		m_chkAdd = 0;
		m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_ADD, (void*)&m_chkAdd, sizeof(BOOL));
	}

	if(m_chkRecordNavCell)
	{
		m_chkRecordNavCell = 0;
		m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_RECORDNAVCELL, (void*)&m_chkRecordNavCell, sizeof(BOOL));
	}

	if(m_chkEditNavCell)
	{
		m_chkEditNavCell = 0;
		m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_EDITNAVCELL, (void*)&m_chkEditNavCell, sizeof(BOOL));
	}

	if(m_chkReviseNavCell)
	{
		m_chkReviseNavCell = 0;
		m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_REVISENAVCELL, (void*)&m_chkReviseNavCell, sizeof(BOOL));
	}


	m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_EDIT, (void*)&m_chkEdit, sizeof(BOOL));

	UpdateData(FALSE);
}

void CMapEdit::OnBnClickedCubeType()
{
	for(int i = 0; i < 9; ++i)
	{
		if(m_Cubetype[i].GetCheck())
		{
			m_IsCubeType = i  + (int)(GAMEOBJECTID::CUBE_WHITE);
			break;
		}
	}
}
 
void CMapEdit::OnBnClickedCreateCube()
{
	if(m_pInputUnit)
  		m_pInputUnit->SetCreateCube(m_IsCubeType);
}

void CMapEdit::OnBnClickedDeleteCube()
{
	if(m_pInputUnit)
		m_pInputUnit->SetDeleteCube();
}

void CMapEdit::OnBnClickedSaveCubes()
{
	CFileDialog	Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.dat", this);

	if(Dlg.DoModal() == IDCANCEL)
		return;

	TCHAR szDirectoryPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirectoryPath), szDirectoryPath);
	SetCurrentDirectory(szDirectoryPath);
	PathRemoveFileSpec(szDirectoryPath);	

	lstrcat(szDirectoryPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szDirectoryPath;

	HANDLE	hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD		dwByte = 0;
	
	Engine::CUBEDATA CubeData;
	ZeroMemory(&CubeData, sizeof(Engine::CUBEDATA));

	typedef std::vector<Engine::CGameObject*>	OBJECTLIST;
	typedef std::map<DWORD, OBJECTLIST>			MAPOBJECTLIST;
	MAPOBJECTLIST& mapObjectlist = m_pCubeDataLayer->GetMapObjectlist();

	for(int i = GAMEOBJECTID::CUBE_WHITE; i < GAMEOBJECTID::CUBE_END; ++i)
	{
		if( mapObjectlist.count( GAMEOBJECTID::CUBE(i) ) )
		{
			size_t iSize = mapObjectlist[i].size();
			OBJECTLIST Objectlist = mapObjectlist[i];
			for(size_t j = 0; j < iSize; ++j)
			{
				CCube* pCube = static_cast<CCube*>(Objectlist[j]);
				Engine::CTransform* pTransform = pCube->GetCubeData();
				CubeData.dwCubeType = pCube->GetCubeType();
				memcpy(CubeData.fAngle, pTransform->m_fAngle, sizeof(float) * 3);
				CubeData.vPos = pTransform->m_vPos;
				CubeData.vDir = pTransform->m_vDir;
				CubeData.vScale = pTransform->m_vScale;

				WriteFile(hFile, &CubeData, sizeof(Engine::CUBEDATA), &dwByte, NULL);
			}
		}
	}

	CloseHandle(hFile);
}

void CMapEdit::OnBnClickedLoadCubes()
{
	CFileDialog		Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.dat", this);

	if(Dlg.DoModal() == IDCANCEL)
		return;

	Engine::CRenderer* pRenderer = Engine::Get_Management()->GetRenderer();
	pRenderer->ResetRenderlist(Engine::RENDER_ALPHA);
	m_pCubeDataLayer->Release();
	m_pInputUnit->ResetData();

	TCHAR szDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirPath), szDirPath);
	SetCurrentDirectory(szDirPath);
	PathRemoveFileSpec(szDirPath);


	lstrcat(szDirPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir	= szDirPath;

	CString cstrPath = Dlg.GetPathName();

	HANDLE	hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD		dwByte = 0;

	typedef std::vector<Engine::CGameObject*>	OBJECTLIST;
	typedef std::map<DWORD, OBJECTLIST>			MAPOBJECTLIST;
	MAPOBJECTLIST& mapObjectlist = m_pCubeDataLayer->GetMapObjectlist();

	Engine::CUBEDATA CubeData;
	LPDIRECT3DDEVICE9 pDevice = Engine::Get_GraphicDev()->GetDevice();

	while (true)
	{
		ZeroMemory(&CubeData, sizeof(Engine::CUBEDATA));
		ReadFile(hFile, &CubeData, sizeof(Engine::CUBEDATA), &dwByte, NULL);

		if(dwByte == 0)
			break;

		CCube* pCube = NULL;

		switch(CubeData.dwCubeType)
		{
		case GAMEOBJECTID::CUBE_WHITE:
			pCube = CWhiteCube::Create(pDevice, m_pCubeDataLayer, GAMEOBJECTID::CUBE_WHITE);
			break;

		case GAMEOBJECTID::CUBE_GREEN:
			pCube = CGreenCube::Create(pDevice, m_pCubeDataLayer, GAMEOBJECTID::CUBE_GREEN);
			break;
			break;

		case GAMEOBJECTID::CUBE_PURPLE:
			pCube = CPurpleCube::Create(pDevice, m_pCubeDataLayer, GAMEOBJECTID::CUBE_PURPLE);
			break;

		case GAMEOBJECTID::CUBE_ORANGE:
			pCube = COrangeCube::Create(pDevice, m_pCubeDataLayer, GAMEOBJECTID::CUBE_ORANGE);
			break;

		case GAMEOBJECTID::CUBE_BLUE:
			pCube = CBlueCube::Create(pDevice, m_pCubeDataLayer, GAMEOBJECTID::CUBE_BLUE);
			break;

		case GAMEOBJECTID::CUBE_RED:
			pCube = CRedCube::Create(pDevice, m_pCubeDataLayer, GAMEOBJECTID::CUBE_RED);
			break;

		case GAMEOBJECTID::CUBE_YELLOW:
			pCube = CYellowCube::Create(pDevice, m_pCubeDataLayer, GAMEOBJECTID::CUBE_YELLOW);
			break;

		case GAMEOBJECTID::CUBE_GRAY:
			pCube = CGrayCube::Create(pDevice, m_pCubeDataLayer, GAMEOBJECTID::CUBE_GRAY);
			break;

		case GAMEOBJECTID::CUBE_WALL:
			pCube = CCubeWall::Create(pDevice, m_pCubeDataLayer, GAMEOBJECTID::CUBE_WALL);
			break;
		}

		Engine::CTransform* pTransform = pCube->GetCubeData();
		memcpy(&pTransform->m_fAngle, &CubeData.fAngle, sizeof(float) * 3);
		pTransform->m_vScale = CubeData.vScale;		pTransform->m_vDir = CubeData.vDir;
		pTransform->m_vPos = CubeData.vPos;
		pTransform->m_matWorld = CubeData.matWorld;
		pTransform->Update();

		m_pCubeDataLayer->AddObject(pCube->GetCubeType(), pCube);
		pRenderer->AddRenderObject(Engine::RENDER_ALPHA, pCube);
	}
	
	CloseHandle(hFile);

}

void CMapEdit::OnBnClickedRecordNavCell()
{
	UpdateData(TRUE);

	if(m_chkAdd)
	{
		m_chkAdd = 0;
		m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_ADD
			, (void*)&m_chkAdd, sizeof(BOOL));
	}

	if(m_chkEdit)
	{
		m_chkEdit = 0;
		m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_EDIT, (void*)&m_chkEdit, sizeof(BOOL));
	}

	if(m_chkEditNavCell)
	{
		m_chkEditNavCell = 0;
		m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_EDITNAVCELL, (void*)&m_chkEditNavCell, sizeof(BOOL));
	}

	if(m_chkReviseNavCell)
	{
		m_chkReviseNavCell = 0;
		m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_REVISENAVCELL, (void*)&m_chkReviseNavCell, sizeof(BOOL));
	}

	m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_RECORDNAVCELL, (void*)&m_chkRecordNavCell, sizeof(BOOL));

	UpdateData(FALSE);
}

void CMapEdit::OnBnClickedEditNavCell()
{
	UpdateData(TRUE);

	if(m_chkAdd)
	{
		m_chkAdd = 0;
		m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_ADD
			, (void*)&m_chkAdd, sizeof(BOOL));
	}

	if(m_chkEdit)
	{
		m_chkEdit = 0;
		m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_EDIT, (void*)&m_chkEdit, sizeof(BOOL));
	}

	if(m_chkRecordNavCell)
	{
		m_chkRecordNavCell = 0;
		m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_RECORDNAVCELL, (void*)&m_chkRecordNavCell, sizeof(BOOL));
	}

	if( !m_chkEditNavCell && m_chkReviseNavCell )
	{
		m_chkReviseNavCell = 0;
		m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_REVISENAVCELL, (void*)&m_chkReviseNavCell, sizeof(BOOL));
	}

	m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_EDITNAVCELL, (void*)&m_chkEditNavCell, sizeof(BOOL));

	UpdateData(FALSE);
}

void CMapEdit::OnBnClickedReviseNavCell()
{
	UpdateData(TRUE);

	if(m_chkAdd)
	{
		m_chkAdd = 0;
		m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_ADD
			, (void*)&m_chkAdd, sizeof(BOOL));
	}

	if(m_chkEdit)
	{
		m_chkEdit = 0;
		m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_EDIT, (void*)&m_chkEdit, sizeof(BOOL));
	}

	if(m_chkRecordNavCell)
	{
		m_chkRecordNavCell = 0;
		m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_RECORDNAVCELL, (void*)&m_chkRecordNavCell, sizeof(BOOL));
	}

	m_pMapEditSubject->NotifyMapEditInfo(CMapEditSubject::DATAMESSAGE_REVISENAVCELL, (void*)&m_chkReviseNavCell, sizeof(BOOL));
	
	UpdateData(FALSE);
}
void CMapEdit::OnBnClickedLinkNavCell()
{
	m_pNavMgr->LinkCell();
}

void CMapEdit::OnBnClickedDeleteNavCell()
{
	if(m_pInputUnit)
		m_pInputUnit->SetDeleteNavCell();
}

void CMapEdit::OnBnClickedSaveNavMesh()
{
	CFileDialog	Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.dat", this);

	if(Dlg.DoModal() == IDCANCEL)
		return;

	TCHAR szDirectoryPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirectoryPath), szDirectoryPath);
	SetCurrentDirectory(szDirectoryPath);
	PathRemoveFileSpec(szDirectoryPath);	

	lstrcat(szDirectoryPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szDirectoryPath;

	HANDLE	hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD		dwByte = 0;

	typedef std::vector<Engine::CNavCell*>  NAVMESH;
	NAVMESH* pNavMesh = Engine::Get_NavMgr()->GetNavMesh();

	if(NULL == pNavMesh->size())
		return;

	D3DXVECTOR3 vPoint[3];

	size_t iSize = pNavMesh->size();
	for(size_t i = 0; i < iSize; ++i)
	{
		const D3DXVECTOR3* pPoint = (*pNavMesh)[i]->GetPoint(Engine::CNavCell::POINT_A);
		memcpy(vPoint, pPoint, sizeof(D3DXVECTOR3) * 3);
		WriteFile(hFile, vPoint, sizeof(D3DXVECTOR3) * 3, &dwByte, NULL);
	}

	CloseHandle(hFile);
}

void CMapEdit::OnBnClickedLoadNavMesh()
{
	CFileDialog		Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.dat", this);

	if(Dlg.DoModal() == IDCANCEL)
		return;

	Engine::CNavMgr* pNavMgr = Engine::Get_NavMgr();
	pNavMgr->Release();
	
	TCHAR szDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirPath), szDirPath);
	SetCurrentDirectory(szDirPath);	
	PathRemoveFileSpec(szDirPath);	

	lstrcat(szDirPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir	= szDirPath;

	HANDLE	hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	DWORD		dwByte = 0;


	typedef std::vector<Engine::CNavCell*>  NAVMESH;
	NAVMESH* pNavMesh = Engine::Get_NavMgr()->GetNavMesh();


	D3DXVECTOR3 vPoint[3];
	ZeroMemory(vPoint, sizeof(D3DXVECTOR3) * 3);

	while (true)
	{
		ReadFile(hFile, vPoint, sizeof(D3DXVECTOR3) * 3, &dwByte, NULL);

		if(dwByte == 0)
			break;

		pNavMgr->AddCell(vPoint, vPoint + 1, vPoint + 2);
	}

	CloseHandle(hFile);
}
