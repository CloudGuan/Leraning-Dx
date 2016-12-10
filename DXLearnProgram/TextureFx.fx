//CloudGuan HLSL Demo

#include "LightHelper.fx"


cbuffer cbPerFrame
{
	DirectionalLight gDLight;
	PointLight gPLight;
	float3 gEyePos;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;
	float4x4 gTexTransform;
	Material gMaterial;
};

Texture2D gDiffuseMap;
/**
*	add texture sampler state
*	this is the different with the last chapter code
*/
SamplerState BoxSS
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct VertexIn
{
	float3 PosL		: POSITION;
	float3 NormalL	: NORMAL;
    float2 Tex      : TEXCOORD;
};

struct VertexOut
{
	float4 PosH		: SV_POSITION;
	float3 PosW		: POSITION;
	float3 NormalW	: NORMAL;
    float2 Tex      : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// 转换到世界空间
	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
    // trans the noraml to fit vertex
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);

	// 转换到齐次剪裁空间
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

    // trans the coord of texture
    vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	return vout;
}

float4 PS(VertexOut pin,uniform bool gUseTexture) :SV_Target
{
	/**插值后的法线需要重新规范化*/
	pin.NormalW = normalize(pin.NormalW);

	float3 toEyeW = normalize(gEyePos - pin.PosW);

    float4 texColor = float4(0, 0, 0, 1);

    if (gUseTexture)
    {
        texColor = gDiffuseMap.Sample(BoxSS,pin.Tex);
    }

	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

    //we must initliza this params
	float4 A = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 D = float4(0.7f, 0.2f, 0.2f, 0.0f);
	float4 S = float4(0.0f, 0.0f, 0.0f, 0.0f);

	ComputeDirectionaLight(gMaterial, gDLight, pin.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	float4 litColor =texColor*(ambient + diffuse) + spec;
	litColor.a = gMaterial.Diffuse.a*texColor.a;
	return litColor;
}

technique11 TextureTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS(true)));
	}
} 