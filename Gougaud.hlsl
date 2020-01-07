//#include "Tutorial04.fx"

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

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
    float3 N : NORMAL;
    float2 Tex : TEXCOORD;
    float3 T : TANGENT;
    float3 B : BINORMAL;
    
};



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

void CalculatePointLight(float3 lightpos, Material mat, float3 viewDis, float3 posWorld, float3 norm, out float4 ambient, out float4 diffuse, out float4 specular);

void CalculateSpotLights(float4 lightcol, float3 lightpos, float3 direction, Material mat, float3 viewDis, float3 posWorld, float3 norm, out float4 ambient, out float4 diffuse, out float4 specular);



VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(input.Pos, World);

    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

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
    output.RotatedL = PLight.Pos;
    output.RotatedL = mul(output.RotatedL, (float3x3) rotationY);
    
    
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    
    output.Norm.xyz = normalize(mul(input.N, (float3x3) World));
    
    output.PosWorld = mul(input.Pos, World);
    
 
    
    float3 viewDis = normalize(output.PosWorld.xyz - Eye.xyz);
    float4 A, D, S;
    
    //float3 lightDir = output.RotatedL - output.PosWorld;

    
    CalculatePointLight(output.RotatedL, mat, viewDis, output.PosWorld.xyz, output.Norm.xyz, A, D, S);
    ambient += A;
    diffuse += D;
    specular += S;
	
    for (int i = 0; i < 4; i++)
    {
        
        CalculateSpotLights(SLights[i].Color, SLights[i].Pos.xyz, SLights[i].Dir, mat, viewDis, output.PosWorld.xyz, output.Norm.xyz, A, D, S);
        ambient += A;
        diffuse += D;
        specular += S;
   
    }

    
    
  

    float4 lightColor = ambient + diffuse + specular;
    lightColor *= input.Color;
    lightColor.a = mat.materialDiff.a;
    
    
    output.Color = lightColor;
    
    output.Tex = input.Tex;

	
    return output;

}


void CalculateSpotLights(float4 lightcol, float3 lightpos, float3 direction, Material mat, float3 viewDis, float3 posWorld, float3 norm, out float4 ambient, out float4 diffuse, out float4 specular)
{
	
    ambient.xyzw = 0.f;
    diffuse.xyzw = 0.f;
    specular.xyzw = 0.f;
	
    float3 lightDir = normalize(lightpos - posWorld);
    float lightDirLenght = length(lightDir);
    lightDir /= lightDirLenght;
    float3 refl = reflect(-lightDir, norm);
    float diff = max(0.f, dot(lightDir, norm));
    float spec = pow(max(0.f, dot(refl, -viewDis)), mat.materialSpec.w);
    
    float3 Direction = normalize(direction - lightpos);
    float spot = pow(max(dot(-lightDir, Direction), 0.0f), 20.0f);
    float att = spot / dot(float3(1.0f, 0.0f, 0.0f), float3(1.0f, lightDirLenght, lightDirLenght * lightDirLenght));
    
    ambient = mat.materialAmb * lightcol * spot;
    diffuse = diff * mat.materialDiff * lightcol * att;
    specular = spec * mat.materialSpec * lightcol * att;
    
    
}

void CalculatePointLight(float3 lightpos, Material mat, float3 viewDis, float3 posWorld, float3 norm, out float4 ambient, out float4 diffuse, out float4 specular)
{
    ambient.xyzw = 0.f;
    diffuse.xyzw = 0.f;
    specular.xyzw = 0.f;
	
	
    float3 lightDir = normalize(lightpos - posWorld);
    float3 refl = reflect(-lightDir, norm);
    float diff = max(0.f, dot(lightDir, norm));
    float spec = pow(max(0.f, dot(refl, -viewDis)), mat.materialSpec.w);
	
    ambient = mat.materialAmb * PLight.Color;
    diffuse = diff * mat.materialDiff * PLight.Color;
    specular = spec * mat.materialSpec * PLight.Color;
    //ambient = mat.materialAmb ;
	//diffuse = diff * mat.materialDiff;
    //specular = spec * mat.materialSpec;
	
}