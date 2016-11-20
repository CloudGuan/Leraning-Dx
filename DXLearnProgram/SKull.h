#pragma once
#include "DXTestClass.h"
#include "Common/d3dx11effect.h"
#include <DirectXMath.h>

struct SkullVect
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};

class SkullApp :public D3DAppBase
{
public:
	SkullApp(HINSTANCE hInstance, int nShowCmd);
	virtual ~SkullApp();
public:
	virtual bool Init();
	virtual void DrawScene();
	virtual void OnResize();
	virtual void UpdateScene(float DelteTime);
private:
	void BuildBuffers();
	void BuildVerticeLayout();
	void BuildFX();
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

	UINT IndicsCount;
	ID3D11RasterizerState* mWireframeRS;
};