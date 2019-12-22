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



float4 PS(VS_OUTPUT input) : SV_Target
{
	return input.Color;
}
