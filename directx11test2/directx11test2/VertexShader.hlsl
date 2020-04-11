cbuffer ConstantBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
}

//--------------------------���ǉ���--------------------------
struct VS_INPUT
{
	float4 pos : POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD;
};
//--------------------------���ǉ���--------------------------

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float2 tex : TEXCOORD0;//---���ǉ�---
};

//--------------------------------------------------------------------------------------
// �o�[�e�b�N�X�V�F�[�_
//--------------------------------------------------------------------------------------
VS_OUTPUT main(VS_INPUT input)//---���ύX---
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = mul(input.pos, world);//---���ύX---
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	output.color = input.color;
	output.tex = input.tex;//---���ύX---
	return output;
} 