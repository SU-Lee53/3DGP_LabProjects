﻿// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#pragma comment(lib, "winmm.lib")

// Direct3D related headers
#include <string>	
#include <wrl.h>
#include <shellapi.h>	

// D3D12
#include <d3d12.h>
#include <dxgi1_4.h>


// D3DCompiler
#include <d3dcompiler.h>

// DirectXMath
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

// dxgidebug
#include <dxgidebug.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

using namespace Microsoft::WRL;


// Import libraries
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

// constants
const static DWORD FRAME_BUFFER_WIDTH = 800;
const static DWORD FRAME_BUFFER_HEIGHT = 600;

// fullscreen mode
//#define _WITH_SWAPCHAIN_FULLSCREEN_STATE

// CRT debug
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif

// Utility functions

// Creates buffer resources by buffer's heap type and initialize if initial data is available
extern ID3D12Resource* CreateBufferResources(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, 
	void* pData, UINT nBytes, D3D12_HEAP_TYPE d3dHeapType = D3D12_HEAP_TYPE_UPLOAD, 
	D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, ID3D12Resource** ppd3dUploadBuffer = nullptr);
