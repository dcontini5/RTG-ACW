//#include "Tutorial04.fx"

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float Time;
}

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};


VS_OUTPUT VS(float4 Pos : POSITION, float4 Color : COLOR)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, World);


	const float pi = 3.1415f;
	const float angle = -1 * pi/8 * 1;

	float4x4 rotationY = float4x4(
		cos(angle), 0, -sin(angle), 0,
		0, 1, 0, 0,
		sin(angle), 0, cos(angle), 0,
		0, 0, 0, 1
		);

	float3 translation = float3(-0.4f, -0.5f, -0.3f);
	float3 scaling = float3(0.2f, 0.4f, 0.2f);

	output.Pos = mul(output.Pos, rotationY);
	output.Pos.xyz *= scaling;
	output.Pos.xyz += translation;

	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Color = Color;
	return output;

}