cbuffer ConstantBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
}

//--------------------------★追加↓--------------------------
struct VS_INPUT
{
	float4 pos : POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD;
};
//--------------------------★追加↑--------------------------

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float2 tex : TEXCOORD0;//---★追加---
};

//--------------------------------------------------------------------------------------
// バーテックスシェーダ
//--------------------------------------------------------------------------------------
VS_OUTPUT main(VS_INPUT input)//---★変更---
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = mul(input.pos, world);//---★変更---
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	output.color = input.color;
	output.tex = input.tex;//---★変更---
	return output;
} 