#include "StaticMesh.h"

#include "CubeColor.h"
#include "Engine_Function.h"
#include "VIBuffer.h"

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pDevice)
: CMesh(pDevice)
, m_ppTexture(NULL)
, m_pMtrl(NULL)
, m_pMesh(NULL)
, m_pSubset(NULL)
, m_dwSubsetCnt(0)
{

}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
: CMesh(rhs.m_pDevice)
, m_ppTexture(rhs.m_ppTexture)
, m_pMtrl(rhs.m_pMtrl)
, m_pMesh(rhs.m_pMesh)
, m_pSubset(rhs.m_pSubset)
, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
{
	m_StdVtx3 = rhs.m_StdVtx3;
	m_pBoundingBox = rhs.m_pBoundingBox;
}

Engine::CStaticMesh::~CStaticMesh(void)
{
	
}

Engine::CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath, const TCHAR* pFileName)
{
	CStaticMesh* pMesh = new CStaticMesh(pDevice);
	

	if(FAILED(pMesh->LoadMeshFromFile(pPath, pFileName)))
		Safe_Delete(pMesh);

	pMesh->MakeBoundingBox();

	return pMesh;
}

Engine::CResource* Engine::CStaticMesh::CloneResource(void)
{
	CStaticMesh* pMesh = new CStaticMesh(*this);
	
	pMesh->AddRefCnt();

	return pMesh;
}

HRESULT Engine::CStaticMesh::LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName)
{
	HRESULT  hr = NULL;
	TCHAR szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pPath);
	lstrcat(szFullPath, pFileName);

	/// �޽� ���� ���� ������� ������ ������ �޾ƿ´�. 
	/// szFullPath: �޽� ������ ��� + �̸� 
	/// D3DMESH_MANAGED: �޽� ���� �ɼ�, �޽� ������ ���� ���
	/// m_pDevice: �޽ÿ� ������ ����̽� �ּ�
	/// m_pSubset: ����� �ּҸ� ���� ������
	/// m_dwSubSetCnt: ����� ����
	/// m_pMesh: �޽� ������ ���� ������
	hr = D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, m_pDevice, NULL, &m_pSubset, NULL
		, &m_dwSubsetCnt, &m_pMesh);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, szFullPath);

	// ����� �����͸� D3DXMATERIAL(����� ����ü)�� ������ ���·� ��ȯ�Ѵ�.
	D3DXMATERIAL* pSubset = ((D3DXMATERIAL*)m_pSubset->GetBufferPointer());

	//// ������� ���� ����(D3DMATERIAL9)�� ���� ����ü �迭�� �����.
	m_pMtrl = new D3DMATERIAL9[m_dwSubsetCnt];
	//// Texture Com ��ü�� ���� ������ �迭�� �����.
	m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	for(DWORD i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pMtrl[i] = pSubset->MatD3D;

		TCHAR  szFileName[128] = L"";

		lstrcpy(szFullPath, pPath);
		
		// ���ڰ����� ���� MultiByteChar�� WideChar������ ��ȯ�Ѵ�. 

		if(pSubset->pTextureFilename != NULL)
		{
			MultiByteToWideChar(CP_ACP
				, 0, pSubset[i].pTextureFilename
				, strlen(pSubset[i].pTextureFilename)
				, szFileName, 128);
			lstrcat(szFullPath, szFileName);

			// Texture com ��ü�� �����ϰ� �����͸� ��´�.
			//hr = D3DXCreateTextureFromFileEx(m_pDevice, szFullPath, 0, 0, 0, D3DUSAGE_RENDERTARGET, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_ppTexture[i]);
			hr = D3DXCreateTextureFromFile(m_pDevice, szFullPath, &m_ppTexture[i]);
			FAILED_CHECK(hr);
		}
	}

	m_Material.Diffuse	= D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f);
	m_Material.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f);
	m_Material.Ambient	= D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f);
	m_Material.Emissive = D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f);
	m_Material.Power = 0.2f;
	
	return S_OK;
}

void Engine::CStaticMesh::RenderMesh(const D3DXMATRIX* pWorldMatrix)
{
	m_pDevice->SetTransform(D3DTS_WORLD, pWorldMatrix);

	for(DWORD i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pDevice->SetTexture(0, m_ppTexture[i]);
		m_pDevice->SetMaterial(&m_Material);
		m_pMesh->DrawSubset(i);
	}
	m_pBoundingBox->Render(pWorldMatrix);
}

DWORD Engine::CStaticMesh::Release(void)
{
	if(--m_dwRefCnt == 0)
	{
		Safe_Delete_Array(m_pMtrl);
		
		for(DWORD i = 0; i < m_dwRefCnt; ++i)
		{
			/// Release texture objects.
			Safe_Release(m_ppTexture[i]);
		}

		/// Delete pointer array.
		Safe_Delete_Array(m_ppTexture);
		Safe_Delete(m_pBoundingBox);

		Safe_Release(m_pMesh);
		Safe_Release(m_pSubset);
		return 0;
	}
	else
		--m_dwRefCnt;

	return m_dwRefCnt;
}

void Engine::CStaticMesh::MakeBoundingBox(void)
{
	/// �޽��� ���ؽ� ���� Ÿ���� �ҷ��ͼ� �����Ѵ�.
	DWORD	dwFVF = m_pMesh->GetFVF();

	/// �ȿ� XYZ������ ����ִ��� Ȯ���Ѵ�. 
	if(dwFVF & D3DFVF_XYZ)
	{
		/// ������������ �Է� ���� �����͸� �����ϴ� ����ü�� �����Ѵ�.
		D3DVERTEXELEMENT9		VtxFVF[MAX_FVF_DECL_SIZE];

		/// �޽ÿ� ���ԵǾ��ִ� ������ ������� �ҷ��´�. 
		m_pMesh->GetDeclaration(VtxFVF);

		int		iIndex = 0;
		DWORD	dwOffset = 0;

		while(true)
		{
			/// �޽��� ������ ������ ����ִ� �������� ã�Ƽ� �����Ѵ�.
			if(VtxFVF[iIndex].Usage == D3DDECLUSAGE_POSITION)
			{
				dwOffset = VtxFVF[iIndex].Offset;
				break;
			}
			++iIndex;
		}

		void* pVtxInfo = NULL;

		m_pMesh->LockVertexBuffer(0,  &pVtxInfo);

		BYTE*		pByteVtxInfo = (BYTE*)pVtxInfo;
		pVtxInfo = &pByteVtxInfo[dwOffset];

		/// �޽� ���� ���ؽ��� ������ �ҷ��´�.
		DWORD	dwNumVertices = m_pMesh->GetNumVertices();

		/// �޽� ���� ���ؽ� ���� ������ �ҷ��´�.
		DWORD	dwStride = D3DXGetFVFVertexSize(dwFVF);

		/// �ٿ�� �ڽ��� ������ Min, Max ������ �����Ѵ�.
		D3DXComputeBoundingBox((D3DXVECTOR3*)pVtxInfo, dwNumVertices, dwStride, &m_StdVtx3._min, &m_StdVtx3._max);

		m_pMesh->UnlockVertexBuffer();

		m_pBoundingBox = CCubeColor::Create(m_pDevice, m_StdVtx3, D3DXCOLOR(0.f,  1.f, 0.f, 1.f));
	}
}