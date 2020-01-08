
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
    float3 viewDir : TEXCOORD0;

};

float4 PS(VS_OUTPUT input) : SV_Target
{

    
    
    float4 texColor = txColor.Sample(txSampler, input.viewDir.xy);
    

    return texColor;
    
}

