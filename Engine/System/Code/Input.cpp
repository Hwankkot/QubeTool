#include "Input.h"

#include "Engine_Function.h"

const DWORD Engine::CInput::KEY_LBUTTON		= 0x00000001;
const DWORD Engine::CInput::KEY_RBUTTON		= 0x00000002;
const DWORD Engine::CInput::KEY_MBUTTON		= 0x00000004;
const DWORD Engine::CInput::KEY_ENTER		= 0x00000008;
const DWORD Engine::CInput::KEY_SPACE		= 0x00000010;
const DWORD Engine::CInput::KEY_SHIFT		= 0x00000020;
const DWORD Engine::CInput::KEY_W			= 0x00000040;
const DWORD Engine::CInput::KEY_S			= 0x00000080;
const DWORD Engine::CInput::KEY_A			= 0x00000100;
const DWORD Engine::CInput::KEY_D			= 0x00000200;
const DWORD Engine::CInput::KEY_UPARROW		= 0x00000400;
const DWORD Engine::CInput::KEY_DOWNARROW	= 0x00000800;
const DWORD Engine::CInput::KEY_LEFTARROW	= 0x00001000;
const DWORD Engine::CInput::KEY_RIGHTARROW	= 0x00002000;
const DWORD Engine::CInput::KEY_Z			= 0x00004000;
const DWORD Engine::CInput::KEY_X			= 0x00008000;
const DWORD Engine::CInput::KEY_C			= 0x00010000;



IMPLEMENT_SINGLETON(Engine::CInput)

Engine::CInput::CInput(void)
: m_pInput(NULL)
, m_pKeyBoard(NULL)
, m_pMouse(NULL)
, m_dwKey(0)
, m_dwPressed(0)
, m_dwKeyDown(0)
, m_dwKeyDrag(0)
{
	ZeroMemory(m_byKeyState, sizeof(BYTE) * 256);
	ZeroMemory(&m_MouseState, sizeof(DIMOUSESTATE));
}

Engine::CInput::~CInput(void)
{
	Release();
}

HRESULT Engine::CInput::InitInputDevice(HINSTANCE hInst, HWND hWnd)
{
	HRESULT		hr = DirectInput8Create(hInst
		, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL);
	FAILED_CHECK(hr);

	hr = InitKeyBoard(hWnd);
	FAILED_CHECK(hr);

	hr = InitMouse(hWnd);
	FAILED_CHECK(hr);
	return S_OK;
}

HRESULT Engine::CInput::InitKeyBoard(HWND hWnd)
{
	HRESULT		hr = NULL;

	hr = m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL); FAILED_CHECK(hr);

	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyBoard->Acquire();

	return S_OK;
}

HRESULT Engine::CInput::InitMouse(HWND hWnd)
{
	HRESULT		hr = NULL;

	hr = m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL); FAILED_CHECK(hr);

	m_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->Acquire();

	return S_OK;
}

void Engine::CInput::GetInputState(void)
{
	HRESULT hr = NULL;

	/// 현재 장치의 정보를 받는다.
	hr = m_pKeyBoard->GetDeviceState(256, m_byKeyState);

	/// 장치를 잃어버렸으면 다시 받는다.
	if(hr == DIERR_INPUTLOST)
	{
		/// 권한을 다시 얻을 때 까지 루프
		while(hr != S_OK)
			hr = m_pKeyBoard->Acquire();

		m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	}

	hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);

	if(hr == DIERR_INPUTLOST)
	{
		/// 권한을 다시 얻을 때 까지 루프
		while(hr != S_OK)
			hr = m_pMouse->Acquire();

		m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);
	}
}

BYTE Engine::CInput::GetDIKeyState(BYTE KeyFlag) const
{
	/// KeyFlag값이 현재 들어와 있는지 확인한다.
	return m_byKeyState[KeyFlag];
}

BYTE Engine::CInput::GetDIMouseState(MOUSECLICK KeyFlag) const
{
	/// 마우스 버튼 입력을 받는다.
	return m_MouseState.rgbButtons[KeyFlag];
}

long Engine::CInput::GetDIMouseMovement(MOUSEAXIS KeyFlag) const
{
	return *(((long*)&m_MouseState) + KeyFlag);
}

void Engine::CInput::UpdateKey(void)
{
	// 사용자가 입력한 마우스와 키보드의 상태를 확인한다.
	GetInputState();
	
	m_dwKey = 0;
	
	if(GetDIMouseState(DIM_LBUTTON) & 0x80)
		m_dwKey |= CInput::KEY_LBUTTON;
	
	if(GetDIMouseState(DIM_RBUTTON) & 0x80)
		m_dwKey |= CInput::KEY_RBUTTON;
	
	if(GetDIMouseState(DIM_MBUTTON) & 0x80)
		m_dwKey |= CInput::KEY_MBUTTON;

	if(GetDIKeyState(DIK_W) & 0x80)
		m_dwKey |= CInput::KEY_W;

	if(GetDIKeyState(DIK_S) & 0x80)
		m_dwKey |= CInput::KEY_S;

	if(GetDIKeyState(DIK_A) & 0x80)
		m_dwKey |= CInput::KEY_A;

	if(GetDIKeyState(DIK_D) & 0x80)
		m_dwKey |= CInput::KEY_D;

	if(GetDIKeyState(DIK_UPARROW) & 0x80)
		m_dwKey |= CInput::KEY_UPARROW;

	if(GetDIKeyState(DIK_DOWNARROW) & 0x80)
		m_dwKey |= CInput::KEY_DOWNARROW;

	if(GetDIKeyState(DIK_LEFTARROW) & 0x80)
		m_dwKey |= CInput::KEY_LEFTARROW;

	if(GetDIKeyState(DIK_RIGHTARROW) & 0x80)
		m_dwKey |= CInput::KEY_RIGHTARROW;

	if(GetDIKeyState(DIK_Z) & 0x80)
		m_dwKey |= CInput::KEY_Z;

	if(GetDIKeyState(DIK_X) & 0x80)
		m_dwKey |= CInput::KEY_X;

	if(GetDIKeyState(DIK_C) & 0x80)
		m_dwKey |= CInput::KEY_C;
}

bool Engine::CInput::KeyPressed(DWORD dwKey)
{				
	if(m_dwKey & dwKey)
	{
		m_dwPressed |= dwKey;
		return false;
	}
	else if(m_dwPressed & dwKey) 
	{
		m_dwPressed = m_dwKey;
		return true;
	}
	return false;
}

bool Engine::CInput::KeyDown(DWORD dwKey)				
{															
	if(KeyPressed(dwKey))
	{	
		m_dwKeyDown ^= dwKey;
	}

	if(!(m_dwKeyDown & dwKey))			
	{									
		if(m_dwKey & dwKey)
		{	 
			m_dwKeyDown  |= dwKey;
			return true;
		}
	}
	return false;
}

bool Engine::CInput::KeyDrag(DWORD dwKey)
{
	if(!(m_dwKey & dwKey))
	{
		if(m_dwKeyDrag & dwKey)
			m_dwKeyDrag ^= dwKey;

		return false;
	}

	else if((m_dwKey & dwKey) && !(m_dwKeyDrag & dwKey))
	{
		m_dwKeyDrag |= dwKey;

		return false;
	}

	if(m_dwKeyDrag & m_dwKey)
		return true;

	return false;
}

bool Engine::CInput::KeyCombine(DWORD dwFirstKey, DWORD dwSecondKey)
{
	if(m_dwKey & dwFirstKey)
	{
		if(KeyDown(dwSecondKey))
		{
			return true;
		}
	}

	return false;
}

DWORD Engine::CInput::GetKey(void)
{
	return m_dwKey;
}

DWORD Engine::CInput::GetKeyDown(void)
{
	return m_dwKeyDown;
}

DWORD Engine::CInput::GetKeyPressed(void)
{
	return m_dwPressed;
}

void Engine::CInput::Release(void)
{
	Safe_Release(m_pMouse);
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pInput);
}