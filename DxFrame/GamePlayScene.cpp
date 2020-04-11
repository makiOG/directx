#include "GamePlayScene.h"
#include"Window.h"

GamePlayScene::GamePlayScene()
{
	
}

void GamePlayScene::start()
{
	//_World.Init();
	//_World.setField(std::make_shared<FieldBase>(Model::GetInstance().GetHandle(MODEL_ID::MODEL_STAGE), Model::GetInstance().GetHandle(MODEL_ID::MODEL_SKYBOX)));
	isEnd_ = false;
	next_ = SceneType::SCENE_GAMEPLAY;

	_pDxs = Graphics::GetInstance().GetDxPointers();

	auto hwnd = _pDxs.window.lock()->GetHWND();


	// ----- パイプラインの準備 -----
	RECT csize;
	GetClientRect(hwnd, &csize);
	CWIDTH = csize.right;
	CHEIGHT = csize.bottom;

	

	// ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = CWIDTH;
	vp.Height = CHEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	// シェーダの設定
	ID3DBlob *pCompileVS = NULL;
	ID3DBlob *pCompilePS = NULL;
	D3DCompileFromFile(L"VertexShader.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	_pDxs.pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &pVertexShader);
	D3DCompileFromFile(L"PixelShader.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	_pDxs.pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &pPixelShader);

	// 頂点レイアウトの作成
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	_pDxs.pDevice->CreateInputLayout(layout, 1, pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &pVertexLayout);
	pCompileVS->Release();
	pCompilePS->Release();

	// 定数バッファの作成(パラメータ受け渡し用)
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	_pDxs.pDevice->CreateBuffer(&cb, NULL, &pConstantBuffer);

	// 頂点データとバッファの作成
	VERTEX vertices[] = {
		XMFLOAT3(0.1f, 0.0f, 0.0f),
	};
	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(VERTEX) * 1;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	_pDxs.pDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer);

	// ----- パイプラインの構築 -----
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	_pDxs.pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);      // 頂点データをセット
	_pDxs.pDeviceContext->IASetInputLayout(pVertexLayout);                             // 頂点レイアウトをセット
	_pDxs.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);      // データの入力種類を指定
	_pDxs.pDeviceContext->OMSetRenderTargets(1, (ID3D11RenderTargetView**)&_pDxs.pBackBuffer_RTV, NULL);                   // レンダーターゲットビューのセット
	_pDxs.pDeviceContext->RSSetViewports(1, &vp);                                          // ビューポートのセット
	_pDxs.pDeviceContext->VSSetShader(pVertexShader, NULL, 0);                         // 頂点シェーダをセット
	_pDxs.pDeviceContext->PSSetShader(pPixelShader, NULL, 0);                              // ピクセルシェーダをセット
	_pDxs.pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);                    // 定数バッファをセット
}

void GamePlayScene::Update(float deltaTime)
{
	//_World.Update(deltaTime);

	// パラメータの計算
	XMVECTOR eye_pos = XMVectorSet(0.0f, 0.0f, -2.0f, 1.0f);        // 視点位置
	XMVECTOR eye_lookat = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);      // 視点方向
	XMVECTOR eye_up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);          // 視点上方向
	XMMATRIX View = XMMatrixLookAtLH(eye_pos, eye_lookat, eye_up);  // 左手座標系のビュー行列
	XMMATRIX Proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)CWIDTH / (FLOAT)CHEIGHT, 0.1f, 100.0f);  // 左手座標系のパースペクティブ射影行列
	XMMATRIX World = XMMatrixRotationZ(x);                          // z軸を回転軸とした回転行列
	x += 0.001;                                                     // 回転パラメータの更新

																	// パラメータの受け渡し
	D3D11_MAPPED_SUBRESOURCE pdata;
	CONSTANT_BUFFER cb;
	cb.mWVP = XMMatrixTranspose(World * View * Proj);                               // リソースへ送る値をセット
	_pDxs.pDeviceContext->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata); // GPUからのリソースアクセスを一時止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));                // リソースへ値を送る
	_pDxs.pDeviceContext->Unmap(pConstantBuffer, 0);                                       // GPUからのリソースアクセスを再開

																					 
}

void GamePlayScene::Draw() const
{
	//_World.Draw();

	// 描画実行
	_pDxs.pDeviceContext->Draw(1, 0);
}

void GamePlayScene::lateDraw()
{
	//_World.lateDraw();
}

void GamePlayScene::end()
{
	//Sound::GetInstance().StopBGM();

	pVertexShader->Release();
	pVertexLayout->Release();
	pPixelShader->Release();
	pConstantBuffer->Release();
	pVertexBuffer->Release();
}
