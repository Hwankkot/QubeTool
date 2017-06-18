#include "stdafx.h"
#include "FreeCam.h"

#include "Engine_Function.h"
#include "Export_System.h"
#include "Value.h"

CFreeCam::CFreeCam(LPDIRECT3DDEVICE9 pDevice)
: Engine::CCamera(pDevice)
, m_pInput(NULL)
, m_bMouseFix(true)
, m_bClick(false)
, m_fCamSpeed(0.f)
{

}

CFreeCam::~CFreeCam(void)
{

}

CFreeCam* CFreeCam::Create(LPDIRECT3DDEVICE9 pDevice , const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	CFreeCam* pCamera = new CFreeCam(pDevice);
	if(FAILED(pCamera->InitFreeCam(pEye, pAt)))
		Safe_Delete(pCamera);

	return pCamera;
}

HRESULT CFreeCam::InitFreeCam(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	m_pInput = Engine::Get_Input();
	m_pTimeMgr = Engine::Get_TimeMgr();

	m_vEye = *pEye;
	m_vAt = *pAt;
	Invalidate_ViewMatrix();
	
	m_fAspect = float(WINCX) / WINCY;
	m_fFovY = D3DXToRadian(45.f);
	m_fNear = 1.f;
	m_fFar = 1000.f;
	Invalidate_ProjMatrix();

	m_fCamSpeed = 40.f;

	return S_OK;
}

void CFreeCam::Update(void)
{
	UpdateMovement();

	if(m_pInput->GetDIMouseState(Engine::CInput::DIM_RBUTTON) & 0x80)
		MouseMove();

	Engine::CCamera::Update();
}

void CFreeCam::UpdateMovement(void)
{
	float fTime = m_pTimeMgr->GetTime();

	if(m_pInput->GetDIKeyState(DIK_W))
	{
		D3DXVECTOR3		vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye += vLook * m_fCamSpeed * fTime;
		m_vAt += vLook * m_fCamSpeed * fTime;
	}

	if(m_pInput->GetDIKeyState(DIK_S))
	{
		D3DXVECTOR3		vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye -= vLook * m_fCamSpeed * fTime;
		m_vAt -= vLook * m_fCamSpeed * fTime;
	}

	if(m_pInput->GetDIKeyState(DIK_A))
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_vEye -= vRight * m_fCamSpeed * fTime;
		m_vAt -= vRight * m_fCamSpeed * fTime;
	}

	if(m_pInput->GetDIKeyState(DIK_D))
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_vEye += vRight * m_fCamSpeed * fTime;
		m_vAt += vRight * m_fCamSpeed * fTime;
	}
}

void CFreeCam::MouseMove(void)
{
	int		iDistance = 0;

	if(iDistance = m_pInput->GetDIMouseMovement(Engine::CInput::DIM_X))
	{
		D3DXMATRIX		matAxis;
		D3DXMatrixRotationAxis(&matAxis, &m_vUp, D3DXToRadian(iDistance * 0.1f));

		D3DXVECTOR3		vDir;
		vDir = m_vAt - m_vEye;
		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vAt = m_vEye + vDir;
	}

	if(iDistance = m_pInput->GetDIMouseMovement(Engine::CInput::DIM_Y))
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		D3DXMATRIX		matAxis;
		D3DXMatrixRotationAxis(&matAxis, &vRight, D3DXToRadian(iDistance * 0.1f));

		D3DXVECTOR3		vDir;
		vDir = m_vAt - m_vEye;
		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vAt = m_vEye + vDir;
	}
}