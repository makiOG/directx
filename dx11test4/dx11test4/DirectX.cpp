#include "Main.h"
#include "DirectX.h"
#include"WICTextureLoader/WICTextureLoader.cpp"

DirectX11::DirectX11()
{
	pd3dDevice = nullptr;
	pd3dDevice1 = nullptr;
	pImmediateContext = nullptr;
	pImmediateContext1 = nullptr;
	pSwapChain = nullptr;
	pSwapChain1 = nullptr;
	pRenderTargetView = nullptr;

	pD2DFactory = nullptr;
	pDWriteFactory = nullptr;
	pTextFormat = nullptr;
	pRT = nullptr;
	pSolidBrush = nullptr;
	pDXGISurface = nullptr;

	pWICImagingFactory = nullptr;//---���ǉ�---
	pWICBitmapDecoder = nullptr;//---���ǉ�---
	pWICBitmapFrameDecode = nullptr;//---���ǉ�---
	pWICFormatConverter = nullptr;//---���ǉ�---
	pD2DBitmap = nullptr;//---���ǉ�---
}

DirectX11::~DirectX11()
{
	if (pD2DBitmap) pD2DBitmap->Release();//---���ǉ�---
	if (pWICFormatConverter) pWICFormatConverter->Release();//---���ǉ�---
	if (pWICBitmapFrameDecode) pWICBitmapFrameDecode->Release();//---���ǉ�---
	if (pWICBitmapDecoder) pWICBitmapDecoder->Release();//---���ǉ�---
	if (pWICImagingFactory) pWICImagingFactory->Release();//---���ǉ�---

	if (pDXGISurface) pDXGISurface->Release();
	if (pSolidBrush) pSolidBrush->Release();
	if (pRT) pRT->Release();
	if (pTextFormat) pTextFormat->Release();
	if (pDWriteFactory) pDWriteFactory->Release();
	if (pD2DFactory) pD2DFactory->Release();

	if (pImmediateContext) pImmediateContext->ClearState();

	if (pRenderTargetView) pRenderTargetView->Release();
	if (pSwapChain1) pSwapChain1->Release();
	if (pSwapChain) pSwapChain->Release();
	if (pImmediateContext1) pImmediateContext1->Release();
	if (pImmediateContext) pImmediateContext->Release();
	if (pd3dDevice1) pd3dDevice1->Release();
	if (pd3dDevice) pd3dDevice->Release();

	CoUninitialize();//---���ǉ�--- COM�̏I�������FRelease�O�ɌĂяo���Ɨ�O����������̂ň�ԍŌ�ɍs��
}

HRESULT DirectX11::InitDevice()
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(Window::GethWnd(), &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;//DirectX11���Direct2D���g�p���邽�߂ɕK�v
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_DRIVER_TYPE g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &pd3dDevice, &g_featureLevel, &pImmediateContext);

		if (hr == E_INVALIDARG)
		{
			hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &pd3dDevice, &g_featureLevel, &pImmediateContext);
		}

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	IDXGIFactory1* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}
	if (FAILED(hr))
		return hr;

	IDXGIFactory2* dxgiFactory2 = nullptr;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
	if (dxgiFactory2)
	{
		hr = pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&pd3dDevice1));
		if (SUCCEEDED(hr))
		{
			(void)pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&pImmediateContext1));
		}

		DXGI_SWAP_CHAIN_DESC1 sd = {};
		sd.Width = width;
		sd.Height = height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		hr = dxgiFactory2->CreateSwapChainForHwnd(pd3dDevice, Window::GethWnd(), &sd, nullptr, nullptr, &pSwapChain1);
		if (SUCCEEDED(hr))
		{
			hr = pSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&pSwapChain));
		}

		dxgiFactory2->Release();
	}
	else
	{
		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = Window::GethWnd();
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(pd3dDevice, &sd, &pSwapChain);
	}

	dxgiFactory->MakeWindowAssociation(Window::GethWnd(), DXGI_MWA_NO_ALT_ENTER);

	dxgiFactory->Release();

	if (FAILED(hr))
		return hr;

	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return hr;

	hr = pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);

	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pImmediateContext->RSSetViewports(1, &vp);

	// Direct2D,DirectWrite�̏�����
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	if (FAILED(hr))
		return hr;

	hr = pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pDXGISurface));
	if (FAILED(hr))
		return hr;

	FLOAT dpiX = 0.0f;
	FLOAT dpiY = 0.0f;
	//pD2DFactory->GetDesktopDpi(&dpiX, &dpiY);

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);

	hr = pD2DFactory->CreateDxgiSurfaceRenderTarget(pDXGISurface, &props, &pRT);
	if (FAILED(hr))
		return hr;

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown * *>(&pDWriteFactory));
	if (FAILED(hr))
		return hr;

	//�֐�CreateTextFormat()
	//��1�����F�t�H���g���iL"���C���I", L"Arial", L"Meiryo UI"���j
	//��2�����F�t�H���g�R���N�V�����inullptr�j
	//��3�����F�t�H���g�̑����iDWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_WEIGHT_BOLD���j
	//��4�����F�t�H���g�X�^�C���iDWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STYLE_ITALIC�j
	//��5�����F�t�H���g�̕��iDWRITE_FONT_STRETCH_NORMAL,DWRITE_FONT_STRETCH_EXTRA_EXPANDED���j
	//��6�����F�t�H���g�T�C�Y�i20, 30���j
	//��7�����F���P�[�����iL""�j
	//��8�����F�e�L�X�g�t�H�[�}�b�g�i&g_pTextFormat�j
	hr = pDWriteFactory->CreateTextFormat(L"���C���I", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"", &pTextFormat);
	if (FAILED(hr))
		return hr;

	//�֐�SetTextAlignment()
	//��1�����F�e�L�X�g�̔z�u�iDWRITE_TEXT_ALIGNMENT_LEADING�F�O, DWRITE_TEXT_ALIGNMENT_TRAILING�F��, DWRITE_TEXT_ALIGNMENT_CENTER�F�����j
	hr = pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	if (FAILED(hr))
		return hr;

	//�֐�CreateSolidColorBrush()
	//��1�����F�t�H���g�F�iD2D1::ColorF(D2D1::ColorF::Black)�F��, D2D1::ColorF(D2D1::ColorF(0.0f, 0.2f, 0.9f, 1.0f))�FRGBA�w��j
	hr = pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pSolidBrush);
	if (FAILED(hr))
		return hr;

	//--------------------------���ǉ���--------------------------
	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&pWICImagingFactory);
	if (FAILED(hr))
		return hr;

	//�֐�CreateDecoderFromFilename()
	//��1�����F�t�@�C����
	hr = pWICImagingFactory->CreateDecoderFromFilename(L"image.png", nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pWICBitmapDecoder);
	if (FAILED(hr))
		return hr;

	hr = pWICBitmapDecoder->GetFrame(0, &pWICBitmapFrameDecode);
	if (FAILED(hr))
		return hr;

	hr = pWICImagingFactory->CreateFormatConverter(&pWICFormatConverter);
	if (FAILED(hr))
		return hr;

	hr = pWICFormatConverter->Initialize(pWICBitmapFrameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 1.0f, WICBitmapPaletteTypeMedianCut);
	if (FAILED(hr))
		return hr;

	hr = pRT->CreateBitmapFromWicBitmap(pWICFormatConverter, nullptr, &pD2DBitmap);
	if (FAILED(hr))
		return hr;
	//--------------------------���ǉ���--------------------------

	return S_OK;
}

void DirectX11::Render()
{
	pImmediateContext->ClearRenderTargetView(pRenderTargetView, DirectX::Colors::Aquamarine);

	//--------------------------���ύX��--------------------------
	//�摜�ƃe�L�X�g�̕`��
	WCHAR wcText1[256] = { 0 };

	swprintf(wcText1, 256, L"%lf", Window::GetFps());

	static float fPosX = 0;
	static float fPosY = 0;

	pRT->BeginDraw();
	//�֐�DrawBitmap()
	//��1�����F�����_�����O����r�b�g�}�b�v�ipD2DBitmap�j
	//��2�����F�r�b�g�}�b�v�̈ʒu�̍��W�iD2D1::RectF(��, ��, �E, ��)�j
	//��3�����F�s�����x�i0.0f�`1.0f�j
	//��4�����F�r�b�g�}�b�v���g��k���܂��͉�]�����ꍇ�Ɏg�p�����ԃ��[�h�iD2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR�F�h�b�g�G��[�M�U�M�U]
	//                                                                         D2D1_BITMAP_INTERPOLATION_MODE_LINEAR�F�ʐ^��[�Ȃ߂炩]�j
	//��5�����F�g���~���O�iD2D1::RectF(��, ��, �E, ��), nullptr�F�C���[�W�S�̂̏ꍇ�j
	pRT->DrawBitmap(pD2DBitmap, D2D1::RectF(fPosX + 0, fPosY + 0, fPosX + 128, fPosY + 128), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, nullptr);
	//pRT->DrawText(wcText1, ARRAYSIZE(wcText1) - 1, pTextFormat, D2D1::RectF(0, 0, 800, 20), pSolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	pRT->EndDraw();
	//--------------------------���ύX��--------------------------

	pSwapChain->Present(0, 0);
}