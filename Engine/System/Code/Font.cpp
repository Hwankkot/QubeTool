#include "Font.h"

#include "Engine_Function.h"

Engine::CFont::CFont(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
, m_pFont(NULL)
, m_iHeight(0)
, m_iWidth(0)
{

}

Engine::CFont::~CFont(void)
{
	Release();
}

Engine::CFont* Engine::CFont::Create(LPDIRECT3DDEVICE9 pDevice , const UINT& iHeight, const UINT& iWidth, const UINT& iWeight , const TCHAR* pFaceName)
{
	CFont*	pFont = new CFont(pDevice);
	if(FAILED(pFont->InitFont(iHeight, iWidth, iWeight, pFaceName)))
		Safe_Delete(pFont);

	return pFont;
}

HRESULT Engine::CFont::InitFont(const UINT& iHeight, const UINT& iWidth
								, const UINT& iWeight , const TCHAR* pFaceName)
{
	/// Reset font structure.
	D3DXFONT_DESC		Font_Desc;
	ZeroMemory(&Font_Desc, sizeof(D3DXFONT_DESC));

	Font_Desc.Height = m_iHeight = iHeight;
	Font_Desc.Width = m_iWidth = iWidth;
	Font_Desc.Weight = iWeight;
	lstrcpy(Font_Desc.FaceName, pFaceName);
	Font_Desc.CharSet = DEFAULT_CHARSET;

	FAILED_CHECK(D3DXCreateFontIndirect(m_pDevice, &Font_Desc, &m_pFont));
	return S_OK;
}

void Engine::CFont::Render(const TCHAR* pString, D3DXCOLOR Color, const D3DXVECTOR3& vPos)
{
	RECT rc = {(long)vPos.x, (long)vPos.y
		, (long)vPos.x + m_iWidth * lstrlen(pString), (long)vPos.y + m_iHeight};

	m_pFont->DrawTextW(NULL, pString, lstrlen(pString), &rc, DT_NOCLIP, Color);
}

void Engine::CFont::Release(void)
{
	Engine::Safe_Release(m_pFont);
}


