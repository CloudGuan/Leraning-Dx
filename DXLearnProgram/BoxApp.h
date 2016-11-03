#pragma once

#include "DXTestClass.h"
#include "Common/d3dx11effect.h"
#include <DirectXMath.h>
//define the vertices layout 

struct BoxVertice
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};

class BoxApp :public D3DAppBase
{
public:
	BoxApp(HINSTANCE hInstance,int nShowCmd);
	virtual~BoxApp();
public:
	virtual bool Init() override;
	virtual void DrawScene() override;
	virtual void UpdateScene() override;
private:
	void BuildVerticeLayout();
	void BuildFX();
	void BuildBuffers();
private:
	/**Our Vertices Buffer and Indices Buffer*/
	ID3D11Buffer* pBoxVB;
	ID3D11Buffer* pBoxIB;

	ID3DX11Effect* pFX;
	ID3DX11EffectTechnique* pTech;
	ID3D11InputLayout* pInputLayout;
	ID3DX11EffectMatrixVariable* mfxWorldViewProj;

	DirectX::XMFLOAT4X4 mWord;
	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;

	ID3D11VertexShader*     g_pVertexShader = nullptr;
	ID3D11PixelShader*      g_pPixelShader = nullptr;
	ID3DBlob* ppBlobOut = nullptr;

	float mTheta;
	float mPhi;
	float mRadius;
};