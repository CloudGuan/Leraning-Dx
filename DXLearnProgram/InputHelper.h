#pragma once

/**
*	using for get the keybord and mouse input
*/


#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <windows.h>




class DXInputHelper
{
public:
	DXInputHelper(HWND hwnd, HINSTANCE hInstance, DWORD keyboardCoopFlags,DWORD mouseCoopFlags);
	virtual~DXInputHelper();
public:
	void  Tick();
	float MouseX();
	float MouseY();
	float MouseZ();
private:
	/**��������Ҫʹ�õ��豸�Ͷ��󣬷ֱ𴴽�*/
	LPDIRECTINPUT8		 DInput;
	LPDIRECTINPUTDEVICE8 MouseInputDev;
	DIMOUSESTATE2		 MouseState;
	LPDIRECTINPUTDEVICE8 KeyBoardInputDev;
	char                 KeyboardState[256];
};