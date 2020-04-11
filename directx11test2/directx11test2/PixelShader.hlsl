Texture2D TextureData : register(t0);//---���ǉ�---

SamplerState SamplerData : register(s0);//---���ǉ�---

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float2 tex : TEXCOORD0;//---���ǉ�---
};

//--------------------------------------------------------------------------------------
// �s�N�Z���V�F�[�_
//--------------------------------------------------------------------------------------
float4 main(VS_OUTPUT input) : SV_TARGET
{
	return TextureData.Sample(SamplerData, input.tex);//---���ύX---
}