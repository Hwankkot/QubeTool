#include "SphereColor.h"

#include "Engine_Function.h"
#include "Engine_Struct.h"


Engine::CSphereColor::CSphereColor(LPDIRECT3DDEVICE9 pDevice)
: CVIBuffer(pDevice)
, m_IsOrigin(false)
{

}

Engine::CSphereColor::CSphereColor(const CSphereColor& rhs)
: CVIBuffer(rhs)
{
	/// ��������ڷ� ���� ���� ���� Clone Sphere�� ����
	// �ּ��Դϴ�. 
	Release();
}

Engine::CSphereColor::~CSphereColor(void)
{

}

Engine::CSphereColor* Engine::CSphereColor::Create(LPDIRECT3DDEVICE9 pDevice, const float& fRadius)
{
	CSphereColor* pBuffer = new CSphereColor(pDevice);

	if(FAILED(pBuffer->CreateBuffer(fRadius)))
		Engine::Safe_Delete(pBuffer);

	return pBuffer;
}

HRESULT Engine::CSphereColor::CreateBuffer(const float& fRadius)
{
	D3DXCreateSphere(m_pDevice, fRadius, 20, 20, &m_pSphere, NULL);
	
	m_IsOrigin = true;

	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = m_pSphere->GetNumVertices();
	m_dwVtxFVF = VTXFVF_COL;

	m_dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 0.f);

	return S_OK;
}

void Engine::CSphereColor::Render(const D3DXMATRIX* pmatWorld)
{
	m_pDevice->SetTransform(D3DTS_WORLD, pmatWorld);
	m_pSphere->DrawSubset(0);
}

Engine::CResource* Engine::CSphereColor::CloneResource(void)
{
	/// �����͸� ������ Ŭ�� ��ü�� �����.
	CSphereColor* pCloneColor = new CSphereColor(*this);

	/// Ŭ�� �޽ø� �����.
	LPD3DXMESH pCloneSphere;
	m_pSphere->CloneMeshFVF(0, m_dwVtxFVF, m_pDevice, &pCloneSphere);

	/// Ŭ�� �޽��� �ּҸ� Ŭ�� ��ü�� �ִ´�.
	pCloneColor->m_pSphere = pCloneSphere;
	pCloneColor->m_IsOrigin = false;

	pCloneSphere->GetVertexBuffer(&pCloneColor->m_pVB);

	pCloneColor->AddRefCnt();

	// ���⵵ ����
	return  pCloneColor;
}

void Engine::CSphereColor::ChangeColor(DWORD dwColor)
{
	if(m_IsOrigin)
		return;

	LPDIRECT3DVERTEXBUFFER9 pVB = m_pVB;

	DWORD dwVtxNum = m_dwVtxCnt;
	VTXCOL* pVertex = NULL;
	
	pVB->Lock(0, 0, (void**)&pVertex, NULL);

	for(DWORD i = 0; i < dwVtxNum; ++i)
		pVertex[i].dwColor = dwColor;

	pVB->Unlock();
}
