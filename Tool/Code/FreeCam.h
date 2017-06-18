/*!
 * \file FreeCam.h
 * \date 2016/04/29 16:36
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

#ifndef FreeCam_h__
#define FreeCam_h__

#include "Camera.h"

namespace Engine
{ 
	class CInput; 
	class CTimeMgr;
}

class CFreeCam : public Engine::CCamera
{
private:
	explicit CFreeCam(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CFreeCam(void);

public:
	static CFreeCam* Create(LPDIRECT3DDEVICE9 pDevice
		, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);

private:
	HRESULT InitFreeCam(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);

public:
	virtual void Update(void);

private:
	void UpdateMovement(void);
	void FixMouse(void);
	void MouseMove(void);

private:
	Engine::CInput*		m_pInput;
	Engine::CTimeMgr*	m_pTimeMgr;

private:
	bool  m_bMouseFix;
	bool  m_bClick;
	float m_fCamSpeed;
};

#endif // FreeCam_h__