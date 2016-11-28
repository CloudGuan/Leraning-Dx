#include "SKull.h"
#include <iostream>
#include <D3Dcompiler.h>
#include <fstream>
#include <string>
#include <vector>
#include "CommonHeader.h"


SkullApp::SkullApp(HINSTANCE hInstance, int nShowCmd)
	:D3DAppBase(hInstance,nShowCmd)
	, pBoxVB(nullptr)
	, pBoxIB(nullptr)
	, pFX(nullptr)
	, pTech(nullptr)
	, pInputLayout(nullptr)
	, mfxWorldViewProj(nullptr)
	, mTheta(1.5f*DirectX::XM_PI), mPhi(0.25f*DirectX::XM_PI)
	, mRadius(8.0f)
	, EyePoint(0.0f,2.0f,0.0f)
{
	DirectX::XMMATRIX I = DirectX::XMMatrixIdentity();
	//DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(0.0f, -2.0f, 0.0f);
	XMStoreFloat4x4(&mWord, I);
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);

	/**declear some paramitors */
	DlightSource.Ambient = DirectX::XMFLOAT4(0.2f,0.2f,0.2f,1.0f);
	DlightSource.Diffuse = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	DlightSource.Specular= DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	DlightSource.Direction = DirectX::XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	PLightSource.Ambinet = DirectX::XMFLOAT4(0.3f,0.3f,0.3f,1.0f);
	PLightSource.Diffuse = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	PLightSource.Specular = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	PLightSource.Att = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);

	SkullMaterial.Ambient = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	SkullMaterial.Diffuse = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	SkullMaterial.Specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);

}

SkullApp::~SkullApp()
{

}

bool SkullApp::Init()
{
	if(!D3DAppBase::Init())
	{
		printf_s("Init D3D Error!!!!");
		return false;
	}
	BuildBuffers();
	BuildFX();
	BuildVerticeLayout();

	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	pD3DDeviceInst->CreateRasterizerState(&wireframeDesc, &mWireframeRS);

	return true;
}

void SkullApp::DrawScene()
{
	assert(pD3DDevContInst);
	assert(pD3DDeviceInst);

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
	UINT stride = sizeof(SkullVect);
	UINT offset = 0;
	

	

	DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4(&mWord);
	DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&mView);
	DirectX::XMMATRIX proj = DirectX::XMLoadFloat4x4(&mProj);
	DirectX::XMMATRIX WVP = world*view*proj;
	mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&WVP));

	fxDLight->SetRawValue(&DlightSource, 0, sizeof(DlightSource));
	fxPoint->SetRawValue(&PLightSource, 0, sizeof(PLightSource));
	fxEyePoint->SetRawValue(&EyePoint, 0, sizeof(EyePoint));


	D3DX11_TECHNIQUE_DESC techDesc;
	pTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		pD3DDevContInst->IASetVertexBuffers(0, 1, &pBoxVB, &stride, &offset);
		pD3DDevContInst->IASetIndexBuffer(pBoxIB, DXGI_FORMAT_R32_UINT, 0);

		fxMaterial->SetRawValue(&SkullMaterial, 0, sizeof(SkullMaterial));
		pTech->GetPassByIndex(p)->Apply(0, pD3DDevContInst);

		// 立方体有36个索引
		pD3DDevContInst->DrawIndexed(IndicsCount*3, 0, 0);
	}

	HRESULT hr = pSwapChain->Present(0, 0);
}

void SkullApp::OnResize()
{
	D3DAppBase::OnResize();

	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(0.25f*DirectX::XM_PI, AspectRatio(), 1.0f, 1500.0f);
	XMStoreFloat4x4(&mProj, P);
}

void SkullApp::UpdateScene(float DelteTime)
{
	//mTheta += DelteTime*DirectX::XM_PI;
	float x = mRadius*sinf(mPhi)*cosf(mTheta);
	float z = mRadius*sinf(mPhi)*sinf(mTheta);
	float y = mRadius*cosf(mPhi);

	// Build the view matrix.
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(x, 10, z, 1.0f);
	DirectX::XMVECTOR target = DirectX::XMVectorZero();
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, V);
}

void SkullApp::BuildBuffers()
{
	std::ifstream fin;
	fin.open("./Common/skull.txt",std::ios_base::in);

	if (!fin.is_open())
	{
		MessageBox(0, L"open skull file error", 0, 0);
		return;
	}
	UINT VertexCount = 0;
	std::string Ignore;

	
	fin >> Ignore >> VertexCount;
	fin >> Ignore >> IndicsCount;
	std::vector<SkullVect> SkBuffers(VertexCount);
	std::vector<UINT> Indices(IndicsCount * 3);

	fin >> Ignore >> Ignore >> Ignore >> Ignore;
	float JumpNum;
	for (int index = 0; index < VertexCount; index++)
	{
		fin >> SkBuffers[index].Pos.x >> SkBuffers[index].Pos.y >> SkBuffers[index].Pos.z;
		/**Scape Normal info*/
		fin >> SkBuffers[index].Normal.x >> SkBuffers[index].Normal.y >> SkBuffers[index].Normal.z;
	
	}
	/**we should change the type */
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(SkullVect) * VertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA sbd;
	sbd.pSysMem = SkBuffers.data();
	HRESULT hr = pD3DDeviceInst->CreateBuffer(
		&vbd,
		&sbd,
		&pBoxVB
		);
	if (FAILED(hr))
	{
		fin.close();
		std::cout << "Create Vertices Bufffer error!!!!" << std::endl;
		return;
	}

	fin >> Ignore;
	fin >> Ignore;
	fin >> Ignore;

	for (int index = 0; index < IndicsCount; index++)
	{
		fin >> Indices[index*3+0];
		fin >> Indices[index*3+1];
		fin >> Indices[index*3+2];
	}

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * IndicsCount*3;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA isbd;
	isbd.pSysMem = Indices.data();

	hr = pD3DDeviceInst->CreateBuffer(&ibd, &isbd, &pBoxIB);
	fin.close();
	if (FAILED(hr))
	{
		std::cout << "Create Indices Bufffer error!!!!" << std::endl;
		return;
	}
}

void SkullApp::BuildVerticeLayout()
{
	D3D11_INPUT_ELEMENT_DESC VertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};


	D3DX11_PASS_DESC passDesc;
	pTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HRESULT hr = pD3DDeviceInst->CreateInputLayout(
		VertexDesc
		, 2
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



	HRESULT hr = D3DX11CompileEffectFromFile(L"FSDemo.fx", nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, shaderFlags,
		0, pD3DDeviceInst, &pFX, &compilationMsg);
	if (compilationMsg)
	{
		char* Str = (char*)compilationMsg->GetBufferPointer();
		printf_s("%s", Str);
		compilationMsg->Release();
	}

	if (FAILED(hr))
	{
		ppBlobOut->Release();
		return;
	}

	/**这里要根据fx文件得名字进行更改*/
	pTech = pFX->GetTechniqueByName("LightTech");
	mfxWorldViewProj = pFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	fxDLight = pFX->GetVariableByName("gDLight");
	fxPoint = pFX->GetVariableByName("gPLight");
	fxEyePoint = pFX->GetVariableByName("gEyePos")->AsVector();
	fxMaterial = pFX->GetVariableByName("gMaterial");

	return;
}

