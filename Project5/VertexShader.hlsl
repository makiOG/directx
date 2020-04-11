// 定数バッファ(CPU側からの値受け取り場)
cbuffer global {
	matrix gWVP;    // 変換行列
};

// 頂点シェーダ
float4 VS(float4 Pos : POSITION) : SV_POSITION{
	return mul(Pos, gWVP);
}