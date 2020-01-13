struct PointLight
{
    
    float4 Color;
    float3 Pos;
	
};

struct SpotLight
{
	
    float4 Pos;
    float4 Color;
    float3 Dir;
    float Spot;
	
};

struct Material
{

    float4 materialAmb;
    float4 materialDiff;
    float4 materialSpec;


};

cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 Eye;
    PointLight PLight;
    float Time;
    SpotLight SLights[4];
    Material mat;
    
}

Texture2D txColor : register(t0);
SamplerState txSampler : register(s0);

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float2 Tex : TEXCOORD0;
	float3 PosWorld : TEXCOORD1;
	float3 RotatedL : TEXCOORD2;
	float3 Norm : NORMAL;
};



float4 PS(VS_OUTPUT input) : SV_Target
{
	

	float4 texColor = txColor.SampleLevel(txSampler, input.Tex, 100.0f);
	texColor *= (1.0f - smoothstep(0.0f, 0.5f, length(input.Tex - 0.5f)));
	float fadingRate = (1 - Time / mat.materialAmb.w); //time to live is stored in the amb mat alpha
    return texColor * fadingRate;
    

}
