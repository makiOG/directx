#pragma once

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment(lib,"windowscodecs.lib")//---���ǉ�---
#include <d3d11_1.h>
#include <directxcolors.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wchar.h>
#include <wincodec.h>//---���ǉ�---
#include"WICTextureLoader/WICTextureLoader.h"

class DirectX11
{
public:
	DirectX11();
	~DirectX11();
	HRESULT InitDevice();
	void Render();
private:
	ID3D11Device* pd3dDevice;
	ID3D11Device1* pd3dDevice1;
	ID3D11DeviceContext* pImmediateContext;
	ID3D11DeviceContext1* pImmediateContext1;
	IDXGISwapChain* pSwapChain;
	IDXGISwapChain1* pSwapChain1;
	ID3D11RenderTargetView* pRenderTargetView;

	ID2D1Factory* pD2DFactory;
	IDWriteFactory* pDWriteFactory;
	IDWriteTextFormat* pTextFormat;
	ID2D1RenderTarget* pRT;
	ID2D1SolidColorBrush* pSolidBrush;
	IDXGISurface* pDXGISurface;

	IWICImagingFactory* pWICImagingFactory;//---���ǉ�---
	IWICBitmapDecoder* pWICBitmapDecoder;//---���ǉ�---
	IWICBitmapFrameDecode* pWICBitmapFrameDecode;//---���ǉ�---
	IWICFormatConverter* pWICFormatConverter;//---���ǉ�---
	ID2D1Bitmap* pD2DBitmap;//---���ǉ�---
};