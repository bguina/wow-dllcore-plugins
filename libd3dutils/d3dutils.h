#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <Windows.h>
#include <TlHelp32.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")

#include <psapi.h> 
#pragma comment(lib, "psapi" )

#define safe_release(p) if (p) { p->Release(); p = nullptr; } 

using namespace DirectX;

#define VMT_PRESENT (UINT)IDXGISwapChainVMT::Present
#define PRESENT_STUB_SIZE 5


struct ConstantBuffer {
	XMMATRIX mProjection;
};

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
};

using fnPresent = HRESULT(__stdcall*)(IDXGISwapChain* pThis, UINT SyncInterval, UINT Flags);

HRESULT __stdcall hkPresent(
	IDXGISwapChain* pThis,
	UINT SyncInterval,
	UINT Flags
);
bool Hook(void* pSrc, void* pDst, size_t size);
bool WriteMem(void* pDst, char* pBytes, size_t size);
bool HookD3D(void newHkPresent(
	IDXGISwapChain* pThis,
	UINT SyncInterval,
	UINT Flags
));
bool CompileShader(const char* szShader, const char* szEntrypoint, const char* szTarget, ID3D10Blob** pBlob);
bool InitD3DHook(IDXGISwapChain* pSwapchain);
void CleanupD3D();
HWND FindMainWindow(DWORD dwPID);
BOOL CALLBACK EnumWindowsCallback(
	HWND hWnd,
	LPARAM lParam
);

// adding this code ripped off SO to find the "main window" as a fallback to RSGetViewports
struct HandleData
{
	DWORD pid;
	HWND hWnd;
};

void drawSomeTriangle();
HWND FindMainWindow(DWORD dwPID);
void UnhookD3D(LPVOID pHandle);
