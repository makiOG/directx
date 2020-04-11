#include <d3d9.h> //DirectXを使えるようにする
#include"stdafx.h"
#include"draw.h"

//グローバル変数
LPDIRECT3D9 g_pD3D = NULL; //Direct3Dへのアクセス
LPDIRECT3DDEVICE9 g_pD3DDev = NULL; //ビデオカードへのアクセス

HRESULT InitD3D(HWND hwnd)
{
	//Direct3Dを使えるようにする
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		return E_FAIL;
	}

	//ディスプレイのモードを調べる
	D3DDISPLAYMODE d3ddm;
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	//Direct3Dデバイスを生成する
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp,sizeof(d3dpp)); //ほとんど0
	d3dpp.Windowed = TRUE; //ウィンドウモード
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; //画面切り替え方法(推奨されている値)
	d3dpp.BackBufferFormat = d3ddm.Format;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp, &g_pD3DDev)))
	{
		return E_FAIL;
	}

	if (FAILED(Initialize(g_pD3DDev)))
	{
		//アプリケーションの初期化
		return E_FAIL;
	}
	g_pD3DDev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	return S_OK;
}
//描画関数
VOID Render()
{
	//背景を黒で塗りつぶす
	//g_pD3DDev->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	g_pD3DDev->BeginScene();//描画の開始

	Update(g_pD3DDev);

	//今度、関数の追加はここで行われる
	g_pD3DDev->EndScene();//描画の終了

	//上で描画したものを実際にウィンドウに反映する
	g_pD3DDev->Present(NULL, NULL, NULL, NULL);
}

//後片付け
#define SAFE_RELEASE(p) if(p){(p)->Release();(p)=NULL;}

void Cleanup()
{
	//アプリケーションの終了
	Close(g_pD3DDev);
	SAFE_RELEASE(g_pD3DDev);
	SAFE_RELEASE(g_pD3D);
}