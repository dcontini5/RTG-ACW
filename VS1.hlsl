//#include "Tutorial04.fx"

cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    float Time;
    float4 LightPos;
    float4 Eye;
	
}

Texture2D txWoodColor : register(t0);
SamplerState txWoodSampler : register(s0);

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


VS_OUTPUT VS(float4 Pos : POSITION, float4 Color : COLOR, float4 N : NORMAL, float2 Tex : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(Pos, World);

	

    const float pi = 3.1415f;
    const float angle = -1 * pi / 2 * Time;

    float4x4 rotationY = float4x4(
		cos(angle), 0, -sin(angle), 0,
		0,          1,           0, 0,
		sin(angle), 0, cos(angle),  0,
		0,          0,          0,  1
		);

 
    float3 translation = float3(0.0, 0.0, 3.5);
	//output.Pos = mul(output.Pos, rotationY);
	//output.Pos.xyz *= scaling;
	//output.Pos.xyz += translation;
    
    //output.RotatedL = LightPos.xyz;
    output.RotatedL = LightPos.xyz + translation;
    output.RotatedL = mul(output.RotatedL, rotationY);
    
    
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Norm = N.xyz;
    //output.PosWorld = Pos.xyz;
    //output.PosWorld = World._11_11_11_11;
    output.PosWorld = mul(Pos, World).xyz;
    output.Color = Color;
    output.Tex = Tex;

	
    return output;

}