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
	Material gMaterial;
};


struct VertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float3 NormalW : NORMAL;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// 转换到世界空间
	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);

	// 转换到齐次剪裁空间
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	return vout;
}

float4 PS(VertexOut pin) :SV_Target
{
	/**插值后的法线需要重新规范化*/
	pin.NormalW = normalize(pin.NormalW);

	float3 toEyeW = normalize(gEyePos - pin.PosW);

		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

		float4 A = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 D = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 S = float4(0.0f, 0.0f, 0.0f, 0.0f);

	ComputeDirectionaLight(gMaterial, gDLight, pin.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	ComputePointLight(gMaterial, gPLight, pin.PosW, pin.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;

	float4 litColor = ambient + diffuse + spec;
	litColor.a = gMaterial.Diffuse.a;
	return litColor;
}

technique11 LightTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}