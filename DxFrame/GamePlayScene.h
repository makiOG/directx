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


// ��̒��_�����i�[����\����
struct VERTEX {
	XMFLOAT3 V;
};

// GPU(�V�F�[�_��)�֑��鐔�l���܂Ƃ߂��\����
struct CONSTANT_BUFFER {
	XMMATRIX mWVP;
};

//�V�[���e���v���[�g
class GamePlayScene :public Scene {
public:
	GamePlayScene();

	virtual void start()override;
	virtual void Update(float deltaTime)override;
	virtual void Draw()const override;
	virtual void lateDraw()override;
	//isEnd��true�ɂȂ�����̏���
	virtual void end() override;

private:
	ID3D11InputLayout * pVertexLayout;
	ID3D11VertexShader *pVertexShader;
	ID3D11PixelShader *pPixelShader;
	ID3D11Buffer *pConstantBuffer;
	ID3D11Buffer *pVertexBuffer;

	int CWIDTH;     // �N���C�A���g�̈�̕�
	int CHEIGHT;    // �N���C�A���g�̈�̍���

	float x = 0;

	pDxs _pDxs;


};