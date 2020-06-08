//--------------------------------------------------------------------------------------
// ps.fx
//--------------------------------------------------------------------------------------
#include	"psvscommon.fx"

//--------------------------------------------------------------------------------------
//  ピクセルシェーダー
//--------------------------------------------------------------------------------------
Texture2D g_Textoon : register(t1);
SamplerState g_samplerToon : register(s1);

float4 main(VS_OUTPUT input) : SV_Target
{
    float4 col;
    float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex);
   
    float4 N;
    N = input.Normal;
    N = normalize(N);

    float4 L = LightDirection;
    L = normalize(L);
    L.w = 0.0f;

    float i;
    //i = saturate(dot(L, N));
    i = max(0.0f, dot(L, N));

    i = i * 0.5 + 0.5;
    i = i * i;

    float2 toon;
    toon.x = i;
    toon.y = i;
    float4 tooncol = g_Textoon.Sample(g_SamplerLinear, toon);

    float4 diffuse;
    diffuse = tooncol/* * diffuseMaterial.xyz*/; //, 1.0f)*/;

    float4 V;
    V = EyePos - input.WPos;
    V = normalize(V);

    float4 H;
    H = L + V;
    H = normalize(H);

    
    float s;
    s = pow(max(0.0f, dot(N, H)), 300);

    float4 specular;
    specular = s;

    //col = texcol * diffuse + specular;
    col = texcol * diffuse;
    //col = diffuse;
    col.a = 1.0f;

    return col;
}