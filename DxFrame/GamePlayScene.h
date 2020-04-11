#pragma once
#include"Scene.h"

#include <Windows.h>
#include <DirectXMath.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include"Graphics.h"

using namespace DirectX;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


// 一つの頂点情報を格納する構造体
struct VERTEX {
	XMFLOAT3 V;
};

// GPU(シェーダ側)へ送る数値をまとめた構造体
struct CONSTANT_BUFFER {
	XMMATRIX mWVP;
};

//シーンテンプレート
class GamePlayScene :public Scene {
public:
	GamePlayScene();

	virtual void start()override;
	virtual void Update(float deltaTime)override;
	virtual void Draw()const override;
	virtual void lateDraw()override;
	//isEndがtrueになった後の処理
	virtual void end() override;

private:
	ID3D11InputLayout * pVertexLayout;
	ID3D11VertexShader *pVertexShader;
	ID3D11PixelShader *pPixelShader;
	ID3D11Buffer *pConstantBuffer;
	ID3D11Buffer *pVertexBuffer;

	int CWIDTH;     // クライアント領域の幅
	int CHEIGHT;    // クライアント領域の高さ

	float x = 0;

	pDxs _pDxs;


};