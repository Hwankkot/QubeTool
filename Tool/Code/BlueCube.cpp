#include "stdafx.h"
#include "BlueCube.h"

#include "Engine_Function.h"
#include "Engine_Struct.h"
#include "Enum.h"
#include "Export_Resources.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Layer.h"
#include "MapEditObserver.h"
#include "MapEditSubject.h"  
#include "MouseObserver.h"
#include "MouseSubject.h"
#include "RayCollision.h"
#include "StaticMesh.h"
#include "Transform.h"
#include "VIBuffer.h"
#include "Value.h"

CBlueCube::CBlueCube(LPDIRECT3DDEVICE9 pDevice)
: CCube(pDevice)
, m_pMouseObserver(NULL)
, m_pMapEditObserver(NULL)
{

}

CBlueCube::~CBlueCube(void)
{
	Release();
}

CBlueCube* CBlueCube::Create(LPDIRECT3DDEVICE9 pDevice, Engine::CLayer* pLayer, const DWORD& dwCubeType)
{
	CBlueCube* pGameObject = new CBlueCube(pDevice);

	if(FAILED(pGameObject->InitCube(pLayer, dwCubeType)))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}   

HRESULT CBlueCube::InitCube(Engine::CLayer* pLayer, const DWORD& dwCubeType)
{
	m_pInput = Engine::Get_Input();
	m_pRenderer = Engine::Get_Management()->GetRenderer();
	m_pResourceMgr = Engine::Get_ResourceMgr();

	m_dwCubeType = dwCubeType;

	FAILED_CHECK(AddComponent());

	m_pLayer = pLayer;

	/// 정해진 박스 스케일을 받는다.
	float fCubeScale = CBlueCube::CUBESCALE;
	m_pInfo->m_vScale = D3DXVECTOR3(fCubeScale, fCubeScale, fCubeScale);

	m_pMouseObserver = CMouseObserver::Create(CMouseSubject::GetInstance());
	CMouseSubject::GetInstance()->Subscribe(m_pMouseObserver);

	m_pMapEditObserver = CMapEditObserver::Create(CMapEditSubject::GetInstance());
	CMapEditSubject::GetInstance()->Subscribe(m_pMapEditObserver);

	return S_OK;
}

void CBlueCube::Update(void)
{  
	if(m_IsIntersectedtoSelect)
	{
		m_IsIntersectedtoSelect = !m_IsIntersectedtoSelect;

		BOOL IsAdd = 0;
		m_pMapEditObserver->GetData(CMapEditSubject::DATAMESSAGE_ADD, (void*)&IsAdd, CMapEditSubject::DATATYPE_BOOL, sizeof(BOOL));

		if(IsAdd)
			UpdateCollision();
	}

	BOOL IsEdit;
	m_pMapEditObserver->GetData(CMapEditSubject::DATAMESSAGE_EDIT, (void*)&IsEdit, CMapEditSubject::DATATYPE_BOOL, sizeof(BOOL));

	if(IsEdit && m_IsSelected)
		UpdateMovement();

	Engine::CGameObject::Update();
	Compute_ViewZ(m_pInfo->m_vPos);
}

void CBlueCube::Render(void)
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	memcpy(&matWorld.m[3][0], &m_pInfo->m_matWorld.m[3][0], sizeof(D3DXVECTOR3));

	m_pSphere->Render(&matWorld);
	m_pMesh->RenderMesh(&m_pInfo->m_matWorld);
}

HRESULT CBlueCube::AddComponent(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(COMPONENTID::TRANSFORM, pComponent));

	// Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCEID::STATICMESH_CUBE_BLUE);
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(COMPONENTID::MESH_STATIC, pComponent));

	// Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCEID::COLBUFFER_SPHERE);
	m_pSphere = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pSphere->ChangeColor(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));
	m_mapComponent.insert(MAPCOMPONENT::value_type(COMPONENTID::COLBUFFER_SPHERE, pComponent));

	// RayCollision
	pComponent = m_pRayCollision = Engine::CRayCollision::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(COMPONENTID::COLLISION_RAY, pComponent));

	return S_OK;
} 

void CBlueCube::UpdateCollision(void)
{ 
	Engine::STANDARD_VERTEX3 StdVtx3;
	m_pMesh->GetMinMax(&StdVtx3);

	Engine::RAY3 Ray3;
	m_pMouseObserver->GetData(CMouseSubject::DATAMESSAGE_RAY, (void*)&Ray3, CMouseSubject::DATATYPE_FLOAT, sizeof(Engine::RAY3));

	D3DXMATRIX matInvWorld;
	D3DXMatrixInverse(&matInvWorld, NULL, &m_pInfo->m_matWorld);

	D3DXVec3TransformCoord(&Ray3.pos, &Ray3.pos, &matInvWorld);
	D3DXVec3TransformNormal(&Ray3.dir, &Ray3.dir, &matInvWorld);

	D3DXVECTOR3 vPos;
	WORD wEdge;

	if(m_pRayCollision->IsRayIntersectedBox(StdVtx3, Ray3, &vPos, &wEdge))
	{ // if
		CBlueCube* pCube = NULL;

		switch(wEdge)
		{ // switch
		case PLANE_TOP:
			pCube = CBlueCube::Create(m_pDevice, m_pLayer, m_dwCubeType);
			pCube->m_pInfo->m_vPos = m_pInfo->m_vPos;
			pCube->m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + CBlueCube::CUBERADIUS;
			break;

		case PLANE_BOT:
			pCube = CBlueCube::Create(m_pDevice, m_pLayer, m_dwCubeType);
			pCube->m_pInfo->m_vPos = m_pInfo->m_vPos;
			pCube->m_pInfo->m_vPos.y = m_pInfo->m_vPos.y - CBlueCube::CUBERADIUS;
			break;

		case PLANE_LEFT:
			pCube = CBlueCube::Create(m_pDevice, m_pLayer, m_dwCubeType);
			pCube->m_pInfo->m_vPos = m_pInfo->m_vPos;
			pCube->m_pInfo->m_vPos.x = m_pInfo->m_vPos.x - CBlueCube::CUBERADIUS;
			break;

		case PLANE_RIGHT:
			pCube = CBlueCube::Create(m_pDevice, m_pLayer, m_dwCubeType);
			pCube->m_pInfo->m_vPos = m_pInfo->m_vPos;
			pCube->m_pInfo->m_vPos.x = m_pInfo->m_vPos.x + CBlueCube::CUBERADIUS;
			break;

		case PLANE_FRONT:
			pCube = CBlueCube::Create(m_pDevice, m_pLayer, m_dwCubeType);
			pCube->m_pInfo->m_vPos = m_pInfo->m_vPos;
			pCube->m_pInfo->m_vPos.z = m_pInfo->m_vPos.z - CBlueCube::CUBERADIUS;
			break;

		case PLANE_BACK:
			pCube = CBlueCube::Create(m_pDevice, m_pLayer, m_dwCubeType);
			pCube->m_pInfo->m_vPos = m_pInfo->m_vPos;
			pCube->m_pInfo->m_vPos.z = m_pInfo->m_vPos.z + CBlueCube::CUBERADIUS;
			break;
		} // Switch

		if(pCube)
		{
			m_pLayer->AddObject(GAMEOBJECTID::CUBE_WHITE, pCube);
			m_pRenderer->AddRenderObject(Engine::RENDER_ALPHA, pCube);
		}
	} // if
}

void CBlueCube::UpdateMovement(void)
{
	if(m_pInput->KeyCombine(Engine::CInput::KEY_Z, Engine::CInput::KEY_UPARROW))
	{
		m_pInfo->m_vPos.z += CCube::CUBERADIUS;
	}

	if(m_pInput->KeyCombine(Engine::CInput::KEY_Z, Engine::CInput::KEY_DOWNARROW))
	{
		m_pInfo->m_vPos.z -= CCube::CUBERADIUS;
	}

	if(m_pInput->KeyCombine(Engine::CInput::KEY_X, Engine::CInput::KEY_UPARROW))
	{
		m_pInfo->m_vPos.x += CCube::CUBERADIUS;
	}

	if(m_pInput->KeyCombine(Engine::CInput::KEY_X, Engine::CInput::KEY_DOWNARROW))
	{
		m_pInfo->m_vPos.x -= CCube::CUBERADIUS;
	}

	if(m_pInput->KeyCombine(Engine::CInput::KEY_C, Engine::CInput::KEY_UPARROW))
	{
		m_pInfo->m_vPos.y += CCube::CUBERADIUS;
	}

	if(m_pInput->KeyCombine(Engine::CInput::KEY_C, Engine::CInput::KEY_DOWNARROW))
	{
		m_pInfo->m_vPos.y -= CCube::CUBERADIUS;
	}

	if(m_pInput->KeyCombine(Engine::CInput::KEY_Z, Engine::CInput::KEY_RIGHTARROW))
	{
		m_pInfo->m_fAngle[Engine::ANGLE_Z] -= D3DXToRadian(15.f);
	}

	if(m_pInput->KeyCombine(Engine::CInput::KEY_Z, Engine::CInput::KEY_LEFTARROW))
	{
		m_pInfo->m_fAngle[Engine::ANGLE_Z] += D3DXToRadian(15.f);
	}

	if(m_pInput->KeyCombine(Engine::CInput::KEY_X, Engine::CInput::KEY_RIGHTARROW))
	{
		m_pInfo->m_fAngle[Engine::ANGLE_X] -= D3DXToRadian(15.f);
	}

	if(m_pInput->KeyCombine(Engine::CInput::KEY_X, Engine::CInput::KEY_LEFTARROW))
	{
		m_pInfo->m_fAngle[Engine::ANGLE_X] += D3DXToRadian(15.f);
	}

	if(m_pInput->KeyCombine(Engine::CInput::KEY_C, Engine::CInput::KEY_RIGHTARROW))
	{
		m_pInfo->m_fAngle[Engine::ANGLE_Y] -= D3DXToRadian(15.f);
	}

	if(m_pInput->KeyCombine(Engine::CInput::KEY_C, Engine::CInput::KEY_LEFTARROW))
	{
		m_pInfo->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(15.f);
	}
}

bool CBlueCube::IsIntersectedtoSelect(D3DXVECTOR3* pPos, WORD* pEdge)
{
	Engine::RAY3 Ray3;
	m_pMouseObserver->GetData(CMouseSubject::DATAMESSAGE_RAY, (void*)&Ray3, CMouseSubject::DATATYPE_FLOAT, sizeof(Engine::RAY3));

	D3DXVECTOR3 vPos;

	if(m_IsIntersectedtoSelect = m_pRayCollision->IsRayIntersectedSphere(Ray3, m_pInfo->m_vPos, CCube::CUBERADIUS, &vPos))
	{
		m_pSphere->ChangeColor(D3DXCOLOR(0.f, 1.f, 0.f, 1.f));

		D3DXMATRIX matInvWorld;
		D3DXMatrixInverse(&matInvWorld, NULL, &m_pInfo->m_matWorld);

		D3DXVec3TransformCoord(&Ray3.pos, &Ray3.pos, &matInvWorld);
		D3DXVec3TransformNormal(&Ray3.dir, &Ray3.dir, &matInvWorld);

		Engine::STANDARD_VERTEX3 StdVtx3;
		m_pMesh->GetMinMax(&StdVtx3);
		m_pRayCollision->IsRayIntersectedBox(StdVtx3, Ray3, pPos, pEdge);
		m_IsSelected = true;
	}
	else
		m_pSphere->ChangeColor(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

	return m_IsIntersectedtoSelect;
}

bool CBlueCube::IsIntersectedtoMakeNavCell(D3DXVECTOR3* pPos)
{
	Engine::RAY3 Ray3;
	m_pMouseObserver->GetData(CMouseSubject::DATAMESSAGE_RAY, (void*)&Ray3, CMouseSubject::DATATYPE_FLOAT, sizeof(Engine::RAY3));

	D3DXMATRIX matInvWorld;
	D3DXMatrixInverse(&matInvWorld, NULL, &m_pInfo->m_matWorld);

	D3DXVec3TransformCoord(&Ray3.pos, &Ray3.pos, &matInvWorld);
	D3DXVec3TransformNormal(&Ray3.dir, &Ray3.dir, &matInvWorld);

	Engine::STANDARD_VERTEX3 StdVtx3;
	m_pMesh->GetMinMax(&StdVtx3);

	bool IsIntersectedtoSelect = m_pRayCollision->IsRayIntersectedtoMakeNavMesh(StdVtx3, Ray3, pPos);

	if(IsIntersectedtoSelect)
		D3DXVec3TransformCoord(pPos, pPos, &m_pInfo->m_matWorld);

	return IsIntersectedtoSelect;
}

void CBlueCube::ReleaseSelect(void)
{
	m_pSphere->ChangeColor(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));
	m_IsSelected = false; 
}

Engine::CTransform* CBlueCube::GetCubeData(void)
{
	return m_pInfo;
}

void CBlueCube::Release(void)
{
	CMouseSubject::GetInstance()->Unsubscribe( m_pMouseObserver->GetIndex(), m_pMouseObserver);
	CMapEditSubject::GetInstance()->Unsubscribe(m_pMapEditObserver->GetIndex(), m_pMapEditObserver);
	Engine::Safe_Delete(m_pMouseObserver);
	Engine::Safe_Delete(m_pMapEditObserver);
}

