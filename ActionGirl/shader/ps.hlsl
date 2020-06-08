//--------------------------------------------------------------------------------------
// ps.fx
//--------------------------------------------------------------------------------------
#include	"psvscommon.fx"

//--------------------------------------------------------------------------------------
//  ピクセルシェーダー
//--------------------------------------------------------------------------------------
float4 main(VS_OUTPUT input) : SV_Target
{
	//float4 col;
	//float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex);

 //   float4 L = LightDirection;
 //   L = normalize(L);
 //   L.w = 0.0f;

 //   float3 N;
 //   N = normalize(input.Normal.xyz);

 //   float i;
 //   i = saturate(dot(L.xyz, N));
    
 //   float4 diffuse;

 //   diffuse = (i * diffuseMaterial.xyz, 1.0);

 //   col = texcol * diffuse;
 //   //col = texcol * float4(0.0f, 0.0f, 0.0f, 0.3f);
 //   col.a = 1.0f;

	//return col;
    float4 col;
    float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex);
   
    float4 N;
    N = input.Normal;
    N = normalize(N);

    float4 L = LightDirection;
    L = normalize(L);
    L.w = 0.0f;

    float i;
    i = saturate(dot(L, N));
    //i = max(0.0f, dot(L, N));

    float4 diffuse;
    diffuse = i;// * diffuseMaterial; //*/ //, 1.0f)*/;

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

    col = texcol; //* diffuse /*+ specular*/;
    //col = diffuse;
    col.a = 1.0f;

    return col;
}