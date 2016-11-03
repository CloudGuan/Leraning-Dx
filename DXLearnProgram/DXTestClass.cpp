#include "DXTestClass.h"
#include "resource.h"


namespace
{
	D3DAppBase* gD3dApp = nullptr;
}

/**Using for listening the windows msg*/
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return gD3dApp->WndProc(hwnd, msg, wParam, lParam);
}

D3DAppBase::D3DAppBase(HINSTANCE hInstcane, int nShowCmd)
	:hWndInst(hInstcane)
	, iShowCmd(nShowCmd)
	, uMsaaQuailty(0)
	, pD3DDeviceInst(nullptr)
	, pD3DDevContInst(nullptr)
	, pSwapChain(nullptr)
	, pRenderingtargetView(nullptr)
	, pDepthBuffer(nullptr)
	, pDepthStencilView(nullptr)
	, iClientWidth(800)
	, iClientHeigth(600)
{
	/**将一段内存置为零*/
	ZeroMemory(&mD3dViewPort,sizeof(D3D11_VIEWPORT));
	gD3dApp = this;
}

D3DAppBase::~D3DAppBase()
{
	if (pSwapChain) pSwapChain->Release();
	if (pDepthBuffer) pDepthBuffer->Release();
	if (pRenderingtargetView) pRenderingtargetView->Release();
	if (pD3DDeviceInst) pD3DDeviceInst->Release();
	if (pD3DDevContInst) pD3DDevContInst->Release();
	
}


bool D3DAppBase::Init()
{
	if (!InitWindow())
		return false;

	if (!InitDevice())
		return false;
	return true;
}

int D3DAppBase::Run()
{
	MSG msg = { 0 };
	AppTimer.RestTimer();
	while (WM_QUIT != msg.message)
	{
		/**Using for process windows messages*/
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			AppTimer.Tick();
			if (!bPaused)
			{
				/**Do something for rendering*/
				UpdateScene(AppTimer.DeltaTime());
				DrawScene();
			}
			else
			{
				Sleep(100);
			}
		}
		
	}
	return msg.wParam;
}

void D3DAppBase::DrawScene()
{
	assert(pD3DDevContInst);
	assert(pD3DDeviceInst);

	pD3DDevContInst->ClearRenderTargetView(
		pRenderingtargetView,
		DirectX::Colors::Blue);
	pD3DDevContInst->ClearDepthStencilView(
		pDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
	pSwapChain->Present(0, 0);
}

void D3DAppBase::OnResize()
{
	/**check if the swapchain d3ddevice and d3dContect has been Initilized*/
	assert(pSwapChain);
	assert(pD3DDeviceInst);
	assert(pD3DDevContInst);

	if (pRenderingtargetView) pRenderingtargetView->Release();
	if (pDepthStencilView) pDepthStencilView->Release();
	if (pDepthBuffer) pDepthBuffer->Release();
	

	pSwapChain->ResizeBuffers(1, iClientWidth, iClientHeigth,DXGI_FORMAT_R8G8B8A8_UNORM,0);
	ID3D11Texture2D* pBackBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	pD3DDeviceInst->CreateRenderTargetView(pBackBuffer, 0, &pRenderingtargetView);
	pBackBuffer->Release();

	/**Create the Depth/Stencil Buffer and View */
	D3D11_TEXTURE2D_DESC DepthBufferDesc;
	DepthBufferDesc.Width = iClientWidth;
	DepthBufferDesc.Height = iClientHeigth;
	DepthBufferDesc.MipLevels = 1;
	DepthBufferDesc.ArraySize = 1;
	DepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (uMsaaQuailty > 0)
	{
		DepthBufferDesc.SampleDesc.Count = uMsaaQuailty;
		DepthBufferDesc.SampleDesc.Quality = uMsaaQuailty - 1;
	}
	else
	{
		DepthBufferDesc.SampleDesc.Count = 1;
		DepthBufferDesc.SampleDesc.Quality = 0;
	}

	DepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthBufferDesc.CPUAccessFlags = 0;
	DepthBufferDesc.MiscFlags = 0;

	HRESULT hr = pD3DDeviceInst->CreateTexture2D(
		&DepthBufferDesc,
		0,
		&pDepthBuffer);
	if (FAILED(hr))
		return;

	hr = pD3DDeviceInst->CreateDepthStencilView(
		pDepthBuffer,
		0,
		&pDepthStencilView);

	if (FAILED(hr))
		return;

	pD3DDevContInst->OMSetRenderTargets(1, &pRenderingtargetView, pDepthStencilView);

	mD3dViewPort.TopLeftX = 0.0f;
	mD3dViewPort.TopLeftY = 0.0f;
	mD3dViewPort.Width = static_cast<float>(iClientWidth);
	mD3dViewPort.Height = static_cast<float>(iClientHeigth);
	mD3dViewPort.MinDepth = 0.0f;
	mD3dViewPort.MaxDepth = 1.0f;

	pD3DDevContInst->RSSetViewports(1, &mD3dViewPort);
}

void D3DAppBase::UpdateScene(float DelteTime)
{

}

/**Init Windows for Application*/
bool D3DAppBase::InitWindow()
{
	/**Fill out struct using for init*/
	WNDCLASSEX WinSex;
	WinSex.cbSize = sizeof(WNDCLASSEX);
	WinSex.style = CS_HREDRAW | CS_VREDRAW;
	WinSex.lpfnWndProc = MainWndProc;
	WinSex.cbClsExtra = 0;
	WinSex.cbWndExtra = 0;
	WinSex.hInstance = hWndInst;
	WinSex.hIcon = LoadIcon(hWndInst, (LPCTSTR)IDI_ICON1);
	WinSex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	WinSex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WinSex.lpszMenuName = nullptr;
	WinSex.lpszClassName = L"TutorialWindowClass";
	WinSex.hIconSm = LoadIcon(hWndInst, (LPCTSTR)IDI_ICON1);

	if (!RegisterClassEx(&WinSex))
	{
		return false;
	}
	//hWndInst = hInstance;
	RECT rc = { 0, 0, iClientWidth, iClientHeigth };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	hWnd = CreateWindowEx(
		WS_EX_NOPARENTNOTIFY,
		L"TutorialWindowClass",
		L"MyFirstD3DApplication",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hWndInst,
		nullptr);
	if (!hWnd)
	{
		return false;
	}
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	return true;
}

bool D3DAppBase::InitDevice()
{
	/**In this function we need to init our DX*/
	/**Step One: Init DXDevice anf DeviceContext*/
	UINT uCreateDeviceFlags = 0;
#if defined(DEBUG)||defined(_DEBUG)
	uCreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL D3dFeatureLevel;

	HRESULT hr=D3D11CreateDevice(
		0
		,D3D_DRIVER_TYPE_HARDWARE
		,0
		,uCreateDeviceFlags
		,0, 0
		, D3D11_SDK_VERSION
		, &pD3DDeviceInst
		, &D3dFeatureLevel
		, &pD3DDevContInst
		);

	if (FAILED(hr))
	{
		MessageBox(0, L"Init D3Device Failed!!!",0,0);
		return false;
	}

	if (D3dFeatureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"TargetD3D Platform is error !!", 0, 0);
		return false;
	}

	/**Step Two: Checking MSAAx4 Support*/
	pD3DDeviceInst->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM
		, 4
		, &uMsaaQuailty);
	assert(uMsaaQuailty > 0);

	/**Step Three: Creating Swap Chain*/
	/**Fill out struct*/
	DXGI_SWAP_CHAIN_DESC SwapChain;
	SwapChain.BufferDesc.Width = iClientWidth;
	SwapChain.BufferDesc.Height = iClientHeigth;
	SwapChain.BufferDesc.RefreshRate.Numerator = 60;
	SwapChain.BufferDesc.RefreshRate.Denominator = 1;
	SwapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (uMsaaQuailty)
	{
		SwapChain.SampleDesc.Count = uMsaaQuailty;
		SwapChain.SampleDesc.Quality = uMsaaQuailty - 1;
	}
	else
	{
		SwapChain.SampleDesc.Count = 1;
		SwapChain.SampleDesc.Quality = 0;
	}
	SwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChain.BufferCount = 1;
	SwapChain.OutputWindow = hWnd;
	SwapChain.Windowed = true;
	SwapChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChain.Flags = 0;

	/**Using IDXGIFactory to create IDXGISwapChain*/

	IDXGIDevice* DxgiDevice = nullptr;
	IDXGIAdapter* DxgiAdapter = nullptr;
	hr = pD3DDeviceInst->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&DxgiDevice));
	if (FAILED(hr))
	{
		return false;
	}

	hr = DxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&DxgiAdapter));

	if (FAILED(hr))
	{
		return false;
	}

	IDXGIFactory* DxgiFactroy = nullptr;
	hr = DxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&DxgiFactroy));
	
	if (FAILED(hr))
	{
		return false;
	}
	hr=DxgiFactroy->CreateSwapChain(pD3DDeviceInst, &SwapChain, &pSwapChain);
	/**Release All temp properties */
	DxgiDevice->Release();
	DxgiAdapter->Release();
	DxgiFactroy->Release();

	/**Setp 4: Bind resource to pipeline stage for rendering it  */

	OnResize();
	return true;
}

LRESULT D3DAppBase::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		iClientWidth = LOWORD(lParam);
		iClientHeigth = HIWORD(lParam);
		if (pD3DDeviceInst)
		{
			OnResize();
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		// Note that this tutorial does not handle resizing (WM_SIZE) requests,
		// so we created the window without the resize border.

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

