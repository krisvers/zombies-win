#include <global.hpp>
#include <iostream>

GlobalData g_data;

LRESULT CALLBACK hwndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int main() {
	g_data.hinstance = GetModuleHandle(NULL);

	WNDCLASS wc = {};
	wc.lpfnWndProc = hwndProc;
	wc.hInstance = g_data.hinstance;
	wc.lpszClassName = "WindowClass";
	RegisterClass(&wc);

	g_data.hwnd = CreateWindow(wc.lpszClassName, "Window", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, g_data.hinstance, NULL);
	if (!g_data.hwnd) {
		return 1;
	}

	ShowWindow(g_data.hwnd, SW_SHOW);
	SetForegroundWindow(g_data.hwnd);

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

	HRESULT hr = D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION, &swapchainDesc, &g_data.d3d10.swapChain, &g_data.d3d10.device);
	if (FAILED(hr)) {
		std::cout << "Failed to create device and swap chain" << std::endl;
		return 1;
	}

	ID3D10Texture2D* backBuffer = NULL;
	hr = g_data.d3d10.swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (void**) &backBuffer);
	if (FAILED(hr)) {
		std::cout << "Failed to get back buffer" << std::endl;
		return 1;
	}

	hr = g_data.d3d10.device->CreateRenderTargetView(backBuffer, NULL, &g_data.d3d10.renderTarget);
	if (FAILED(hr)) {
		std::cout << "Failed to create render target view" << std::endl;
		return 1;
	}
	backBuffer->Release();

	hr = DirectInput8Create(g_data.hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**) &g_data.dinput.dinput, NULL);
	if (FAILED(hr)) {
		std::cout << "Failed to create DirectInput8 object" << std::endl;
		return 1;
	}

	hr = g_data.dinput.dinput->CreateDevice(GUID_SysKeyboard, &g_data.dinput.keyboard, NULL);
	if (FAILED(hr)) {
		std::cout << "Failed to create keyboard device" << std::endl;
		return 1;
	}

	hr = g_data.dinput.keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) {
		std::cout << "Failed to set data format for keyboard" << std::endl;
		return 1;
	}

	hr = g_data.dinput.keyboard->SetCooperativeLevel(g_data.hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr)) {
		std::cout << "Failed to set cooperative level for keyboard" << std::endl;
		return 1;
	}

	hr = g_data.dinput.keyboard->Acquire();
	if (FAILED(hr)) {
		std::cout << "Failed to acquire keyboard: " << std::hex << hr << std::endl;
		return 1;
	}

	hr = g_data.dinput.dinput->CreateDevice(GUID_SysMouse, &g_data.dinput.mouse, NULL);
	if (FAILED(hr)) {
		std::cout << "Failed to create mouse device" << std::endl;
		return 1;
	}

	hr = g_data.dinput.mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr)) {
		std::cout << "Failed to set data format for mouse" << std::endl;
		return 1;
	}

	hr = g_data.dinput.mouse->SetCooperativeLevel(g_data.hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr)) {
		std::cout << "Failed to set cooperative level for mouse" << std::endl;
		return 1;
	}

	hr = g_data.dinput.mouse->Acquire();
	if (FAILED(hr)) {
		std::cout << "Failed to acquire mouse" << std::endl;
		return 1;
	}

	hr = DirectSoundCreate8(NULL, &g_data.dsound.dsound, NULL);
	if (FAILED(hr)) {
		std::cout << "Failed to create DirectSound object" << std::endl;
		return 1;
	}

	hr = g_data.dsound.dsound->SetCooperativeLevel(g_data.hwnd, DSSCL_PRIORITY);
	if (FAILED(hr)) {
		std::cout << "Failed to set cooperative level for DirectSound" << std::endl;
		return 1;
	}

	JavaVMOption option = {};
	option.optionString = const_cast<char*>("-Djava.class.path=scripts\\game-script.jar");
	option.extraInfo = NULL;

	JavaVMInitArgs vmArgs = {};
	vmArgs.version = JNI_VERSION_1_8;
	vmArgs.nOptions = 1;
	vmArgs.options = &option;
	vmArgs.ignoreUnrecognized = JNI_TRUE;

	if (JNI_CreateJavaVM(&g_data.java.jvm, (void**)&g_data.java.env, &vmArgs) != JNI_OK) {
		std::cout << "Failed to create Java VM" << std::endl;
		return 1;
	}

	jclass scriptClass = g_data.java.env->FindClass("GameScript");
	if (!scriptClass) {
		std::cout << "Failed to find GameScript class" << std::endl;
		return 1;
	}

	jmethodID setupMethod = g_data.java.env->GetStaticMethodID(scriptClass, "setup", "()V");
	if (!setupMethod) {
		std::cout << "Failed to find main method" << std::endl;
		return 1;
	}

	g_data.java.env->CallStaticVoidMethod(scriptClass, setupMethod);

	g_data.running = true;

	MSG msg = {};
	while (g_data.running) {
		g_data.dinput.keyboard->GetDeviceState(sizeof(g_data.dinput.keyboardState), g_data.dinput.keyboardState);
		g_data.dinput.mouse->GetDeviceState(sizeof(g_data.dinput.mouseState), &g_data.dinput.mouseState);

		/* print */
		std::cout << "Keyboard: ";
		for (int i = 0; i < 256; i++) {
			if (g_data.dinput.keyboardState[i] & 0x80) {
				std::cout << i << " ";
			}
		}

		std::cout << std::endl;
		std::cout << "Mouse: " << g_data.dinput.mouseState.lX << " " << g_data.dinput.mouseState.lY << " " << g_data.dinput.mouseState.lZ << std::endl;

		while (PeekMessage(&msg, g_data.hwnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	g_data.dsound.dsound->Release();
	g_data.dinput.mouse->Unacquire();
	g_data.dinput.mouse->Release();
	g_data.dinput.keyboard->Unacquire();
	g_data.dinput.keyboard->Release();
	g_data.dinput.dinput->Release();
	g_data.d3d10.renderTarget->Release();
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