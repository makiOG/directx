// �萔�o�b�t�@(CPU������̒l�󂯎���)
cbuffer global {
	matrix gWVP;    // �ϊ��s��
};

// ���_�V�F�[�_
float4 VS(float4 Pos : POSITION) : SV_POSITION{
	return mul(Pos, gWVP);
}