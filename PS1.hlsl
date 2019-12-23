
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



float4 PS(VS_OUTPUT input) : SV_Target
{

    float4 materialAmb = float4(0.1f, 0.2f, 0.2f, 1.0f);
    float4 materialDiff = float4(0.5f, 0.3f, 0.6f, 1.0f);
    float4 materialSpec = float4(0.8f, 0.8f, 0.8f, 1.0f);
    float4 lightCol = float4(1.0f, 0.6f, 0.8f, 1.0f);
    float3 viewDis = normalize(Eye.xyz - input.Pos.xyz);
    float3 lightDir = normalize(input.RotatedL.xyz - input.PosWorld);
    float3 refl = reflect(lightDir, input.Norm);
    float spec = dot(refl, viewDis);
    float f = 200.0f; //vaolr in [1, 200], specifies the degree of shininess
    spec = pow(max(0.0, spec), f);
    float3 normal = normalize(input.Norm);
    float diff = max(0.0, dot(lightDir, normal));
    float4 lightColor = (materialAmb + diff * materialDiff + spec * materialSpec) * lightCol;
	

	//float4 woodColor = txWoodColor.Sample(txWoodSampler, input.Tex);

    return lightColor;
	//return woodColor * lightColor ;

}
