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
	hr = DInput->CreateDevice(
		GUID_SysKeyboard,
		&KeyBoardInputDev,
		NULL
		);
	hr = KeyBoardInputDev->SetCooperativeLevel(hwnd, keyboardCoopFlags);
	hr = KeyBoardInputDev->SetDataFormat(&c_dfDIKeyboard);
	hr = KeyBoardInputDev->Acquire();
	hr = KeyBoardInputDev->Poll();

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

	hr = KeyBoardInputDev->GetDeviceState(sizeof(KeyboardState),(void**)&KeyboardState);
	if (FAILED(hr))
	{
		ZeroMemory(&KeyboardState, sizeof(KeyboardState));
		KeyBoardInputDev->Acquire();
	}
}

bool DXInputHelper::GetKey(char Key)
{
	return (KeyboardState[Key] & 0x80) != 0;
}

bool DXInputHelper::GetMouseButtonDown(int ButtonIndex)
{
	return (MouseState.rgbButtons[ButtonIndex] & 0x80)!=0;
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

