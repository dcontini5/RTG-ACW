//#include "Tutorial04.fx"

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

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
    float3 N : NORMAL;
    float2 Tex : TEXCOORD;
    float3 T : TANGENT;
    float3 B : BINORMAL;
    
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 viewDir : TEXCOORD0;

};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
       

    output.viewDir = input.Pos;
    
    float4 skyPos = float4(Eye.xyz, 0.f) + input.Pos;
   
    
    output.Pos = mul(skyPos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);

    return output;

}

