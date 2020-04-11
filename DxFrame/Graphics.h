#pragma once
#include <memory>
#include <DirectXMath.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include"WindowsPtr.h"

using namespace DirectX;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


/*
// 一つの頂点情報を格納する構造体
struct VERTEX {
	XMFLOAT3 V;
};

// GPU(シェーダ側)へ送る数値をまとめた構造体
struct CONSTANT_BUFFER {
	XMMATRIX mWVP;
};
*/

struct pDxs
{
	std::shared_ptr<IDXGISwapChain> pSwapChain;
	std::shared_ptr<ID3D11Device> pDevice;
	std::shared_ptr<ID3D11DeviceContext> pDeviceContext;

	std::shared_ptr<ID3D11RenderTargetView> pBackBuffer_RTV;

	WindowWeakPtr window;
};

/**
*  Gameクラス
*/
class Graphics
{
	//  -------------------------------------------------------------------------------
	//  生成と破棄
	//  -------------------------------------------------------------------------------
public:
	/**
	*  コンストラクタ
	*/
	Graphics() = default;

	//シングルトン
	static Graphics& GetInstance() {
		static Graphics grap;
		return grap;
	}
	

	//  -------------------------------------------------------------------------------
	//  基本機能
	//  -------------------------------------------------------------------------------
public:
	/**
	*  ゲームの初期化処理を行う
	*/
	bool Start(const WindowWeakPtr& window);
	/**
	*  ゲームの解放処理を行う
	*/
	void End();
	/**
	*  ゲーム１フレームの処理を行う
	*/
	void Update();
	//
	void Draw();

	pDxs GetDxPointers()const;

private:
	/**
	*  COMオブジェクト解放用関数オブジェクト
	*/
	struct com_deleter
	{
		void operator()(IUnknown* p)
		{
			p->Release();
		}
	};

private:
	pDxs _pDxs;
							/*
							std::shared_ptr<IDXGISwapChain> pSwapChain;
							std::shared_ptr<ID3D11Device> pDevice;
							std::shared_ptr<ID3D11DeviceContext> pDeviceContext;

							std::shared_ptr<ID3D11RenderTargetView> pBackBuffer_RTV;
							*/
							
							/*
	IDXGISwapChain *pSwapChain;
	ID3D11Device *pDevice;
	ID3D11DeviceContext *pDeviceContext;

	ID3D11RenderTargetView *pBackBuffer_RTV;
	*/
};