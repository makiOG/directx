#pragma once

#include<d3d9.h>
#include"stdafx.h"

//define�Ō�Еt���������`
//�|�C���^�[��nullptr�łȂ���΃|�C���^�[�̐��Release()�����s���A�|�C���^�[�ɂ�nullptr���i�[����
#define RELEASE(p) if(p){(p)->Release();(p)=nullptr;}

//�錾����Ă���֐�
//������
HRESULT Initialize(LPDIRECT3DDEVICE9 pD3DDev);
VOID Update(LPDIRECT3DDEVICE9 pD3DDev);
VOID Close(LPDIRECT3DDEVICE9 pD3DDev);