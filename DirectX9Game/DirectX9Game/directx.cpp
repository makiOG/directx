#include <d3d9.h> //DirectX���g����悤�ɂ���
#include"stdafx.h"
#include"draw.h"

//�O���[�o���ϐ�
LPDIRECT3D9 g_pD3D = NULL; //Direct3D�ւ̃A�N�Z�X
LPDIRECT3DDEVICE9 g_pD3DDev = NULL; //�r�f�I�J�[�h�ւ̃A�N�Z�X

HRESULT InitD3D(HWND hwnd)
{
	//Direct3D���g����悤�ɂ���
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		return E_FAIL;
	}

	//�f�B�X�v���C�̃��[�h�𒲂ׂ�
	D3DDISPLAYMODE d3ddm;
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	//Direct3D�f�o�C�X�𐶐�����
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp,sizeof(d3dpp)); //�قƂ��0
	d3dpp.Windowed = TRUE; //�E�B���h�E���[�h
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; //��ʐ؂�ւ����@(��������Ă���l)
	d3dpp.BackBufferFormat = d3ddm.Format;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp, &g_pD3DDev)))
	{
		return E_FAIL;
	}

	if (FAILED(Initialize(g_pD3DDev)))
	{
		//�A�v���P�[�V�����̏�����
		return E_FAIL;
	}
	g_pD3DDev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	return S_OK;
}
//�`��֐�
VOID Render()
{
	//�w�i�����œh��Ԃ�
	//g_pD3DDev->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	g_pD3DDev->BeginScene();//�`��̊J�n

	Update(g_pD3DDev);

	//���x�A�֐��̒ǉ��͂����ōs����
	g_pD3DDev->EndScene();//�`��̏I��

	//��ŕ`�悵�����̂����ۂɃE�B���h�E�ɔ��f����
	g_pD3DDev->Present(NULL, NULL, NULL, NULL);
}

//��Еt��
#define SAFE_RELEASE(p) if(p){(p)->Release();(p)=NULL;}

void Cleanup()
{
	//�A�v���P�[�V�����̏I��
	Close(g_pD3DDev);
	SAFE_RELEASE(g_pD3DDev);
	SAFE_RELEASE(g_pD3D);
}