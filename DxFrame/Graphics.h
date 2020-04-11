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
// ��̒��_�����i�[����\����
struct VERTEX {
	XMFLOAT3 V;
};

// GPU(�V�F�[�_��)�֑��鐔�l���܂Ƃ߂��\����
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
*  Game�N���X
*/
class Graphics
{
	//  -------------------------------------------------------------------------------
	//  �����Ɣj��
	//  -------------------------------------------------------------------------------
public:
	/**
	*  �R���X�g���N�^
	*/
	Graphics() = default;

	//�V���O���g��
	static Graphics& GetInstance() {
		static Graphics grap;
		return grap;
	}
	

	//  -------------------------------------------------------------------------------
	//  ��{�@�\
	//  -------------------------------------------------------------------------------
public:
	/**
	*  �Q�[���̏������������s��
	*/
	bool Start(const WindowWeakPtr& window);
	/**
	*  �Q�[���̉���������s��
	*/
	void End();
	/**
	*  �Q�[���P�t���[���̏������s��
	*/
	void Update();
	//
	void Draw();

	pDxs GetDxPointers()const;

private:
	/**
	*  COM�I�u�W�F�N�g����p�֐��I�u�W�F�N�g
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