
cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 LightPos;
    float4 Eye; 
    float Time;
}

//Texture2D txWoodColor : register(t0);
//SamplerState txWoodSampler : register(s0);

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
    float2 Tex : TEXCOORD0;
    float3 PosWorld : TEXCOORD1;
    float3 RotatedL : TEXCOORD2;
    float3 Norm : TEXTCORD3;
};



float4 PS(VS_OUTPUT input) : SV_Target
{

    return input.Color;
    
}
