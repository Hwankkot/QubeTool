/*!
 * \file Input.h
 * \date 2016/03/30 15:21
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Input_h__
#define Input_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"

BEGIN(Engine)

class ENGINE_DLL CInput
{
	DECLARE_SINGLETON(CInput)

public:
	enum MOUSECLICK { DIM_LBUTTON, DIM_RBUTTON, DIM_MBUTTON };
	enum MOUSEAXIS { DIM_X, DIM_Y, DIM_Z };
	
public:
	static const DWORD KEY_LBUTTON;
	static const DWORD KEY_RBUTTON;	
	static const DWORD KEY_MBUTTON;	
	static const DWORD KEY_ENTER;	
	static const DWORD KEY_SPACE;	
	static const DWORD KEY_SHIFT;	
	static const DWORD KEY_W;		
	static const DWORD KEY_S;		
	static const DWORD KEY_A;		
	static const DWORD KEY_D;
	static const DWORD KEY_UPARROW;
	static const DWORD KEY_DOWNARROW;
	static const DWORD KEY_LEFTARROW;
	static const DWORD KEY_RIGHTARROW;
	static const DWORD KEY_Z;	
	static const DWORD KEY_X;	
	static const DWORD KEY_C;	

	
private:
	CInput(void);
	~CInput(void);

public:
	/// Create Direct8 Input com object.
	HRESULT InitInputDevice(HINSTANCE hInst, HWND hWnd);

private:
	/// Ű���� �ʱ�ȭ
	HRESULT InitKeyBoard(HWND hWnd);

	/// ���콺 �ʱ�ȭ
	HRESULT InitMouse(HWND hWnd);

public:
	/// �Է� ��ġ�� ���� ���¸� �޴´�.
	void GetInputState(void);

public:
	/// Ű���� �Է��� �޴´�.
	BYTE GetDIKeyState(BYTE KeyFlag) const;
	/// ���콺 �Է��� �޴´�.
	BYTE GetDIMouseState(MOUSECLICK KeyFlag) const;

public:
	/// ���콺 �̵� ���� �޴´�.
	long GetDIMouseMovement(MOUSEAXIS KeyFlag) const;

public:
	void UpdateKey(void);
	bool KeyPressed(DWORD dwKey);
	bool KeyDown(DWORD dwKey);
	bool KeyDrag(DWORD dwKey);
	bool KeyCombine(DWORD dwFirstKey, DWORD dwSecondKey);

public:
	inline DWORD GetKey(void);
	inline DWORD GetKeyPressed(void);
	inline DWORD GetKeyDown(void);

private:
	/// Release Com objects.
	void Release(void);

private:
	LPDIRECTINPUT8				m_pInput;
	LPDIRECTINPUTDEVICE8		m_pKeyBoard;
	LPDIRECTINPUTDEVICE8		m_pMouse;

private:
	DIMOUSESTATE m_MouseState;
	BYTE	m_byKeyState[256];
	DWORD	m_dwKey;
	DWORD	m_dwPressed;
	DWORD	m_dwKeyDown;
	DWORD	m_dwKeyDrag;
};	

END	

#endif // Input_h__


