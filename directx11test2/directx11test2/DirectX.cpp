#include "Main.h"
#include "DirectX.h"

#include <tchar.h>
#include<locale.h>

#include"WICTextureLoader/WICTextureLoader.cpp"

//--------------------------------------------------------------------------------------
// DirectX11::DirectX11()関数：コンストラクタ
//--------------------------------------------------------------------------------------
DirectX11::DirectX11()
{
	m_matWorld = DirectX::XMMatrixIdentity();
	m_matView = DirectX::XMMatrixIdentity();
	m_matProjection = DirectX::XMMatrixIdentity();
}

//--------------------------------------------------------------------------------------
// DirectX11::~DirectX11関数：デストラクタ
//--------------------------------------------------------------------------------------
DirectX11::~DirectX11()
{

}

//--------------------------------------------------------------------------------------
// DirectX11::CompileShaderFromFile()：シェーダのコンパイル
//--------------------------------------------------------------------------------------
HRESULT DirectX11::CompileShaderFromFile(const WCHAR* wcFileName, LPCSTR lpEntryPoint, LPCSTR lpShaderModel, ID3DBlob** D3DBlob)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	Microsoft::WRL::ComPtr<ID3DBlob> D3DErrorBlob;
	hr = D3DCompileFromFile(wcFileName, nullptr, nullptr, lpEntryPoint, lpShaderModel,
		dwShaderFlags, 0, D3DBlob, &D3DErrorBlob);
	if (FAILED(hr))
	{
		if (D3DErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(D3DErrorBlob->GetBufferPointer()));
		}
		return hr;
	}

	return S_OK;
}
//const char* → 
std::wstring towstring(const char* c) {

	std::wstring tmps;

	if (c == nullptr)
		return tmps;

	size_t sz = strlen(c);

	tmps.reserve(sz);//メモリを確保し、newが走りすぎないようにする

	const size_t CNT_MAX = 50;
	char tmpc[CNT_MAX];
	wchar_t tmpw[CNT_MAX];


	const char* p = c;
	assert(p);
	while (*p != '\0') {
		int L = mblen(p, CNT_MAX);//pが指し示すマルチバイト文字のバイト数を取得
		if (L <= 0)
			break;

		strncpy_s(tmpc, p, L);//tmpcにその一文字をコピーする
		tmpc[L] = '\0';
		//multi byte string to wide char string
		mbstowcs_s(0,tmpw, tmpc, CNT_MAX);
		//mbstowcs(tmpw, tmpc, CNT_MAX);//tmpcの終端を0にしてあるので人文字だけ変換する
		tmps += tmpw;

		p += L;

	}

	return tmps;
}

//--------------------------------------------------------------------------------------
// DirectX11::InitDevice()：DirectX関係の初期化
//--------------------------------------------------------------------------------------
HRESULT DirectX11::InitDevice()
{
	//------------------------------------------------------------
	// DirectX11とDirect2D 1.1の初期化
	//------------------------------------------------------------
	HRESULT hr = S_OK;

	UINT uiDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;//DirectX11上でDirect2Dを使用するために必要
#ifdef _DEBUG
	uiDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE drivertypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT uiDriverTypesNum = ARRAYSIZE(drivertypes);

	D3D_FEATURE_LEVEL featurelevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT uiFeatureLevelsNum = ARRAYSIZE(featurelevels);

	Microsoft::WRL::ComPtr<ID3D11Device> D3DDevice;
	D3D_DRIVER_TYPE drivertype = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL featurelevel = D3D_FEATURE_LEVEL_11_0;
	for (UINT uiDriverTypeIndex = 0; uiDriverTypeIndex < uiDriverTypesNum; uiDriverTypeIndex++)
	{
		drivertype = drivertypes[uiDriverTypeIndex];
		hr = D3D11CreateDevice(nullptr, drivertype, nullptr, uiDeviceFlags, featurelevels, uiFeatureLevelsNum,
			D3D11_SDK_VERSION, &D3DDevice, &featurelevel, &m_D3DDeviceContext);//&D3DDevice &m_D3DDeviceContext 初期化

		if (hr == E_INVALIDARG)
		{
			hr = D3D11CreateDevice(nullptr, drivertype, nullptr, uiDeviceFlags, &featurelevels[1], uiFeatureLevelsNum - 1,
				D3D11_SDK_VERSION, &D3DDevice, &featurelevel, &m_D3DDeviceContext);//&D3DDevice &m_D3DDeviceContext 初期化
		}

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	Microsoft::WRL::ComPtr<IDXGIDevice2> DXGIDevice2;
	hr = D3DDevice.As(&DXGIDevice2);//D3DDevice->QueryInterface()ではなくD3DDevice.As()、&DXGIDevice2 初期化
	if (FAILED(hr))
		return hr;

	Microsoft::WRL::ComPtr<ID2D1Factory1> D2DFactory1;
	D2D1_FACTORY_OPTIONS factoryoptions = {};
#ifdef _DEBUG
	factoryoptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factoryoptions, D2DFactory1.GetAddressOf());//&D2DFactory1ではなくD2DFactory1.GetAddressOf()
	if (FAILED(hr))
		return hr;

	Microsoft::WRL::ComPtr<ID2D1Device> D2D1Device;
	hr = D2DFactory1->CreateDevice(DXGIDevice2.Get(), &D2D1Device);//DXGIDevice2ではなくDXGIDevice2.Get()、&D2D1Device 初期化
	if (FAILED(hr))
		return hr;

	hr = D2D1Device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_D2DDeviceContext);//&m_D2DDeviceContext 初期化
	if (FAILED(hr))
		return hr;

	Microsoft::WRL::ComPtr<IDXGIAdapter> DXGIAdapter;
	hr = DXGIDevice2->GetAdapter(&DXGIAdapter);//&DXGIAdapter 初期化
	if (FAILED(hr))
		return hr;

	Microsoft::WRL::ComPtr<IDXGIFactory2> DXGIFactory2;
	hr = DXGIAdapter->GetParent(IID_PPV_ARGS(&DXGIFactory2));//&DXGIFactory2 初期化
	if (FAILED(hr))
		return hr;

	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.Width = Window::GetClientWidth();
	desc.Height = Window::GetClientHeight();
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.Stereo = FALSE;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 2;
	desc.Scaling = DXGI_SCALING_STRETCH;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

	hr = DXGIFactory2->CreateSwapChainForHwnd(D3DDevice.Get(), Window::GethWnd(), &desc, nullptr, nullptr, &m_DXGISwapChain1);//D3DDeviceではなくD3DDevice.Get()、&m_DXGISwapChain1 初期化
	if (FAILED(hr))
		return hr;

	(void)DXGIDevice2->SetMaximumFrameLatency(1);

	DXGIFactory2->MakeWindowAssociation(Window::GethWnd(), DXGI_MWA_NO_ALT_ENTER);//Alt+Enter時フルスクリーンを無効

	Microsoft::WRL::ComPtr<IDXGISurface2> DXGISurface2;
	hr = m_DXGISwapChain1->GetBuffer(0, IID_PPV_ARGS(&DXGISurface2));//&DXGISurface2 初期化
	if (FAILED(hr))
		return hr;

	hr = m_D2DDeviceContext->CreateBitmapFromDxgiSurface(DXGISurface2.Get(),//DXGISurface2ではなくDXGISurface2.Get()
		D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)), &m_D2DBitmap1);//&m_D2DBitmap1 初期化
	if (FAILED(hr))
		return hr;

	m_D2DDeviceContext->SetTarget(m_D2DBitmap1.Get());//D2DBitmap1ではなくD2DBitmap1.Get()

	Microsoft::WRL::ComPtr<ID3D11Texture2D> D3DTexture2D;
	hr = m_DXGISwapChain1->GetBuffer(0, IID_PPV_ARGS(&D3DTexture2D));//&D3DTexture2D 初期化
	if (FAILED(hr))
		return hr;

	hr = D3DDevice->CreateRenderTargetView(D3DTexture2D.Get(), nullptr, &m_D3DRenderTargetView);//D3DTexture2DではなくD3DTexture2D.Get()、&m_D3DRenderTargetView 初期化
	if (FAILED(hr))
		return hr;

	m_D3DDeviceContext->OMSetRenderTargets(1, m_D3DRenderTargetView.GetAddressOf(), nullptr);//&m_D3DRenderTargetViewではなくm_D3DRenderTargetView.GetAddressOf()

	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)Window::GetClientWidth();
	vp.Height = (FLOAT)Window::GetClientHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_D3DDeviceContext->RSSetViewports(1, &vp);

	//バーテックスシェーダのコンパイル
	Microsoft::WRL::ComPtr<ID3DBlob> D3DBlobVS;
	hr = CompileShaderFromFile(L"VertexShader.hlsl", "main", "vs_5_0", &D3DBlobVS);
	if (FAILED(hr))
		return hr;

	//バーテックスシェーダの作成
	hr = D3DDevice->CreateVertexShader(D3DBlobVS->GetBufferPointer(), D3DBlobVS->GetBufferSize(), nullptr, &m_D3DVertexShader);
	if (FAILED(hr))
		return hr;

	//インプットレイアウトの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//5番目のパラメータ：先頭からのバイト数4バイト(FLAOT)×3(RGB)=12
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//5番目のパラメータ：先頭からのバイト数4バイト(FLAOT)×4(RGBA)+12=16+12=28
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT uiElements = ARRAYSIZE(layout);

	//インプットレイアウトの作成
	Microsoft::WRL::ComPtr<ID3D11InputLayout> D3DInputLayout;
	hr = D3DDevice->CreateInputLayout(layout, uiElements, D3DBlobVS->GetBufferPointer(),
		D3DBlobVS->GetBufferSize(), &D3DInputLayout);
	if (FAILED(hr))
		return hr;

	//インプットレイアウトの設定
	m_D3DDeviceContext->IASetInputLayout(D3DInputLayout.Get());

	//ピクセルシェーダのコンパイル
	Microsoft::WRL::ComPtr<ID3DBlob> D3DBlobPS;
	hr = CompileShaderFromFile(L"PixelShader.hlsl", "main", "ps_5_0", &D3DBlobPS);
	if (FAILED(hr))
		return hr;

	//ピクセルシェーダの作成
	hr = D3DDevice->CreatePixelShader(D3DBlobPS->GetBufferPointer(), D3DBlobPS->GetBufferSize(), nullptr, &m_D3DPixelShader);
	if (FAILED(hr))
		return hr;

	//バーテックスバッファの作成
	SimpleVertex vertices[] =
	{
		{ DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
	};
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(SimpleVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sub = {};
	sub.pSysMem = vertices;
	hr = D3DDevice->CreateBuffer(&bd, &sub, &m_D3DVertexBuffer);
	if (FAILED(hr))
		return hr;

	//バーテックスバッファの設定
	UINT uiStride = sizeof(SimpleVertex);
	UINT uiOffset = 0;
	m_D3DDeviceContext->IASetVertexBuffers(0, 1, m_D3DVertexBuffer.GetAddressOf(), &uiStride, &uiOffset);

	//インデックスバッファの作成
	Microsoft::WRL::ComPtr<ID3D11Buffer> D3DIndexBuffer;
	WORD indices[] =
	{
		0,1,2,
		2,1,3,
	};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 6;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	sub.pSysMem = indices;
	hr = D3DDevice->CreateBuffer(&bd, &sub, &D3DIndexBuffer);
	if (FAILED(hr))
		return hr;

	//インデックスバッファの設定
	m_D3DDeviceContext->IASetIndexBuffer(D3DIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	//プリミティブトポロジの設定
	m_D3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//コンスタントバッファの作成
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hr = D3DDevice->CreateBuffer(&bd, nullptr, &m_D3DConstantBuffer);
	if (FAILED(hr))
		return hr;

	//ラスタライザの作成
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> D3DRasterizerState;
	D3D11_RASTERIZER_DESC ras = {};
	//D3D11_FILL_WIREFRAME（ワイヤーフレーム） D3D11_FILL_SOLID（ソリッド）
	ras.FillMode = D3D11_FILL_SOLID;
	//D3D11_CULL_NONE（カリングなし：裏表描画） D3D11_CULL_FRONT（表面カリング：裏面描画） D3D11_CULL_BACK（裏面カリング：表面描画）
	ras.CullMode = D3D11_CULL_NONE;
	hr = D3DDevice->CreateRasterizerState(&ras, &D3DRasterizerState);
	if (FAILED(hr))
		return hr;

	//ラスタライザの設定
	m_D3DDeviceContext->RSSetState(D3DRasterizerState.Get());

	//--------------------------★追加↓--------------------------
	//テクスチャの読み込み
	Microsoft::WRL::ComPtr<IWICImagingFactory> WICImagingFactory;
	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)(&WICImagingFactory));
	if (FAILED(hr))
		return hr;

	Microsoft::WRL::ComPtr<IWICBitmapDecoder> WICBitmapDecoder;
	//関数CreateDecoderFromFilename()
	//第1引数：ファイル名
	hr = WICImagingFactory->CreateDecoderFromFilename(L"image.png", nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &WICBitmapDecoder);
	if (FAILED(hr))
		return hr;

	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> WICBitmapFrameDecode;
	hr = WICBitmapDecoder->GetFrame(0, &WICBitmapFrameDecode);
	if (FAILED(hr))
		return hr;

	Microsoft::WRL::ComPtr<IWICFormatConverter> WICFormatConverter;
	hr = WICImagingFactory->CreateFormatConverter(&WICFormatConverter);
	if (FAILED(hr))
		return hr;

	hr = WICFormatConverter->Initialize(WICBitmapFrameDecode.Get(), GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 1.0f, WICBitmapPaletteTypeMedianCut);
	if (FAILED(hr))
		return hr;

	//テクスチャのサイズを取得
	UINT uiImageWidth;
	UINT uiImageHeight;
	hr = WICFormatConverter->GetSize(&uiImageWidth, &uiImageHeight);
	if (FAILED(hr))
		return hr;

	//テクスチャの作成
	Microsoft::WRL::ComPtr<ID3D11Texture2D> D3DTexture;
	D3D11_TEXTURE2D_DESC td;
	td.Width = uiImageWidth;
	td.Height = uiImageHeight;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DYNAMIC;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	td.MiscFlags = 0;
	hr = D3DDevice->CreateTexture2D(&td, nullptr, &D3DTexture);
	if (FAILED(hr))
		return hr;

	D3D11_MAPPED_SUBRESOURCE msr;
	m_D3DDeviceContext->Map(D3DTexture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	WICFormatConverter->CopyPixels(nullptr, uiImageWidth * 4, uiImageWidth * uiImageHeight * 4, (BYTE*)msr.pData);
	m_D3DDeviceContext->Unmap(D3DTexture.Get(), 0);

	//シェーダリソースビューの作成
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> D3DShaderResourceView;
	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MipLevels = 1;
	hr = D3DDevice->CreateShaderResourceView(D3DTexture.Get(), &srv, &D3DShaderResourceView);
	if (FAILED(hr))
		return hr;

	//サンプラステートの設定
	Microsoft::WRL::ComPtr<ID3D11SamplerState> D3DSamplerState;
	D3D11_SAMPLER_DESC sd = {};
	//補間方法　D3D11_FILTER_MIN_MAG_MIP_POINT（ポイントサンプリング）、D3D11_FILTER_MIN_MAG_MIP_LINEAR（線形補間）、D3D11_FILTER_ANISOTROPIC（異方性補間）等
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//0〜1の範囲外テクスチャ　D3D11_TEXTURE_ADDRESS_WRAP（並べる）、D3D11_TEXTURE_ADDRESS_MIRROR（反転）等
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = D3DDevice->CreateSamplerState(&sd, &D3DSamplerState);
	if (FAILED(hr))
		return hr;
	//--------------------------★追加↑--------------------------

	//ワールドマトリックスの設定
	m_matWorld = DirectX::XMMatrixIdentity();

	//ビューマトリックスの設定
	DirectX::XMVECTOR vecEye = DirectX::XMVectorSet(0.0f, 0.0f, -3.0f, 0.0f);//カメラの位置
	DirectX::XMVECTOR vecFocus = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);//カメラの焦点
	DirectX::XMVECTOR vecUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);//カメラの上方向
	m_matView = DirectX::XMMatrixLookAtLH(vecEye, vecFocus, vecUp);

	//プロジェクションマトリックスの設定
	m_matProjection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, static_cast<FLOAT>(Window::GetClientWidth()) / static_cast<FLOAT>(Window::GetClientHeight()), 0.01f, 100.0f);

	//シェーダのセット
	m_D3DDeviceContext->VSSetShader(m_D3DVertexShader.Get(), nullptr, 0);
	m_D3DDeviceContext->VSSetConstantBuffers(0, 1, m_D3DConstantBuffer.GetAddressOf());
	m_D3DDeviceContext->PSSetShader(m_D3DPixelShader.Get(), nullptr, 0);
	m_D3DDeviceContext->PSSetSamplers(0, 1, D3DSamplerState.GetAddressOf());//★---追加---
	m_D3DDeviceContext->PSSetShaderResources(0, 1, D3DShaderResourceView.GetAddressOf());//★---追加---

	//------------------------------------------------------------
	// DirectWriteの初期化
	//------------------------------------------------------------
	Microsoft::WRL::ComPtr<IDWriteFactory> DWriteFactory;
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &DWriteFactory);//&DWriteFactory 初期化
	if (FAILED(hr))
		return hr;

	//関数CreateTextFormat()
	//第1引数：フォント名（L"メイリオ", L"Arial", L"Meiryo UI"等）
	//第2引数：フォントコレクション（nullptr）
	//第3引数：フォントの太さ（DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_WEIGHT_BOLD等）
	//第4引数：フォントスタイル（DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STYLE_ITALIC）
	//第5引数：フォントの幅（DWRITE_FONT_STRETCH_NORMAL,DWRITE_FONT_STRETCH_EXTRA_EXPANDED等）
	//第6引数：フォントサイズ（20, 30等）
	//第7引数：ロケール名（L""）
	//第8引数：テキストフォーマット（&g_pTextFormat）
	hr = DWriteFactory->CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"", &m_DWriteTextFormat);//&m_DWriteTextFormat 初期化
	if (FAILED(hr))
		return hr;

	//関数SetTextAlignment()
	//第1引数：テキストの配置（DWRITE_TEXT_ALIGNMENT_LEADING：前, DWRITE_TEXT_ALIGNMENT_TRAILING：後, DWRITE_TEXT_ALIGNMENT_CENTER：中央）
	hr = m_DWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	if (FAILED(hr))
		return hr;

	//関数CreateSolidColorBrush()
	//第1引数：フォント色（D2D1::ColorF(D2D1::ColorF::Black)：黒, D2D1::ColorF(D2D1::ColorF(0.0f, 0.2f, 0.9f, 1.0f))：RGBA指定）
	hr = m_D2DDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_D2DSolidBrush);//&m_D2DSolidBrush 初期化
	if (FAILED(hr))
		return hr;

	ID3D11Resource* res;
	ID3D11ShaderResourceView* vie;

	setlocale(LC_ALL, "");//必要
	const char *c = "image3.png";
	std::wstring s = towstring(c);


	//D3DDevice
	//テクスチャ読み込み
	hr = CreateWICTextureFromFile(D3DDevice.Get(), s.c_str(), &res, &vie);
	if (FAILED(hr))
		return hr;

	return S_OK;
}



//--------------------------------------------------------------------------------------
// DirectX11::Render()：DirectX関係の描画
//--------------------------------------------------------------------------------------
void DirectX11::Render()
{
	m_D3DDeviceContext->ClearRenderTargetView(m_D3DRenderTargetView.Get(), DirectX::Colors::Aquamarine);//m_D3DRenderTargetViewではなくm_D3DRenderTargetView.Get()

	//--------------------------★変更↓--------------------------
	//------------------------------------------------------------
	//計算
	//------------------------------------------------------------
	static FLOAT fAngle = 0;
	fAngle += static_cast<FLOAT>(Window::GetFrameTime() / 1000.0);
	m_matWorld = DirectX::XMMatrixRotationY(fAngle);//Y軸回転
	//--------------------------★変更↑--------------------------

	//------------------------------------------------------------
	// 文字操作
	//------------------------------------------------------------
	//FPS表示用
	WCHAR wcText1[256] = { 0 };
	swprintf(wcText1, 256, L"FPS=%lf", Window::GetFps());

	//------------------------------------------------------------
	// 3D描画
	//------------------------------------------------------------
	//頂点の書き替え
	D3D11_MAPPED_SUBRESOURCE msr;
	m_D3DDeviceContext->Map(m_D3DVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	//--------------------------★変更↓--------------------------
	SimpleVertex vertices[] =
	{
		{ DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
	};
	//--------------------------★変更↑--------------------------
	memcpy(msr.pData, vertices, sizeof(vertices));
	m_D3DDeviceContext->Unmap(m_D3DVertexBuffer.Get(), 0);
	//カメラの更新
	ConstantBuffer cb;
	cb.world = DirectX::XMMatrixTranspose(m_matWorld);
	cb.view = DirectX::XMMatrixTranspose(m_matView);
	cb.projection = DirectX::XMMatrixTranspose(m_matProjection);
	m_D3DDeviceContext->Map(m_D3DConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, (void*)(&cb), sizeof(cb));
	m_D3DDeviceContext->Unmap(m_D3DConstantBuffer.Get(), 0);
	//四角形の描画
	m_D3DDeviceContext->DrawIndexed(6, 0, 0);

	//------------------------------------------------------------
	// 2D描画
	//------------------------------------------------------------
	m_D2DDeviceContext->BeginDraw();
	m_D2DDeviceContext->DrawText(wcText1, ARRAYSIZE(wcText1) - 1, m_DWriteTextFormat.Get(), D2D1::RectF(0, 0, 800, 20), m_D2DSolidBrush.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);//m_DWriteTextFormatではなくm_DWriteTextFormat.Get()
	m_D2DDeviceContext->EndDraw();

	m_DXGISwapChain1->Present(0, 0);
}