#pragma once

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dCompiler.lib")
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment(lib,"windowscodecs.lib")//---���ǉ�---
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <d2d1_1.h>
#include <dwrite.h>
#include <wchar.h>
#include <wrl/client.h>
#include <random>
#include <wincodec.h>//---���ǉ�---

//--------------------------------------------------------------------------------------
// �\����
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT2 Tex;//---���ǉ�---
};

struct ConstantBuffer
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

//--------------------------------------------------------------------------------------
// DirectX11�N���X�FDirectX�֌W
//--------------------------------------------------------------------------------------
class DirectX11
{
public:
	DirectX11();
	~DirectX11();
	HRESULT CompileShaderFromFile(const WCHAR* wcFileName, LPCSTR lpEntryPoint, LPCSTR lpShaderModel, ID3DBlob** D3DBlob);
	HRESULT InitDevice();
	void Render();
private:
	//------------------------------------------------------------
	// DirectX11��Direct2D 1.1�̏�����
	//------------------------------------------------------------
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_D3DDeviceContext;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_D2DDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> m_DXGISwapChain1;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_D2DBitmap1;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_D3DRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_D3DVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_D3DPixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_D3DVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_D3DConstantBuffer;
	DirectX::XMMATRIX m_matWorld;
	DirectX::XMMATRIX m_matView;
	DirectX::XMMATRIX m_matProjection;

	//------------------------------------------------------------
	// DirectWrite�̏�����
	//------------------------------------------------------------
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_DWriteTextFormat;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_D2DSolidBrush;
};