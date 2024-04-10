#include <global.hpp>

GlobalData g_data;

LRESULT CALLBACK hwndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int main() {
	WNDCLASS wc = {};
	wc.lpfnWndProc = hwndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = "WindowClass";
	RegisterClass(&wc);

	g_data.hwnd = CreateWindow(wc.lpszClassName, "Window", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, wc.hInstance, NULL);
	if (!g_data.hwnd) {
		return 1;
	}

	DXGI_SWAP_CHAIN_DESC swapchainDesc = {};
	swapchainDesc.BufferDesc.Width = 800;
	swapchainDesc.BufferDesc.Height = 600;
	swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.SampleDesc.Quality = 0;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.BufferCount = 1;
	swapchainDesc.OutputWindow = g_data.hwnd;
	swapchainDesc.Windowed = TRUE;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (FAILED(D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION, &swapchainDesc, &g_data.d3d10.swapChain, &g_data.d3d10.device))) {
		return 1;
	}

	ShowWindow(g_data.hwnd, SW_SHOW);
	g_data.running = true;

	MSG msg = {};
	while (g_data.running) {
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	g_data.d3d10.swapChain->Release();
	g_data.d3d10.device->Release();
	DestroyWindow(g_data.hwnd);
	return 0;
}

LRESULT CALLBACK hwndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_CLOSE:
		g_data.running = false;
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}