/*!
 * \file StaticMesh.h
 * \date 2016/03/31 15:30
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

#ifndef StaticMesh_h__
#define StaticMesh_h__

/// Resources
#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMesh
	: public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit CStaticMesh(const CStaticMesh& rhs);

public:
	virtual	~CStaticMesh(void);

public:
	/// ����ƽ �޽ø� �����Ѵ�.
	/// ����ƽ �޽ð� �մ� ������ ���
	/// ����ƽ �޽� ������ �̸�
	static CStaticMesh* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath, const TCHAR* pFileName);

public:
	/// Ŭ���� �����Ѵ�.
	virtual CResource* CloneResource(void);

public:
	/// X���Ͽ��� Mesh�� �ҷ��´�.
	virtual HRESULT LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName);

	/// Render Mesh file on world coordinate system.
	virtual void RenderMesh(const D3DXMATRIX* pWorldMatrix);

private:
	void MakeBoundingBox(void);

public:
	virtual DWORD Release(void);

private:
	LPDIRECT3DTEXTURE9*		m_ppTexture;
	D3DMATERIAL9*			m_pMtrl;

private:
	LPD3DXMESH				m_pMesh;
	LPD3DXBUFFER			m_pSubset;
	DWORD					m_dwSubsetCnt;
	D3DMATERIAL9			m_Material;
};

END

#endif // StaticMesh_h__