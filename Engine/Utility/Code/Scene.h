/*!
 * \file Scene.h
 * \date 2016/04/05 13:01
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

#ifndef Scene_h__
#define Scene_h__

#include "Engine_Include.h"
#include "Engine_Macro.h"

BEGIN(Engine)

class CLayer;

class ENGINE_DLL CScene
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pDevice);
	
private:
	explicit CScene(const CScene&);
	CScene& operator = (const CScene&);

public:
	virtual ~CScene(void);

public:
	virtual void Update(void);

private:
	void Release(void);

protected:
	LPDIRECT3DDEVICE9		m_pDevice;

protected:
	typedef std::map<DWORD, CLayer*> MAPLAYER;
	MAPLAYER  m_mapLayer;
};

END

#endif // Scene_h__