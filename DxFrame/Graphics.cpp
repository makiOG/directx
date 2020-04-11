#include "Graphics.h"
#include <iostream>
#include"Window.h"

/**
*  ƒQ[ƒ€‚Ì‰Šú‰»ˆ—‚ðs‚¤
*
*  @param  ‰Šú‰»¬Œ÷Žžtrue‚ðAŽ¸”sŽžfalse‚ð•Ô‚·
*/
bool Graphics::Start(const WindowWeakPtr& window)
{

	_pDxs.window = window;
	auto hwnd = window.lock()->GetHWND();


	DXGI_SWAP_CHAIN_DESC scd = { 0 };
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hwnd;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = TRUE;
	D3D_FEATURE_LEVEL fl = D3D_FEATURE_LEVEL_11_0;
	
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &fl, 1, D3D11_SDK_VERSION, &scd, (IDXGISwapChain**)&_pDxs.pSwapChain, (ID3D11Device**)&_pDxs.pDevice, NULL, (ID3D11DeviceContext**)&_pDxs.pDeviceContext);
	// ----- •`‰ææ‚ÌÝ’è -----
	ID3D11Texture2D *pbbTex;
	_pDxs.pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pbbTex);

	
	_pDxs.pDevice->CreateRenderTargetView(pbbTex, NULL, (ID3D11RenderTargetView**)&_pDxs.pBackBuffer_RTV);
	pbbTex->Release();

	


	return true;    //‰Šú‰»¬Œ÷
}

/**
*  ƒQ[ƒ€‚Ì‰ð•úˆ—‚ðs‚¤
*/
void Graphics::End()
{
	/*
	pSwapChain->Release();
	pDeviceContext->Release();
	pDevice->Release();

	pBackBuffer_RTV->Release();
	*/

	PostQuitMessage(0);
}

/**
*  ƒQ[ƒ€‚PƒtƒŒ[ƒ€‚Ìˆ—‚ðs‚¤
*/
void Graphics::Update()
{
	// DX‚Ìƒ‹[ƒvˆ—
	float clearColor[4] = { 0.1, 0.1, 0.1, 1 };                          // ”wŒiF(R, G, B, A)
	//float clearColor[4] = { 1, 1, 1, 1 };                          // ”wŒiF(R, G, B, A)
	_pDxs.pDeviceContext->ClearRenderTargetView(_pDxs.pBackBuffer_RTV.get(), clearColor);  // ”wŒiF‚ÌÝ’è
}
//•`‰æ
void Graphics::Draw()
{
	// •`‰æŽÀs
	_pDxs.pSwapChain->Present(0, 0);
}

pDxs Graphics::GetDxPointers()const
{
	return _pDxs;
}
