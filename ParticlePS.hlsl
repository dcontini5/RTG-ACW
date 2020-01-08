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

	float4 materialAmb = float4(0.1f, 0.2f, 0.2f, 1.0f);
	float4 materialDiff = float4(0.5f, 0.3f, 0.6f, 1.0f);
	float4 materialSpec = float4(0.4f, 0.2f, 0.5f, 1.0f);
	//float4 lightCol = float4(1.0f, 0.6f, 0.8f, 1.0f);
	float4 lightCol = float4(1.0f, 1.0f, 1.0f, 1.0f);
	input.Norm = normalize(input.Norm);


	float3 viewDis = normalize(input.PosWorld - Eye.xyz);
	float3 lightDir = normalize(input.RotatedL - input.PosWorld);


	float diff = max(0.f, dot(lightDir, input.Norm));

	float4 texColor = txColor.SampleLevel(txSampler, input.Tex, 100.0f);
	//float4 texColor = txColor.Sample(txSampler, input.Tex);
	texColor *= (1.0f - smoothstep(0.0f, 0.5f, length(input.Tex - 0.5f)));
	float fadingRate = (1 - Time / 5.f);

	diff *= materialDiff;

	float4 lightColor = (materialAmb + diff) * lightCol;


	//return input.Color * lightColor * texColor;
	//return input.Color * lightColor;
	//return spec * lightCol;

	//float4 texColor = txWoodColor.Sample(txWoodSampler, input.Tex);
	//return texColor * lightColor;
	return texColor * fadingRate;

}
