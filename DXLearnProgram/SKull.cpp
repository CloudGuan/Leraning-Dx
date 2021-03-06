#include "SKull.h"
#include <iostream>
#include <D3Dcompiler.h>
#include <fstream>
#include <string>
#include <vector>
#include "DDSTextureLoader.h"
#include "GemotryHelper.h"
#include "CommonHeader.h"
#include "DebugTools.h"


SkullApp::SkullApp(HINSTANCE hInstance, int nShowCmd)
	:D3DAppBase(hInstance,nShowCmd)
	, pBoxVB(nullptr)
	, pBoxIB(nullptr)
	, pInputLayout(nullptr)
	, mTheta(1.5f*DirectX::XM_PI), mPhi(0.25f*DirectX::XM_PI)
	, mRadius(8.0f)
	, EyePoint(1.0f,6.0f,1.0f)
	, InpterController(nullptr)
	, pDiffuseMapSRV(nullptr)
{
	

	XMMATRIX skullScale = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	XMMATRIX skullOffset = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	
	DirectX::XMMATRIX I = DirectX::XMMatrixIdentity();
	//DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(0.0f, -2.0f, 0.0f);
	XMStoreFloat4x4(&mWord, XMMatrixMultiply(skullScale, skullOffset));
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);   
	/**Init the texture tans matrix*/
	XMStoreFloat4x4(&TexTrans, I);

	/**declear some paramitors */
	DlightSource.Ambient = DirectX::XMFLOAT4(0.5f,0.5f,0.5f,1.0f);
	DlightSource.Diffuse = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	DlightSource.Specular= DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	DlightSource.Direction = DirectX::XMFLOAT3(-0.57735f, 0.57735f, 0.57735f);

	PLightSource.Ambinet = DirectX::XMFLOAT4(0.3f,0.3f,0.3f,1.0f);
	PLightSource.Diffuse = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	PLightSource.Specular = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	PLightSource.Att = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);

	SkullMaterial.Ambient = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SkullMaterial.Diffuse = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	SkullMaterial.Specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);

}

SkullApp::~SkullApp()
{

}

bool SkullApp::Init()
{
	if(!D3DAppBase::Init())
	{
		DX_LOG("ERROR", "Init Dx Program error");
		return false;
	}
	InpterController = new DXInputHelper(hWnd, hWndInst, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	
	//BuildFX();
	
	SkullEffect = new TextEffect(pD3DDeviceInst, "../Debug/TextureFx.fxo");
	BuildVerticeLayout();
	BuildBuffers();

	/**
	*	原本这里的方法D3DX11CreateShaderResourceViewFromFile 已经被废弃了，现在这些东西被拆分了出来
	*	所以现在使用新的方法代替，https://github.com/Microsoft/DirectXTK/releases
	*	https://directxtk.codeplex.com/wikipage?title=DDSTextureLoader
	*/
	HRESULT hr = DirectX::CreateDDSTextureFromFile(
		pD3DDeviceInst,
		L"Textures/WoodCrate01.dds",
		nullptr,
		&pDiffuseMapSRV
		);
	if (FAILED(hr))
	{
		return false;
	}
	/*D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HRESULT hr=pD3DDeviceInst->CreateRasterizerState(&wireframeDesc, &mWireframeRS);
*/
	return true;
}

void SkullApp::DrawScene()
{
	assert(pD3DDevContInst);
	assert(pD3DDeviceInst);
	assert(SkullEffect);

	pD3DDevContInst->ClearRenderTargetView(
		pRenderingtargetView,
		reinterpret_cast<const float*>(&DirectX::Colors::LightSteelBlue));
	pD3DDevContInst->ClearDepthStencilView(
		pDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);


	pD3DDevContInst->IASetInputLayout(pInputLayout);
	pD3DDevContInst->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//线框模式
	//pD3DDevContInst->RSSetState(mWireframeRS);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	
	pD3DDevContInst->IASetVertexBuffers(0, 1, &pBoxVB, &stride, &offset);
	pD3DDevContInst->IASetIndexBuffer(pBoxIB, DXGI_FORMAT_R32_UINT, 0);
	

	DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4(&mWord);
	DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&mView);
	DirectX::XMMATRIX proj = DirectX::XMLoadFloat4x4(&mProj);
	DirectX::XMMATRIX WVP = world*view*proj;

	XMMATRIX A = world;
	A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR det = XMMatrixDeterminant(A);


	SkullEffect->SetWordViewMatrix(WVP);
	SkullEffect->SetWordMatrix(world);
	DirectX::XMMATRIX trans = XMMatrixTranspose(XMMatrixInverse(&det, A));
	SkullEffect->SetWordTransMatrix(trans);
	
	SkullEffect->SetTexTranMatrix(XMLoadFloat4x4(&TexTrans));
	SkullEffect->SetDirectLight(DlightSource);
	SkullEffect->SetEyePos(EyePoint);
	SkullEffect->SetMaterial(SkullMaterial);
	SkullEffect->SetDiffuse(pDiffuseMapSRV);

	D3DX11_TECHNIQUE_DESC techDesc;
	SkullEffect->GetTechPtr()->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		SkullEffect->GetTechPtr()->GetPassByIndex(p)->Apply(0, pD3DDevContInst);
		// 立方体有36个索引
		pD3DDevContInst->DrawIndexed(IndicsCount, 0, 0);
	}

	HRESULT hr = pSwapChain->Present(0, 0);
}

void SkullApp::OnResize()
{
	D3DAppBase::OnResize();

	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(0.25f*DirectX::XM_PI, AspectRatio(), 1.0f, 2000.0f);
	XMStoreFloat4x4(&mProj, P);
}

void SkullApp::UpdateScene(float DelteTime)
{
	InpterController->Tick();


	mTheta += InpterController->MouseX()*0.008f;
	
	float x = mRadius*sinf(mPhi)*cosf(mTheta);
	float z = mRadius*sinf(mPhi)*sinf(mTheta);
	float y = mRadius*cosf(mPhi);


	EyePoint = XMFLOAT3(x, y, z);
	// Build the view matrix.
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(x,y, z, 1.0f);
	DirectX::XMVECTOR target = DirectX::XMVectorZero();
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, V);
}

void SkullApp::BuildBuffers()
{
	/**
	*	chapter 8, add the texture params 
	*/
	MeshDate SkullMesh;
	if (GemotryHelper::GetInstance()->GetBox(SkullMesh) == 0)
		printf_s("read successful!!");
	
	IndicsCount=SkullMesh.Indices.size();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * SkullMesh.PosInfos.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sbd;
	sbd.pSysMem = SkullMesh.PosInfos.data();
	HRESULT hr = pD3DDeviceInst->CreateBuffer(
		&vbd,
		&sbd,
		&pBoxVB
		);
	if (FAILED(hr))
	{
		std::cout << "Create Vertices Bufffer error!!!!" << std::endl;
		return;
	}


	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * IndicsCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA isbd;
	isbd.pSysMem = SkullMesh.Indices.data();

	hr = pD3DDeviceInst->CreateBuffer(&ibd, &isbd, &pBoxIB);
	
	if (FAILED(hr))
	{
		std::cout << "Create Indices Bufffer error!!!!" << std::endl;
		return;
	}
}

void SkullApp::BuildVerticeLayout()
{
	/** add the description of Texcoord*/
	D3D11_INPUT_ELEMENT_DESC VertexDesc[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	
	D3DX11_PASS_DESC passDesc;
	assert(SkullEffect);
	SkullEffect->GetTechPtr()->GetPassByIndex(0)->GetDesc(&passDesc);
	HRESULT hr = pD3DDeviceInst->CreateInputLayout(
		VertexDesc
		, 3
		, passDesc.pIAInputSignature
		, passDesc.IAInputSignatureSize
		, &pInputLayout);

	if (FAILED(hr))
	{
		std::cout << "Create InputLayout" << std::endl;
		return;
	}
	/**Setting the layout before draw scene*/
	//pD3DDevContInst->IASetInputLayout(pInputLayout);
}

void SkullApp::BuildFX()
{
	
	DWORD shaderFlags = 0;
#if _DEBUG||DEBUG
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
	ID3DBlob* compilationMsg = nullptr;
	/**the old code has been 废弃*/

	return;
}

