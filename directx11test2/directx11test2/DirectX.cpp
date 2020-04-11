#include "Main.h"
#include "DirectX.h"

//--------------------------------------------------------------------------------------
// DirectX11::DirectX11()�֐��F�R���X�g���N�^
//--------------------------------------------------------------------------------------
DirectX11::DirectX11()
{
	m_matWorld = DirectX::XMMatrixIdentity();
	m_matView = DirectX::XMMatrixIdentity();
	m_matProjection = DirectX::XMMatrixIdentity();
}

//--------------------------------------------------------------------------------------
// DirectX11::~DirectX11�֐��F�f�X�g���N�^
//--------------------------------------------------------------------------------------
DirectX11::~DirectX11()
{

}

//--------------------------------------------------------------------------------------
// DirectX11::CompileShaderFromFile()�F�V�F�[�_�̃R���p�C��
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

//--------------------------------------------------------------------------------------
// DirectX11::InitDevice()�FDirectX�֌W�̏�����
//--------------------------------------------------------------------------------------
HRESULT DirectX11::InitDevice()
{
	//------------------------------------------------------------
	// DirectX11��Direct2D 1.1�̏�����
	//------------------------------------------------------------
	HRESULT hr = S_OK;

	UINT uiDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;//DirectX11���Direct2D���g�p���邽�߂ɕK�v
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
			D3D11_SDK_VERSION, &D3DDevice, &featurelevel, &m_D3DDeviceContext);//&D3DDevice &m_D3DDeviceContext ������

		if (hr == E_INVALIDARG)
		{
			hr = D3D11CreateDevice(nullptr, drivertype, nullptr, uiDeviceFlags, &featurelevels[1], uiFeatureLevelsNum - 1,
				D3D11_SDK_VERSION, &D3DDevice, &featurelevel, &m_D3DDeviceContext);//&D3DDevice &m_D3DDeviceContext ������
		}

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	Microsoft::WRL::ComPtr<IDXGIDevice2> DXGIDevice2;
	hr = D3DDevice.As(&DXGIDevice2);//D3DDevice->QueryInterface()�ł͂Ȃ�D3DDevice.As()�A&DXGIDevice2 ������
	if (FAILED(hr))
		return hr;

	Microsoft::WRL::ComPtr<ID2D1Factory1> D2DFactory1;
	D2D1_FACTORY_OPTIONS factoryoptions = {};
#ifdef _DEBUG
	factoryoptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factoryoptions, D2DFactory1.GetAddressOf());//&D2DFactory1�ł͂Ȃ�D2DFactory1.GetAddressOf()
	if (FAILED(hr))
		return hr;

	Microsoft::WRL::ComPtr<ID2D1Device> D2D1Device;
	hr = D2DFactory1->CreateDevice(DXGIDevice2.Get(), &D2D1Device);//DXGIDevice2�ł͂Ȃ�DXGIDevice2.Get()�A&D2D1Device ������
	if (FAILED(hr))
		return hr;

	hr = D2D1Device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_D2DDeviceContext);//&m_D2DDeviceContext ������
	if (FAILED(hr))
		return hr;

	Microsoft::WRL::ComPtr<IDXGIAdapter> DXGIAdapter;
	hr = DXGIDevice2->GetAdapter(&DXGIAdapter);//&DXGIAdapter ������
	if (FAILED(hr))
		return hr;

	Microsoft::WRL::ComPtr<IDXGIFactory2> DXGIFactory2;
	hr = DXGIAdapter->GetParent(IID_PPV_ARGS(&DXGIFactory2));//&DXGIFactory2 ������
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

	hr = DXGIFactory2->CreateSwapChainForHwnd(D3DDevice.Get(), Window::GethWnd(), &desc, nullptr, nullptr, &m_DXGISwapChain1);//D3DDevice�ł͂Ȃ�D3DDevice.Get()�A&m_DXGISwapChain1 ������
	if (FAILED(hr))
		return hr;

	(void)DXGIDevice2->SetMaximumFrameLatency(1);

	DXGIFactory2->MakeWindowAssociation(Window::GethWnd(), DXGI_MWA_NO_ALT_ENTER);//Alt+Enter���t���X�N���[���𖳌�

	Microsoft::WRL::ComPtr<IDXGISurface2> DXGISurface2;
	hr = m_DXGISwapChain1->GetBuffer(0, IID_PPV_ARGS(&DXGISurface2));//&DXGISurface2 ������
	if (FAILED(hr))
		return hr;

	hr = m_D2DDeviceContext->CreateBitmapFromDxgiSurface(DXGISurface2.Get(),//DXGISurface2�ł͂Ȃ�DXGISurface2.Get()
		D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)), &m_D2DBitmap1);//&m_D2DBitmap1 ������
	if (FAILED(hr))
		return hr;

	m_D2DDeviceContext->SetTarget(m_D2DBitmap1.Get());//D2DBitmap1�ł͂Ȃ�D2DBitmap1.Get()

	Microsoft::WRL::ComPtr<ID3D11Texture2D> D3DTexture2D;
	hr = m_DXGISwapChain1->GetBuffer(0, IID_PPV_ARGS(&D3DTexture2D));//&D3DTexture2D ������
	if (FAILED(hr))
		return hr;

	hr = D3DDevice->CreateRenderTargetView(D3DTexture2D.Get(), nullptr, &m_D3DRenderTargetView);//D3DTexture2D�ł͂Ȃ�D3DTexture2D.Get()�A&m_D3DRenderTargetView ������
	if (FAILED(hr))
		return hr;

	m_D3DDeviceContext->OMSetRenderTargets(1, m_D3DRenderTargetView.GetAddressOf(), nullptr);//&m_D3DRenderTargetView�ł͂Ȃ�m_D3DRenderTargetView.GetAddressOf()

	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)Window::GetClientWidth();
	vp.Height = (FLOAT)Window::GetClientHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_D3DDeviceContext->RSSetViewports(1, &vp);

	//�o�[�e�b�N�X�V�F�[�_�̃R���p�C��
	Microsoft::WRL::ComPtr<ID3DBlob> D3DBlobVS;
	hr = CompileShaderFromFile(L"VertexShader.hlsl", "main", "vs_5_0", &D3DBlobVS);
	if (FAILED(hr))
		return hr;

	//�o�[�e�b�N�X�V�F�[�_�̍쐬
	hr = D3DDevice->CreateVertexShader(D3DBlobVS->GetBufferPointer(), D3DBlobVS->GetBufferSize(), nullptr, &m_D3DVertexShader);
	if (FAILED(hr))
		return hr;

	//�C���v�b�g���C�A�E�g�̒�`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//5�Ԗڂ̃p�����[�^�F�擪����̃o�C�g��4�o�C�g(FLAOT)�~3(RGB)=12
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//5�Ԗڂ̃p�����[�^�F�擪����̃o�C�g��4�o�C�g(FLAOT)�~4(RGBA)+12=16+12=28
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT uiElements = ARRAYSIZE(layout);

	//�C���v�b�g���C�A�E�g�̍쐬
	Microsoft::WRL::ComPtr<ID3D11InputLayout> D3DInputLayout;
	hr = D3DDevice->CreateInputLayout(layout, uiElements, D3DBlobVS->GetBufferPointer(),
		D3DBlobVS->GetBufferSize(), &D3DInputLayout);
	if (FAILED(hr))
		return hr;

	//�C���v�b�g���C�A�E�g�̐ݒ�
	m_D3DDeviceContext->IASetInputLayout(D3DInputLayout.Get());

	//�s�N�Z���V�F�[�_�̃R���p�C��
	Microsoft::WRL::ComPtr<ID3DBlob> D3DBlobPS;
	hr = CompileShaderFromFile(L"PixelShader.hlsl", "main", "ps_5_0", &D3DBlobPS);
	if (FAILED(hr))
		return hr;

	//�s�N�Z���V�F�[�_�̍쐬
	hr = D3DDevice->CreatePixelShader(D3DBlobPS->GetBufferPointer(), D3DBlobPS->GetBufferSize(), nullptr, &m_D3DPixelShader);
	if (FAILED(hr))
		return hr;

	//�o�[�e�b�N�X�o�b�t�@�̍쐬
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

	//�o�[�e�b�N�X�o�b�t�@�̐ݒ�
	UINT uiStride = sizeof(SimpleVertex);
	UINT uiOffset = 0;
	m_D3DDeviceContext->IASetVertexBuffers(0, 1, m_D3DVertexBuffer.GetAddressOf(), &uiStride, &uiOffset);

	//�C���f�b�N�X�o�b�t�@�̍쐬
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

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	m_D3DDeviceContext->IASetIndexBuffer(D3DIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	//�v���~�e�B�u�g�|���W�̐ݒ�
	m_D3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�R���X�^���g�o�b�t�@�̍쐬
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hr = D3DDevice->CreateBuffer(&bd, nullptr, &m_D3DConstantBuffer);
	if (FAILED(hr))
		return hr;

	//���X�^���C�U�̍쐬
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> D3DRasterizerState;
	D3D11_RASTERIZER_DESC ras = {};
	//D3D11_FILL_WIREFRAME�i���C���[�t���[���j D3D11_FILL_SOLID�i�\���b�h�j
	ras.FillMode = D3D11_FILL_SOLID;
	//D3D11_CULL_NONE�i�J�����O�Ȃ��F���\�`��j D3D11_CULL_FRONT�i�\�ʃJ�����O�F���ʕ`��j D3D11_CULL_BACK�i���ʃJ�����O�F�\�ʕ`��j
	ras.CullMode = D3D11_CULL_NONE;
	hr = D3DDevice->CreateRasterizerState(&ras, &D3DRasterizerState);
	if (FAILED(hr))
		return hr;

	//���X�^���C�U�̐ݒ�
	m_D3DDeviceContext->RSSetState(D3DRasterizerState.Get());

	//--------------------------���ǉ���--------------------------
	//�e�N�X�`���̓ǂݍ���
	Microsoft::WRL::ComPtr<IWICImagingFactory> WICImagingFactory;
	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)(&WICImagingFactory));
	if (FAILED(hr))
		return hr;

	Microsoft::WRL::ComPtr<IWICBitmapDecoder> WICBitmapDecoder;
	//�֐�CreateDecoderFromFilename()
	//��1�����F�t�@�C����
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

	//�e�N�X�`���̃T�C�Y���擾
	UINT uiImageWidth;
	UINT uiImageHeight;
	hr = WICFormatConverter->GetSize(&uiImageWidth, &uiImageHeight);
	if (FAILED(hr))
		return hr;

	//�e�N�X�`���̍쐬
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

	//�V�F�[�_���\�[�X�r���[�̍쐬
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> D3DShaderResourceView;
	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MipLevels = 1;
	hr = D3DDevice->CreateShaderResourceView(D3DTexture.Get(), &srv, &D3DShaderResourceView);
	if (FAILED(hr))
		return hr;

	//�T���v���X�e�[�g�̐ݒ�
	Microsoft::WRL::ComPtr<ID3D11SamplerState> D3DSamplerState;
	D3D11_SAMPLER_DESC sd = {};
	//��ԕ��@�@D3D11_FILTER_MIN_MAG_MIP_POINT�i�|�C���g�T���v�����O�j�AD3D11_FILTER_MIN_MAG_MIP_LINEAR�i���`��ԁj�AD3D11_FILTER_ANISOTROPIC�i�ٕ�����ԁj��
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//0�`1�͈̔͊O�e�N�X�`���@D3D11_TEXTURE_ADDRESS_WRAP�i���ׂ�j�AD3D11_TEXTURE_ADDRESS_MIRROR�i���]�j��
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = D3DDevice->CreateSamplerState(&sd, &D3DSamplerState);
	if (FAILED(hr))
		return hr;
	//--------------------------���ǉ���--------------------------

	//���[���h�}�g���b�N�X�̐ݒ�
	m_matWorld = DirectX::XMMatrixIdentity();

	//�r���[�}�g���b�N�X�̐ݒ�
	DirectX::XMVECTOR vecEye = DirectX::XMVectorSet(0.0f, 0.0f, -3.0f, 0.0f);//�J�����̈ʒu
	DirectX::XMVECTOR vecFocus = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);//�J�����̏œ_
	DirectX::XMVECTOR vecUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);//�J�����̏����
	m_matView = DirectX::XMMatrixLookAtLH(vecEye, vecFocus, vecUp);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	m_matProjection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, static_cast<FLOAT>(Window::GetClientWidth()) / static_cast<FLOAT>(Window::GetClientHeight()), 0.01f, 100.0f);

	//�V�F�[�_�̃Z�b�g
	m_D3DDeviceContext->VSSetShader(m_D3DVertexShader.Get(), nullptr, 0);
	m_D3DDeviceContext->VSSetConstantBuffers(0, 1, m_D3DConstantBuffer.GetAddressOf());
	m_D3DDeviceContext->PSSetShader(m_D3DPixelShader.Get(), nullptr, 0);
	m_D3DDeviceContext->PSSetSamplers(0, 1, D3DSamplerState.GetAddressOf());//��---�ǉ�---
	m_D3DDeviceContext->PSSetShaderResources(0, 1, D3DShaderResourceView.GetAddressOf());//��---�ǉ�---

	//------------------------------------------------------------
	// DirectWrite�̏�����
	//------------------------------------------------------------
	Microsoft::WRL::ComPtr<IDWriteFactory> DWriteFactory;
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &DWriteFactory);//&DWriteFactory ������
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
	hr = DWriteFactory->CreateTextFormat(L"���C���I", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"", &m_DWriteTextFormat);//&m_DWriteTextFormat ������
	if (FAILED(hr))
		return hr;

	//�֐�SetTextAlignment()
	//��1�����F�e�L�X�g�̔z�u�iDWRITE_TEXT_ALIGNMENT_LEADING�F�O, DWRITE_TEXT_ALIGNMENT_TRAILING�F��, DWRITE_TEXT_ALIGNMENT_CENTER�F�����j
	hr = m_DWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	if (FAILED(hr))
		return hr;

	//�֐�CreateSolidColorBrush()
	//��1�����F�t�H���g�F�iD2D1::ColorF(D2D1::ColorF::Black)�F��, D2D1::ColorF(D2D1::ColorF(0.0f, 0.2f, 0.9f, 1.0f))�FRGBA�w��j
	hr = m_D2DDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_D2DSolidBrush);//&m_D2DSolidBrush ������
	if (FAILED(hr))
		return hr;

	return S_OK;
}

//--------------------------------------------------------------------------------------
// DirectX11::Render()�FDirectX�֌W�̕`��
//--------------------------------------------------------------------------------------
void DirectX11::Render()
{
	m_D3DDeviceContext->ClearRenderTargetView(m_D3DRenderTargetView.Get(), DirectX::Colors::Aquamarine);//m_D3DRenderTargetView�ł͂Ȃ�m_D3DRenderTargetView.Get()

	//--------------------------���ύX��--------------------------
	//------------------------------------------------------------
	//�v�Z
	//------------------------------------------------------------
	static FLOAT fAngle = 0;
	fAngle += static_cast<FLOAT>(Window::GetFrameTime() / 1000.0);
	m_matWorld = DirectX::XMMatrixRotationY(fAngle);//Y����]
	//--------------------------���ύX��--------------------------

	//------------------------------------------------------------
	// ��������
	//------------------------------------------------------------
	//FPS�\���p
	WCHAR wcText1[256] = { 0 };
	swprintf(wcText1, 256, L"FPS=%lf", Window::GetFps());

	//------------------------------------------------------------
	// 3D�`��
	//------------------------------------------------------------
	//���_�̏����ւ�
	D3D11_MAPPED_SUBRESOURCE msr;
	m_D3DDeviceContext->Map(m_D3DVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	//--------------------------���ύX��--------------------------
	SimpleVertex vertices[] =
	{
		{ DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
	};
	//--------------------------���ύX��--------------------------
	memcpy(msr.pData, vertices, sizeof(vertices));
	m_D3DDeviceContext->Unmap(m_D3DVertexBuffer.Get(), 0);
	//�J�����̍X�V
	ConstantBuffer cb;
	cb.world = DirectX::XMMatrixTranspose(m_matWorld);
	cb.view = DirectX::XMMatrixTranspose(m_matView);
	cb.projection = DirectX::XMMatrixTranspose(m_matProjection);
	m_D3DDeviceContext->Map(m_D3DConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, (void*)(&cb), sizeof(cb));
	m_D3DDeviceContext->Unmap(m_D3DConstantBuffer.Get(), 0);
	//�l�p�`�̕`��
	m_D3DDeviceContext->DrawIndexed(6, 0, 0);

	//------------------------------------------------------------
	// 2D�`��
	//------------------------------------------------------------
	m_D2DDeviceContext->BeginDraw();
	m_D2DDeviceContext->DrawText(wcText1, ARRAYSIZE(wcText1) - 1, m_DWriteTextFormat.Get(), D2D1::RectF(0, 0, 800, 20), m_D2DSolidBrush.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);//m_DWriteTextFormat�ł͂Ȃ�m_DWriteTextFormat.Get()
	m_D2DDeviceContext->EndDraw();

	m_DXGISwapChain1->Present(0, 0);
}