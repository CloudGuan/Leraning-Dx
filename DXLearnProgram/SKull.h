#pragma once
#include "DXTestClass.h"
#include "Common/d3dx11effect.h"
#include "LightHelper.h"
#include "EffectHelper.h"
#include <DirectXMath.h>



struct SkullVect
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
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

	/**当我们编译fx得时候要和书上说的一样，把这些变量绑定上去*/
	ID3D11InputLayout* pInputLayout;

	//
	DirectX::XMFLOAT4X4 mWord;
	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;

	float mTheta;
	float mPhi;
	float mRadius;

	UINT IndicsCount;
	BasicEffect* SkullEffect;

	ID3D11RasterizerState* mWireframeRS;

	/**additional code to implement light effect*/
	DirectionalLight DlightSource;
	PointLigt PLightSource;
	Material SkullMaterial;

	DirectX::XMFLOAT3 EyePoint;
};