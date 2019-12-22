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

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
    float3 Norm : TEXCOORD0;
    float3 PosWorld : TEXCOORD1;
    float4 LightPos : TEXTCORD2;
};


VS_OUTPUT VS(float4 Pos : POSITION, float4 Color : COLOR, float4 N : Normal)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(Pos, World);

	

    const float pi = 3.1415f;
    const float angle = -1 * pi / 2 * Time;



    float3 scaling = float3(2, 2, 2);
    float4x4 rotationY = float4x4(
		cos(angle), 0, -sin(angle), 0,
		0, 1, 0, 0,
		sin(angle), 0, cos(angle), 0,
		0, 0, 0, 1
		);

    float3 translation = float3(3, 0, 3);

	//output.Pos = mul(output.Pos, rotationY);
	//output.Pos.xyz *= scaling;
	//output.Pos.xyz += translation;
    output.LightPos.xyz = LightPos.xyz + translation;
    output.LightPos = mul(output.LightPos, rotationY);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Norm = N.xyz;
    output.PosWorld = Pos.xyz;
    output.Color = Color;


	
    return output;

}