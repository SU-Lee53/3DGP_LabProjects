#pragma once
#include "Timer.h"
#include "Scene.h"
#include "Camera.h"
#include "Player.h"

class CGameFramework {
public:
	CGameFramework();
	~CGameFramework();

public:
	// Initialize framework
	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd, bool bEnableDebugLayer, bool bEnableGBV);
	void OnDestroy();

private:
	void CreateD3DDevice(bool bEnableDebugLayer, bool bEnableGBV);
	void CreateSwapChain();
	void CreateRTVAndDSVDescriptorHeaps();
	void CreateCommandQueueAndList();

private:
	void CreateRenderTargetViews();
	void CreateDepthStencilView();

public:
	// Initialize & Destroy GameObjects
	void BuildObjects();
	void ReleaseObjects();

public:
	// Core of CGameFramework : Processing Input / Aniamtion / Render
	void ProcessInput();
	void AnimateObjects();
	void FrameAdvance();

	void Render();

private:
	void RenderBegin();
	void RenderEnd();
	void Present();

private:
	// GPU-CPU Sync
	void WaitForGPUComplete();

public:
	// Processing Window Messages
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

public:
	// Added in LabProject04
	void ChangeSwapChainState();

public:
	// Addedin LabProject05
	void MoveToNextFrame();


private:
	HINSTANCE	m_hInstance = nullptr;
	HWND		m_hWnd = nullptr;

	int m_nWndClientWidth = 0;
	int m_nWndClientHeight = 0;

	// Pointer to interface of DXGI factory
	IDXGIFactory4* m_pdxgiFactory = nullptr;

	// Pointer to interface of SwapChain
	// SwapChain is mostly used for control display
	IDXGISwapChain3* m_pdxgiSwapChain = nullptr;

	// Pointer to interface of D3D12 device
	// Device is mostly use for creating resorces
	ID3D12Device* m_pd3dDevice = nullptr;

	// Enable Debug layer and GBV(GPU-Based Validation)
	bool m_bEnableDebugLayer = false;
	bool m_bEnableGBV = false;

	// Enable MSAA multi-sampling and set multi-sampling level
	bool m_bMsaa4xEnable = false;
	bool m_nMsaa4xQualityLevels = 0;

	// Back buffer count for SwapChain
	const static UINT m_nSwapChainBuffers = 2;

	// Current back buffer index in SwapChain
	UINT m_nSwapChainBufferIndex = 0;

private:
	// Framework resources
	// RTV
	ID3D12Resource*			m_ppd3dRenderTargetBuffers[m_nSwapChainBuffers] = {};
	ID3D12DescriptorHeap*	m_pd3dRTVDescriptorHeap = nullptr;
	UINT					m_nRTVDescriptorIncrementSize = 0;

	// DSV
	ID3D12Resource*			m_pd3dDepthStencilBuffer = nullptr;
	ID3D12DescriptorHeap*	m_pd3dDSVDescriptorHeap = nullptr;
	UINT					m_nDSVDescriptorIncrementSize = 0;
	
	// Command Queue, Command Allocator, Command List
	ID3D12CommandQueue*			m_pd3dCommandQueue = nullptr;
	ID3D12CommandAllocator*		m_pd3dCommandAllocator = nullptr;
	ID3D12GraphicsCommandList*	m_pd3dCommandList = nullptr;

	// Pipeline State Object
	ID3D12PipelineState* m_pd3dPipelineState = nullptr;

	// Fence & Fence value
	ID3D12Fence*	m_pd3dFence = nullptr;
	//UINT64			m_nFenceValue = 0;
	UINT64			m_nFenceValues[m_nSwapChainBuffers] = {};
	HANDLE			m_hFenceEvent = nullptr;

	// Removed from in LabProject07
	//	// Viewport & Scissor rect 
	//	D3D12_VIEWPORT	m_d3dViewport = {};
	//	D3D12_RECT		m_d3dScissorRect = {};

public:
	// Added in LabProject05
	CCamera* m_pCamera = nullptr;

private:
	// Added in LabProject03
	CGameTimer m_GameTimer;
	_TCHAR m_pszFrameRate[50];

private:
	// Added in LabProject05
	CScene* m_pScene = nullptr;

public:
	CPlayer* m_pPlayer = NULL;

	// Last cursor pos when mouse button clicked
	POINT m_ptOldCursorPos;

};

