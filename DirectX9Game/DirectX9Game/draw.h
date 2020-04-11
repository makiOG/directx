#pragma once

#include<d3d9.h>
#include"stdafx.h"

//defineで後片付け処理を定義
//ポインターがnullptrでなければポインターの先のRelease()を実行し、ポインターにはnullptrを格納する
#define RELEASE(p) if(p){(p)->Release();(p)=nullptr;}

//宣言されている関数
//初期化
HRESULT Initialize(LPDIRECT3DDEVICE9 pD3DDev);
VOID Update(LPDIRECT3DDEVICE9 pD3DDev);
VOID Close(LPDIRECT3DDEVICE9 pD3DDev);