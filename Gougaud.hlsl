//#include "Tutorial04.fx"

cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 LightPos;
    float4 Eye;
    float Time;
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
    float3 Norm : NORMAL;

};


VS_OUTPUT VS(float4 Pos : POSITION, float4 Color : COLOR, float3 N : NORMAL, float2 Tex : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(Pos, World);

	

    const float pi = 3.1415f;
    const float angle = 1 * pi / 2 * Time;

    float4x4 rotationY = float4x4(
		cos(angle), 0, -sin(angle), 0,
		0, 1, 0, 0,
		sin(angle), 0, cos(angle), 0,
		0, 0, 0, 1
		);

 
    float3 translation = float3(0.0, 0.0, 3.5);
	//output.Pos = mul(output.Pos, rotationY);
	//output.Pos.xyz *= scaling;
	//output.Pos.xyz += translation;
    
    //output.RotatedL = LightPos.xyz;
    output.RotatedL = LightPos.xyz + translation;
    output.RotatedL = mul(output.RotatedL, (float3x3) rotationY);
    
    
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    
    output.Norm = normalize(mul(N, (float3x3) World));
    
    output.PosWorld = mul(Pos, World).xyz;
    
    float4 materialAmb = float4(0.1f, 0.2f, 0.2f, 1.0f);
    float4 materialDiff = float4(0.5f, 0.3f, 0.6f, 1.0f);
    float4 materialSpec = float4(0.4f, 0.2f, 0.5f, 1.0f);
    //float4 lightCol = float4(1.0f, 0.6f, 0.8f, 1.0f);
    float4 lightCol = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    float3 viewDis = normalize(Eye.xyz - output.PosWorld);
    float3 lightDir = normalize(output.RotatedL - output.PosWorld);
    
    //float3 lightDir = output.RotatedL - output.PosWorld;

    

    float f = 32.0f; //vaolr in [1, 200], specifies the degree of shininess
    
    
    float diff = saturate(dot(lightDir, output.Norm));
    
    float3 refl = reflect(-lightDir, output.Norm);
    float spec = pow(saturate(dot(refl, viewDis)), f);
    
    
    
    spec *= materialSpec;
    diff *= materialDiff;
	
    float4 lightColor = materialAmb + (diff + spec) * lightCol;
    //return lightColor;
    output.Color = lightColor;
    
    
    output.Tex = float2(0, 0);

	
    return output;

}
