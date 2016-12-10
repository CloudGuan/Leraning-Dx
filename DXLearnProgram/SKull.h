#pragma once
#include "DXTestClass.h"
#include "Common/d3dx11effect.h"
#include "LightHelper.h"
#include "EffectHelper.h"
#include <DirectXMath.h>
#include "InputHelper.h"




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

	/**�����Ǳ���fx��ʱ��Ҫ������˵��һ��������Щ��������ȥ*/
	ID3D11InputLayout* pInputLayout;


	DirectX::XMFLOAT4X4 mWord;
	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;

	/**store the texture tans*/
	DirectX::XMFLOAT4X4 TexTrans;
	/**Store the reference of texture pointer*/
	ID3D11ShaderResourceView* pDiffuseMapSRV;

	float mTheta;
	float mPhi;
	float mRadius;

	UINT IndicsCount;
	TextEffect* SkullEffect;
	DXInputHelper*  InpterController;

	//ID3D11RasterizerState* mWireframeRS;

	/**additional code to implement light effect*/
	DirectionalLight DlightSource;
	PointLigt PLightSource;
	Material SkullMaterial;

	DirectX::XMFLOAT3 EyePoint;
	/***/
};