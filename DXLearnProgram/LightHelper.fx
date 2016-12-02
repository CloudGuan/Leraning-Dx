struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float pad;
};


struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Att;
	float pad;
};

struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Direction;
	float Spot;

	float3 Att;
	float pad;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = SpecPower
	float4 Reflect;
};

void ComputeDirectionaLight(Material mat, DirectionalLight L, float3 normal, float3 toEye, out float4 ambient, float4 diffuse, float4 specical)
{
	ambient	=float4(0.0f,0.0f,0.0f,0.0f);
	diffuse	=float4(0.0f,0.0f,0.0f,0.0f);
	specical=float4(0.0f,0.0f,0.0f,0.0f);

	/**计算反射线，材质受平行光的影响*/
	float3 lightVec = -L.Direction;
	ambient = mat.Ambient*L.Ambient;

	float Diffusefactor = dot(lightVec, normal);
	/**分支语句*/
	[flatten]
	if (Diffusefactor > 0.0f)
	{
		/**compute ref light*/
		float3 reflight = reflect(-lightVec,normal);
		float ks = pow(max(dot(reflight, toEye), 0.0f),mat.Specular.w);
		diffuse = Diffusefactor*L.Diffuse*mat.Diffuse;

		specical = ks*mat.Specular*L.Specular;
	}
}

void ComputePointLight(Material mat, PointLight L,float3 pos, float3 normal, float3 toEye, out float4 ambient, float4 diffuse, float4 specical)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specical = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 LightVec = L.Position - pos;
	float distance = length(LightVec);

	if (distance > L.Range)
		return;

	LightVec /= distance;

	ambient = L.Ambient*mat.Ambient;
	float Diffusefactor = dot(LightVec, normal);

	/**分支语句*/
	[flatten]
	if (Diffusefactor > 0.0f)
	{
		/**compute ref light*/
		float3 reflight = reflect(-LightVec, normal);
		float ks = pow(max(dot(reflight, toEye), 0.0f), mat.Specular.w);
		diffuse = Diffusefactor*L.Diffuse*mat.Diffuse;

		specical = ks*mat.Specular*L.Specular;
	}

	float att = 1.0f / dot(L.Att, float3(1.0f, distance, distance*distance));

	diffuse *= att;
	specical *= specical;
}