//--------------------------------------------------------------------------------------
// vs.fx
//--------------------------------------------------------------------------------------
#include	"psvscommon.fx"
//--------------------------------------------------------------------------------------
// ���_�V�F�[�_�[
//--------------------------------------------------------------------------------------

VS_OUTPUT main(float4 Pos		:	POSITION,
	float4 Normal : NORMAL,
	float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, World);
	output.WPos = output.Pos;
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	float4 N = Normal;
	N.w = 0.0f;					// �@���̓x�N�g���Ȃ̂łv�̒l���O�ɂ���B

	N = mul(N, World);			// �@���x�N�g���Ƀ��[���h�ϊ��s����|���ĕ�����ς���
	N = normalize(N);			// ���K��

	output.Normal = N;

	//float4 L = LightDirection;
	//L = normalize(L);
	//L.w = 0.0f;

	//float d;
	//d = max(0.0f, dot(L, N));	// �����o�[�g�̗]�����ŉA�e��t����

	//float4 diffuse;
	//diffuse = d;

	output.Tex = Tex;
    //output.Color = float4(1.0f, 1.0f, 1.0f, 1.0f); //diffuse;
	return output;
}