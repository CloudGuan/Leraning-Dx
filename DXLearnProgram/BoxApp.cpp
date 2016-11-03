#include "BoxApp.h"
#include <stdio.h>
#include <iostream>
#include "CommonHeader.h"
#include <d3dcompiler.h>
#include <vector>
#include <d3dcommon.h>


BoxApp::BoxApp(HINSTANCE hInstance, int nShowCmd)
	:D3DAppBase(hInstance, nShowCmd)
	, pBoxVB(nullptr)
	, pBoxIB(nullptr)
	, pFX(nullptr)
	, pTech(nullptr)
	, pInputLayout(nullptr)
	, mfxWorldViewProj(nullptr)
	, mTheta(1.5f*DirectX::XM_PI), mPhi(0.25f*DirectX::XM_PI), mRadius(5.0f)
{
	DirectX::XMMATRIX I = DirectX::XMMatrixIdentity();
	XMStoreFloat4x4(&mWord, I);
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);
}

bool BoxApp::Init()
{
	if (!D3DAppBase::Init())
	{
		printf_s("Init D3D Error!!!!");
		return false;
	}
	BuildBuffers();
	BuildFX();
	BuildVerticeLayout();

	return true;
}

void BoxApp::DrawScene()
{
	assert(pD3DDevContInst);
	assert(pD3DDeviceInst);

	pD3DDevContInst->ClearRenderTargetView(
		pRenderingtargetView,
		reinterpret_cast<const float*>(&Colors::LightSteelBlue));
	pD3DDevContInst->ClearDepthStencilView(
		pDepthStencilView,
		D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	pD3DDevContInst->IASetInputLayout(pInputLayout);
	pD3DDevContInst->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(BoxVertice);
	UINT offset = 0;
	pD3DDevContInst->IASetVertexBuffers(0, 1, &pBoxVB,&stride,&offset);
	pD3DDevContInst->IASetIndexBuffer(pBoxIB, DXGI_FORMAT_R32_UINT, 0);

	DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4(&mWord);
	DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&mView);
	DirectX::XMMATRIX proj = DirectX::XMLoadFloat4x4(&mProj);
	DirectX::XMMATRIX WVP = world*view*proj;
	mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&WVP));
	
	D3DX11_TECHNIQUE_DESC techDesc;
	pTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		pTech->GetPassByIndex(p)->Apply(0, pD3DDevContInst);

		// 立方体有36个索引
		pD3DDevContInst->DrawIndexed(36, 0, 0);
	}

	HRESULT hr=pSwapChain->Present(0, 0);
}

void BoxApp::OnResize()
{
	D3DAppBase::OnResize();

	DirectX::XMMATRIX P =DirectX::XMMatrixPerspectiveFovLH(0.25f*DirectX::XM_PI, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}

void BoxApp::UpdateScene()
{
	float x = mRadius*sinf(mPhi)*cosf(mTheta);
	float z = mRadius*sinf(mPhi)*sinf(mTheta);
	float y = mRadius*cosf(mPhi);

	// Build the view matrix.
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(x, y, z, 1.0f);
	DirectX::XMVECTOR target = DirectX::XMVectorZero();
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, V);
}

void BoxApp::BuildVerticeLayout()
{
	D3D11_INPUT_ELEMENT_DESC VertexDesc[] = 
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12 , D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	
	D3DX11_PASS_DESC passDesc;
	pTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HRESULT hr=pD3DDeviceInst->CreateInputLayout(
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

void BoxApp::BuildFX()
{
	DWORD shaderFlags = 0;
#if _DEBUG||DEBUG
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
	ID3DBlob* compilationMsg= nullptr;
	/**the old code has been 废弃*/
	
	

	HRESULT hr = D3DX11CompileEffectFromFile(L"BoxEffect.fx", nullptr,
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

	pTech = pFX->GetTechniqueByName("ColorTech");
	mfxWorldViewProj = pFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	return;
}

void BoxApp::BuildBuffers()
{
	/**Vertex has been update to simple vertex*/
	BoxVertice Vertices[] =
	{
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4((const float*)&Colors::White) },
		{ DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4((const float*)&Colors::Black) },
		{ DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4((const float*)&Colors::Red) },
		{ DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4((const float*)&Colors::Green) },
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4((const float*)&Colors::Blue) },
		{ DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4((const float*)&Colors::Yellow) },
		{ DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4((const float*)&Colors::Cyan) },
		{ DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4((const float*)&Colors::Magenta) }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(BoxVertice) * 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA sbd;
	sbd.pSysMem = Vertices;
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
	/**then we bind buffer to the device before start drawing scene*/
	/*UINT stride = sizeof(BoxVertice);
	UINT offect = 0;
	pD3DDevContInst->IASetVertexBuffers(0, 1, &pBoxVB, &stride, &offect);*/

	UINT indices[] =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 36;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA isbd;
	isbd.pSysMem = indices;

	hr=pD3DDeviceInst->CreateBuffer(&ibd,&isbd,&pBoxIB);

	if (FAILED(hr))
	{
		std::cout << "Create Indices Bufffer error!!!!" << std::endl;
		return;
	}

	/**we also need bind this indices to pieppleline */
	//pD3DDevContInst->IASetIndexBuffer(pBoxIB, DXGI_FORMAT_R32_UINT,0);

}

BoxApp::~BoxApp()
{

}

