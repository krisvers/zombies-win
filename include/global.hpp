#ifndef ZOMBIES_GLOBAL_HPP
#define ZOMBIES_GLOBAL_HPP

#include <Windows.h>

/*
#include <d3d12.h>
#include <dxgi1_4.h>
*/

#include <d3d10.h>

#include <dinput.h>

#include <dsound.h>

#include <vector>

/*
struct GlobalD3D12 {
	ID3D12Device* device = NULL;

	IDXGISwapChain3* swapChain = NULL;
	ID3D12DescriptorHeap* rtvHeap = NULL;
	std::vector<ID3D12Resource*> renderTargets;
	UINT frameIndex = 0;

	ID3D12CommandQueue* commandQueue = NULL;
	ID3D12CommandAllocator* commandAllocator = NULL;
	ID3D12GraphicsCommandList* commandList = NULL;

	ID3D12Fence* fence = NULL;
	HANDLE fenceEvent = NULL;
	UINT64 fenceValue = 0;
};
*/

struct GlobalD3D10 {
	ID3D10Device* device = NULL;
	IDXGISwapChain* swapChain = NULL;
	ID3D10RenderTargetView* renderTarget = NULL;
};

struct GlobalDInput {
	IDirectInput8* dinput = NULL;
	IDirectInputDevice8* keyboard = NULL;
	IDirectInputDevice8* mouse = NULL;
	DIMOUSESTATE mouseState = {};
	char keyboardState[256] = {};
};

struct GlobalDSound {
	IDirectSound8* dsound = NULL;
};

struct GlobalData {
	HINSTANCE hinstance = NULL;
	HWND hwnd = NULL;
	bool running = true;

	/* GlobalD3D12 d3d12 = {}; */
	GlobalD3D10 d3d10 = {};
	GlobalDInput dinput = {};
	GlobalDSound dsound = {};
};

extern GlobalData g_data;

#endif