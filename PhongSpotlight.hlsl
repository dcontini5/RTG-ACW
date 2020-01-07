
cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 LightPos;
    float4 Eye;
    float Time;
}

//Texture2D txWoodColor : register(t0);
//SamplerState txWoodSampler : register(s0);

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
    float3 viewDis = normalize(Eye.xyz - input.PosWorld);
    float3 lightDir = input.RotatedL - input.PosWorld;
    float lightDirLenght = length(lightDir);
    lightDir /= lightDirLenght;
    
    float3 refl = reflect(-lightDir, input.Norm);

    float f = 64.0f; //vaolr in [1, 200], specifies the degree of shininess
    
    float diff = dot(lightDir, input.Norm) ;
    float spec = pow(max(dot(viewDis, refl), 0.0f), f) ;
    spec = spec * materialSpec * lightCol;
    diff = diff * materialDiff * lightCol;
    
    float3 Direction = normalize(float3(-2.0f, 0.0f, 2.0f) - input.RotatedL);
    float spot = pow(max(dot(-lightDir, Direction), 0.0f), 20.0f);
    float att = spot / dot(float3(1.0f, 0.0f, 0.0f), float3(1.0f, lightDirLenght, lightDirLenght * lightDirLenght));
    float amb = materialAmb * lightCol;
    
    
	amb *= spot;
    diff *= att;
    spec *= att;
    
    
    //float4 lightColor = materialAmb + (diff + spec) * lightCol;
    float4 lightColor = amb + (diff + spec);
    
    //lightColor.a = 1.0f;
    
    return input.Color * lightColor;
    //return spec * lightCol;
    //return lightColor;
	
    //float4 woodColor = txWoodColor.Sample(txWoodSampler, input.Tex);
    //return woodColor * lightColor ;
    
}
