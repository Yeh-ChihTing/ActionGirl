//--------------------------------------------------------------------------------------
// vs.fx
//--------------------------------------------------------------------------------------
#include	"psvscommon.fx"
//--------------------------------------------------------------------------------------
// 頂点シェーダー
//--------------------------------------------------------------------------------------
VS_OUTPUT main(float4 Pos		:	POSITION,
	float4 Normal : NORMAL,
	float2 Tex : TEXCOORD,
    int4 BoneIndex : BONEINDEX,
	float4 BoneWeight : BONEWEIGHT)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

    // 頂点ブレンドの処理
    float4x4 comb = (float4x4) 0;
    
    for (int i = 0; i < 4; i++)
    {
        comb += BoneMatrix[BoneIndex[i]] * BoneWeight[i];
    }
    
    Pos.xyz = Pos.xyz + Normal.xyz * 0.01f;
    
    output.Pos = mul(Pos, comb);
    output.Pos = mul(output.Pos, World);
    output.WPos = output.Pos;
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
      
   
    float4 N = Normal;
    N.w = 0.0f;

    N = mul(N, World);
    N = normalize(N);

    output.Normal = N;

    output.Tex = Tex;

	return output;
}