#pragma once

#include "DXTestClass.h"
#include "Common/d3dx11effect.h"
#include <DirectXMath.h>
//define the vertices layout 

/**这是一个图形绘制演示程序，只要替换build buffers这个函数内部的东西，就可以更换不同的图形进行绘制，注意这个程序并无光照*/

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
	virtual void OnResize() override;
	virtual void UpdateScene(float) override;
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