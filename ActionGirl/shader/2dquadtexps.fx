#include	"psvscommon.fx"

//--------------------------------------------------------------------------------------
//  ピクセルシェーダー
//--------------------------------------------------------------------------------------
float4 main( VS_OUTPUT input ) : SV_Target
{
	float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex);

    return texcol;
}