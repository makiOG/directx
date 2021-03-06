#include "Graphics.h"
#include <iostream>
#include"Window.h"

/**
*  ゲームの初期化処理を行う
*
*  @param  初期化成功時trueを、失敗時falseを返す
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
	// ----- 描画先の設定 -----
	ID3D11Texture2D *pbbTex;
	_pDxs.pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pbbTex);

	
	_pDxs.pDevice->CreateRenderTargetView(pbbTex, NULL, (ID3D11RenderTargetView**)&_pDxs.pBackBuffer_RTV);
	pbbTex->Release();

	


	return true;    //初期化成功
}

/**
*  ゲームの解放処理を行う
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
*  ゲーム１フレームの処理を行う
*/
void Graphics::Update()
{
	// DXのループ処理
	float clearColor[4] = { 0.1, 0.1, 0.1, 1 };                          // 背景色(R, G, B, A)
	//float clearColor[4] = { 1, 1, 1, 1 };                          // 背景色(R, G, B, A)
	_pDxs.pDeviceContext->ClearRenderTargetView(_pDxs.pBackBuffer_RTV.get(), clearColor);  // 背景色の設定
}
//描画
void Graphics::Draw()
{
	// 描画実行
	_pDxs.pSwapChain->Present(0, 0);
}

pDxs Graphics::GetDxPointers()const
{
	return _pDxs;
}
