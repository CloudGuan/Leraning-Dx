
#include "DXTestClass.h"
#include "SKull.h"
#include "resource.h"


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	/*D3DAppBase D3DApp(hInstance, nCmdShow);

	if (!D3DApp.Init())
	{
		return 0;
	}
	return D3DApp.Run();*/

	SkullApp MyBox(hInstance,nCmdShow);
	if (!MyBox.Init())
	{
		return 0;
	}
	return MyBox.Run();
}

