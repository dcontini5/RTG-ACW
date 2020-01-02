
cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 LightPos;
    float4 Eye;
    float Time;
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
    //float3 lightDir = output.RotatedL - output.PosWorld;
    
    float3 refl = reflect(-lightDir, input.Norm);

    float f = 1.0f; //vaolr in [1, 200], specifies the degree of shininess
    
    float diff = saturate(dot(lightDir, input.Norm));
    float spec = pow(saturate(dot(refl, -viewDis)), f);
    
    float4 texColor = txColor.SampleLevel(txSampler, input.Tex, 1.0f);
    //float4 texColor = txColor.Sample(txSampler, input.Tex);
    //texColor *= (1.0f - smoothstep(0.0f, 0.43f, length(input.Tex - 0.5f)));
    
    spec *= materialSpec;
    diff *= materialDiff;
	
    float4 lightColor = materialAmb + (diff + spec) * lightCol;
    
    
    //return input.Color * lightColor * woodColor;
    //return spec * lightCol;
	
    //float4 texColor = txWoodColor.Sample(txWoodSampler, input.Tex);
    return texColor * lightColor ;
    //return texColor;
    
}
