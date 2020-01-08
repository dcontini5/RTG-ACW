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

Texture2D txWoodColor : register(t0);
Texture2D txWooaadColor : register(t1);
SamplerState txWoodSampler : register(s0);

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
    float4 Color : COLOR0;
    float4 Norm : NORMAL;
    float4 PosWorld : TEXCOORD0;
    float2 Tex : TEXCOORD1;
    float3 RotatedL : TEXCOORD2;
    float3 viewDirInTang : TEXCOORD3;
    float3 lightDirInTang : TEXCOORD4;

};




VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    output.Pos = mul(input.Pos, World);

    const float pi = 3.1415f;
    const float angle = 1 * pi / 2 * Time;

    float4x4 rotationY = float4x4(
		cos(angle), 0, -sin(angle), 0,
		0, 1, 0, 0,
		sin(angle), 0, cos(angle), 0,
		0, 0, 0, 1
		);

    //output.Pos.xyz -= PLight.Pos;
    //float3 translation = float3(0.0, 0.0, 3.5);
    //output.Pos = mul(output.Pos, rotationY);
	//output.Pos.xyz += translation;
    
    
    output.RotatedL = PLight.Pos;
    //output.RotatedL = output.RotatedL + translation;
    output.RotatedL = mul(output.RotatedL, (float3x3) rotationY);
    
    
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Norm = mul(-input.N, (float3x4) World);
    
    output.Norm = normalize(output.Norm);
    output.PosWorld = mul(input.Pos, World);
    
    output.Color = float4(1.0f, 0.6f, 0.2f, 1.0f);
    output.Tex = input.Tex;

	
    return output;

}

