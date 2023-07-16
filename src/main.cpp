#include <iostream>
#include <Windows.h>
#include <d3d9.h>

#pragma comment (lib, "d3d9.lib")

HWND g_hWnd = NULL;
LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;


bool InitD3D(HWND hwnd) {
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL) {
		return false;
	}

	D3DPRESENT_PARAMETERS d3dp;
	memset(&d3dp, 0, sizeof(d3dp));
	d3dp.Windowed = true;
	d3dp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dp.BackBufferWidth = D3DFMT_UNKNOWN;

	HRESULT hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dp, &g_pd3dDevice);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}


void Render() {
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(18, 141, 168), 1.0f, 0);

	g_pd3dDevice->EndScene();

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLne, int cmdShow) {
	const char* wndClassName = "D3D9WindowClass";
	const char* wndTitle = "Direct3D 9 Window";
	const int wndWidth = 800;
	const int wndHeight = 600;

	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L,
					  GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
					  wndClassName, NULL };

	RegisterClassEx(&wc);

	g_hWnd = CreateWindow(wndClassName, wndTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, wndWidth, wndHeight,
		NULL, NULL, wc.hInstance, NULL);

	// Initialiser Direct3D
	if (!InitD3D(g_hWnd))
		return 1;

	ShowWindow(g_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(g_hWnd);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Render();
		}
	}

	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();
	if (g_pD3D != NULL)
		g_pD3D->Release();

	DestroyWindow(g_hWnd);

	UnregisterClass(wndClassName, wc.hInstance);

	return 0;
}