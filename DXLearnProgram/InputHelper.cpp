#include "InputHelper.h"
#include <assert.h>

DXInputHelper::DXInputHelper(HWND hwnd, HINSTANCE hInstance, DWORD keyboardCoopFlags, DWORD mouseCoopFlags)
	:DInput(nullptr)
	, MouseInputDev(nullptr)
	, KeyBoardInputDev(nullptr)
{
	ZeroMemory(KeyboardState, sizeof(KeyboardState));
	ZeroMemory(&MouseState, sizeof(MouseState));

	HRESULT hr = DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&DInput,
		NULL);

	if (FAILED(hr))
	{
		//("Init Input device error"); 
		return;
	}

	hr = DInput->CreateDevice(
		GUID_SysMouse,
		&MouseInputDev,
		NULL
		);

	
	hr = MouseInputDev->SetCooperativeLevel(hwnd, mouseCoopFlags);
	hr = MouseInputDev->SetDataFormat(&c_dfDIMouse2);
	hr = MouseInputDev->Acquire();
	hr = MouseInputDev->Poll();
}

void DXInputHelper::Tick()
{
	assert(MouseInputDev);
	HRESULT hr = MouseInputDev->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&MouseState);
	if (FAILED(hr))
	{
		ZeroMemory(&MouseState, sizeof(MouseState));
		MouseInputDev->Acquire();
	}
}

float DXInputHelper::MouseX()
{
	return (float)MouseState.lX;
}

float DXInputHelper::MouseY()
{
	return (float)MouseState.lY;
}

float DXInputHelper::MouseZ()
{
	return (float)MouseState.lZ;
}

DXInputHelper::~DXInputHelper()
{

}

