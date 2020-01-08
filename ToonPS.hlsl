
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
    float4 Norm : NORMAL;
    float4 PosWorld : TEXCOORD0;
    float2 Tex : TEXCOORD1;
    float3 RotatedL : TEXCOORD2;
    float3 viewDirInTang : TEXCOORD3;
    float3 lightDirInTang : TEXCOORD4;

};

void CalculatePointLight(float3 lightpos, Material mat, float3 viewDis, float3 posWorld, float3 norm, out float4 ambient, out float4 diffuse, out float4 specular, out float4 rim);

void CalculateSpotLights(float4 lightcol, float3 lightpos, float3 direction, Material mat, float3 viewDis, float3 posWorld, float3 norm, out float4 ambient, out float4 diffuse, out float4 specular, out float4 rim);

float4 PS(VS_OUTPUT input) : SV_Target
{

    float4 ambient = 0.f;
    float4 diffuse = 0.f;
    float4 specular = 0.f;
    float4 rim = 0.f;
    
    input.Norm = normalize(input.Norm);
    float3 viewDis = normalize(input.PosWorld.xyz - Eye.xyz);
    float4 A, D, S, R;
	
		
    CalculatePointLight(input.RotatedL, mat, viewDis, input.PosWorld.xyz, input.Norm.xyz, A, D, S, R);
    ambient += A;
    diffuse += D;
    specular += S;
    rim += R;
	
    for (int i = 0; i < 4; i++)
    {
        
        CalculateSpotLights(SLights[i].Color, SLights[i].Pos.xyz, SLights[i].Dir, mat, viewDis, input.PosWorld.xyz, input.Norm.xyz, A, D, S, R);
        ambient += A;
        diffuse += D;
        specular += S;
        rim += R;
   
    }
	
    float4 lightColor = ambient + diffuse + specular + rim;
    lightColor *= input.Color;
    lightColor.a = mat.materialDiff.a;
    
	//return input.Color * lightColor * texColor;
    //return input.Color * lightColor;
    //return spec * lightCol;
	
    //float4 texColor = txWoodColor.Sample(txWoodSampler, input.Tex);
	//return texColor * lightColor;
    return lightColor;
    
}


void CalculateSpotLights(float4 lightcol, float3 lightpos, float3 direction, Material mat, float3 viewDis, float3 posWorld, float3 norm, out float4 ambient, out float4 diffuse, out float4 specular, out float4 rim)
{
	
    ambient.xyzw = 0.f;
    diffuse.xyzw = 0.f;
    specular.xyzw = 0.f;
    rim = 0.f;
	
    float3 lightDir = normalize(lightpos - posWorld);
    float lightDirLenght = length(lightDir);
    lightDir /= lightDirLenght;
    float3 refl = reflect(-lightDir, norm);
    float diff = smoothstep(0, 0.01, dot(lightDir, norm));
    float spec = smoothstep(0.0005, 0.01, pow(max(0.f, dot(refl, -viewDis)), mat.materialSpec.w));
    float4 rimDot = (1 - dot(-viewDis, norm)) * pow(diff, 0.1f);
    float4 rimIntensity = smoothstep(0.716f - 0.01, 0.716f + 0.01, rimDot);
    
    float3 Direction = normalize(direction - lightpos);
    float spot = pow(max(dot(-lightDir, Direction), 0.0f), 20.0f);
    float att = spot / dot(float3(1.0f, 0.0f, 0.0f), float3(1.0f, lightDirLenght, lightDirLenght * lightDirLenght));
    
    rim = rimIntensity * lightcol * att;
    ambient = mat.materialAmb * lightcol * spot;
    diffuse = diff * mat.materialDiff * lightcol * att;
    specular = spec * mat.materialSpec * lightcol * att;
    
    
}

void CalculatePointLight(float3 lightpos, Material mat, float3 viewDis, float3 posWorld, float3 norm, out float4 ambient, out float4 diffuse, out float4 specular, out float4 rim)
{
    ambient.xyzw = 0.f;
    diffuse.xyzw = 0.f;
    specular.xyzw = 0.f;
    rim = 0.f;
    
    float3 lightDir = normalize(lightpos - posWorld);
    float3 refl = reflect(-lightDir, norm);
    float diff = smoothstep(0, 0.01, dot(lightDir, norm));
    float spec = smoothstep(0.0005, 0.01, pow(max(0.f, dot(refl, -viewDis)), mat.materialSpec.w));
    float4 rimDot = (1 - dot(-viewDis, norm)) * pow(diff, 0.1f);
    float4 rimIntensity = smoothstep(0.716f - 0.01, 0.716f + 0.01, rimDot);
    
    rim = rimIntensity * PLight.Color;
    ambient = mat.materialAmb * PLight.Color;
    diffuse = diff * mat.materialDiff * PLight.Color;
    specular = spec * mat.materialSpec * PLight.Color;
 
}
