#include <global.hpp>

GlobalData g_data;

LRESULT CALLBACK hwndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int main() {
	WNDCLASS wc = {};
	wc.lpfnWndProc = hwndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = "WindowClass";
	RegisterClass(&wc);

	g_data.hwnd = CreateWindow(wc.lpszClassName, "Window", WS_OVERLAPPEDWINDOW, 0, 0, 640, 480, NULL, NULL, wc.hInstance, NULL);
	ShowWindow(g_data.hwnd, SW_SHOW);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CloseWindow(g_data.hwnd);
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