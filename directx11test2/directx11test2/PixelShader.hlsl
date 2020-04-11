Texture2D TextureData : register(t0);//---★追加---

SamplerState SamplerData : register(s0);//---★追加---

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float2 tex : TEXCOORD0;//---★追加---
};

//--------------------------------------------------------------------------------------
// ピクセルシェーダ
//--------------------------------------------------------------------------------------
float4 main(VS_OUTPUT input) : SV_TARGET
{
	return TextureData.Sample(SamplerData, input.tex);//---★変更---
}