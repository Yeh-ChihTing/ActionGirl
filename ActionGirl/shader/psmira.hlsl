//--------------------------------------------------------------------------------------
// ps.fx
//--------------------------------------------------------------------------------------
#include	"psvscommon.fx"

//--------------------------------------------------------------------------------------
//  ピクセルシェーダー
//--------------------------------------------------------------------------------------
float4 main(VS_OUTPUT input) : SV_Target
{
	float4 col;
	float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex);
	
    float4 L = LightDirection;
    L = normalize(L);
    L.w = 0.0f;

    float4 N;
    N = normalize(input.Normal);

    float i;
    //i = saturate(dot(L, N));
    i = max(0.0f, dot(L, N));
    
    float4 diffuse;

    diffuse = (i *diffuseMaterial.xyz, 1.0);
 
    float4 V;
    V = EyePos - input.WPos;
    V = normalize(V);

    float4 H;
    H = L + V;
    H = normalize(H);

    float4 specular;

    specular = pow(max(0.0f, dot(N, H)), 6000);

    col = texcol * diffuse + specular;

    col.a = 1.0f;

    return col;
}