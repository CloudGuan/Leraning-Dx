#pragma once 

#include "Common/d3dUtil.h"
#include "Common/d3dx11effect.h"
#include "LightHelper.h"

#pragma region effect

class Effect
{
public:
	Effect(ID3D11Device* device,const std::string& filename);
	virtual ~Effect();
private:
	Effect(const Effect&rhp);
	Effect& operator=(const Effect& rhp);
protected:
	ID3DX11Effect* pFx;
};
#pragma endregion

class BasicEffect :public Effect
{
public:
	BasicEffect(ID3D11Device*device,const std::string filename);
	virtual ~BasicEffect();

	void SetWordMatrix(CXMMATRIX& M){ fxWorld->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWordTransMatrix(CXMMATRIX& M){ fxWInTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWordViewMatrix(CXMMATRIX& M){ fxWViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }

	void SetDirectLight(const DirectionalLight& DL){ fxDlight->SetRawValue(&DL, 0, sizeof(DirectionalLight)); }
	void SetMaterial(const Material& Ma){ fxSkullMaterial->SetRawValue(&Ma, 0, sizeof(Material)); }
	void SetEyePos(const XMFLOAT3& p){ EyePos->SetRawValue(&p, 0, sizeof(XMFLOAT3)); }

	ID3DX11EffectTechnique*& GetTechPtr(){ return pTech; }
private:
	ID3DX11EffectTechnique* pTech;
	ID3DX11EffectVariable*fxDlight;
	ID3DX11EffectVariable*fxSkullMaterial;

	ID3DX11EffectMatrixVariable* fxWorld;
	ID3DX11EffectMatrixVariable* fxWInTranspose;
	ID3DX11EffectMatrixVariable* fxWViewProj;

	ID3DX11EffectVectorVariable*EyePos;
};

class TextEffect :Effect
{
public:
	TextEffect(ID3D11Device* Device, const std::string filename);
	virtual ~TextEffect();
	void SetWordMatrix(CXMMATRIX& M){ fxWorld->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWordTransMatrix(CXMMATRIX& M){ fxWInTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWordViewMatrix(CXMMATRIX& M){ fxWViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTranMatrix(CXMMATRIX&M){ fxTexTrans->SetMatrix(reinterpret_cast<const float*>(&M)); }

	void SetDirectLight(const DirectionalLight& DL){ fxDlight->SetRawValue(&DL, 0, sizeof(DirectionalLight)); }
	void SetMaterial(const Material& Ma){ fxSkullMaterial->SetRawValue(&Ma, 0, sizeof(Material)); }
	void SetEyePos(const XMFLOAT3& p){ EyePos->SetRawValue(&p, 0, sizeof(XMFLOAT3)); }
	void SetDiffuse(ID3D11ShaderResourceView*& DiffusePointer){ DiffuseMap->SetResource(DiffusePointer); }

	ID3DX11EffectTechnique*& GetTechPtr(){ return pTech; }
private:
	ID3DX11EffectTechnique* pTech;
	ID3DX11EffectVariable*fxDlight;
	ID3DX11EffectVariable*fxSkullMaterial;

	ID3DX11EffectMatrixVariable* fxWorld;
	ID3DX11EffectMatrixVariable* fxWInTranspose;
	ID3DX11EffectMatrixVariable* fxWViewProj;
	ID3DX11EffectMatrixVariable* fxTexTrans;

	ID3DX11EffectVectorVariable*EyePos;

	ID3DX11EffectShaderResourceVariable* DiffuseMap;
};



