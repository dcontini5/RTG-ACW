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
    
  
    //float3 w = (Eye - Pos).xyz / length(Eye.xyz - Pos.xyz);
    //float3 j = float3(0.0f, 1.0f, 0.0f);
    //float3 u = j * w / length(j * w);
    //float3 v = w * u;
    
    
    //float4 w = (Eye - Pos) / length(Eye - Pos);
    //float4 j = float4(0.0f, 1.0f, 0.0f, 0.0f);
    //float4 u = mul(j, w) / length(mul(j, w));
    //float4 v = mul(w, u);

    
    //float4x4 W = float4x4(  u.x, u.y, u.z, 0.0f,
                            //v.x, v.y, v.z, 0.0f,
                            //w.x, w.y, w.z, 0.0f,
                            //Pos.x, Pos.y, Pos.z, 1.0f );
    
    //output.Pos = mul(Pos, W);
    

    float3 viewLeft = View._11_21_31;
    float3 viewUp = View._12_22_32;
    float4 inPos = Pos;
    
    
    inPos.xyz = inPos.x * viewLeft + inPos.y * viewUp;
    
    inPos += float4(0.0f, 0.0f, Pos.z, 0.0f);
    
    
    
    output.Pos = mul(inPos, World);
    
    
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
    output.Norm = mul(N, (float3x3) World);
    
    output.Norm = normalize(output.Norm);
    output.PosWorld = mul(Pos, World).xyz;
    
    output.Color = Color;
    output.Tex = Tex;

	
    return output;

}