#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <directxcolors.h>





LRESULT CALLBACK MainWndProc(HWND, UINT , WPARAM , LPARAM );




class D3DAppBase
{
public:

	D3DAppBase(HINSTANCE hInstcane,int nShowCmd);
	virtual ~D3DAppBase();

public:
	HINSTANCE GetWndHandle(){ return hWndInst; }
	HWND	GetMainWindows(){ return hWnd; }

	float AspectRatio() const
	{
		return static_cast<float>(iClientWidth) / iClientHeigth;
	}
	virtual bool Init();
	virtual int Run();
	virtual void DrawScene();
	virtual void OnResize();
	virtual void UpdateScene();
	virtual LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
protected:
	bool InitWindow();
	bool InitDevice();

protected:
	HINSTANCE	hWndInst;
	HWND		hWnd;
	int			iShowCmd;
	
	int		iClientWidth;
	int		iClientHeigth;
	bool	bSupportMSAA;
	//UINT	uCreateDeviceFlags;
	UINT	uMsaaQuailty;

	ID3D11Device*			pD3DDeviceInst;
	ID3D11DeviceContext*	pD3DDevContInst;
	IDXGISwapChain*			pSwapChain;
	ID3D11RenderTargetView*	pRenderingtargetView;
	ID3D11Texture2D*		pDepthBuffer;
	ID3D11DepthStencilView* pDepthStencilView;
	D3D11_VIEWPORT			mD3dViewPort;
};