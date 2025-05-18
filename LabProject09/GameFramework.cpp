#include "stdafx.h"
#include "GameFramework.h"

CGameFramework::CGameFramework()
{
	m_pdxgiFactory = nullptr;
	m_pdxgiSwapChain = nullptr;
	m_pd3dDevice = nullptr;

	m_pd3dCommandAllocator = nullptr;
	m_pd3dCommandQueue = nullptr;
	m_pd3dPipelineState = nullptr;
	m_pd3dCommandList = nullptr;

	for (int i = 0; i < m_nSwapChainBuffers; i++)
		m_ppd3dRenderTargetBuffers[i] = nullptr;

	m_pd3dRTVDescriptorHeap = nullptr;
	m_nRTVDescriptorIncrementSize = 0;

	m_pd3dDepthStencilBuffer = nullptr;
	m_pd3dDSVDescriptorHeap = nullptr;
	m_nDSVDescriptorIncrementSize = 0;

	m_nSwapChainBufferIndex = 0;

	m_hFenceEvent = nullptr;
	m_pd3dDevice = nullptr;
	//m_nFenceValue = 0;
	for (int i = 0; i << m_nSwapChainBuffers; ++i)
		m_nFenceValues[i] = 0;

	m_pScene = nullptr;
	
	::_tcscpy_s(m_pszFrameRate, _T("LapProject ("));

}

CGameFramework::~CGameFramework()
{
}

bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd, bool bEnableDebugLayer, bool bEnableGBV)
{
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	// Create Core Objects
	CreateD3DDevice(bEnableDebugLayer, bEnableGBV);
	CreateCommandQueueAndList();
	CreateRTVAndDSVDescriptorHeaps();
	CreateSwapChain();
	CreateDepthStencilView();
	
	// Create renderable GameObjects
	BuildObjects();

	RECT r;
	::GetClientRect(m_hWnd, &r);

	m_nWndClientWidth = FRAME_BUFFER_WIDTH;
	m_nWndClientHeight = FRAME_BUFFER_HEIGHT;

	return true;
}

void CGameFramework::OnDestroy()
{
	// Wait for GPU executes all remain commands.
	WaitForGPUComplete();

	// Destroy GameObjects
	ReleaseObjects();

	// Close fence event
	::CloseHandle(m_hFenceEvent);

	for (int i = 0; i < m_nSwapChainBuffers; i++)
		if (m_ppd3dRenderTargetBuffers[i]) m_ppd3dRenderTargetBuffers[i]->Release();
	if (m_pd3dRTVDescriptorHeap) m_pd3dRTVDescriptorHeap->Release();

	if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
	if (m_pd3dDSVDescriptorHeap) m_pd3dDSVDescriptorHeap->Release();

	if (m_pd3dCommandAllocator) m_pd3dCommandAllocator->Release();
	if (m_pd3dCommandQueue) m_pd3dCommandQueue->Release();
	if (m_pd3dPipelineState) m_pd3dPipelineState->Release();
	if (m_pd3dCommandList) m_pd3dCommandList->Release();

	if (m_pd3dFence) m_pd3dFence->Release();

	m_pdxgiSwapChain->SetFullscreenState(FALSE, NULL);
	if (m_pdxgiSwapChain) m_pdxgiSwapChain->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();
	if (m_pdxgiFactory) m_pdxgiFactory->Release();

	if (m_bEnableDebugLayer)
	{
		IDXGIDebug1* pdxgiDebug = nullptr;
		DXGIGetDebugInterface1(0, IID_PPV_ARGS(&pdxgiDebug));
		HRESULT hr = pdxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
		pdxgiDebug->Release();
	}

}

void CGameFramework::CreateD3DDevice(bool bEnableDebugLayer, bool bEnableGBV)
{
	assert(!(!bEnableDebugLayer && bEnableGBV));

	HRESULT hr{};

	// Set Debug Layer
	UINT nDXGIFactoryFlags = 0;
	if (bEnableDebugLayer)
	{
		ID3D12Debug* pd3dDebugController = nullptr;
		hr = D3D12GetDebugInterface(IID_PPV_ARGS(&pd3dDebugController));
		if (FAILED(hr)) __debugbreak();
		nDXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

		if (pd3dDebugController)
		{
			pd3dDebugController->EnableDebugLayer();
			if (bEnableGBV)
			{
				ID3D12Debug5* pd3dDebugController5;
				if (SUCCEEDED(pd3dDebugController->QueryInterface(IID_PPV_ARGS(&pd3dDebugController5))))
				{
					pd3dDebugController5->SetEnableGPUBasedValidation(true);
					pd3dDebugController5->SetEnableAutoName(true);
					pd3dDebugController5->Release();
				}
			}

			pd3dDebugController->Release();
		}
	}

	// Create DXGIFactory
	if (FAILED(CreateDXGIFactory2(nDXGIFactoryFlags, IID_PPV_ARGS(&m_pdxgiFactory)))) __debugbreak();

	// Create DXGIAdapter
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_2,
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0
	};

	IDXGIAdapter* pdxgiAdapter = nullptr;
	DXGI_ADAPTER_DESC1 adapterDesc{};
	size_t bestMemory = 0;
	for (DWORD adapterIndex = 0; ; ++adapterIndex)
	{
		IDXGIAdapter1* pCurAdapter = nullptr;
		if (m_pdxgiFactory->EnumAdapters1(adapterIndex, &pCurAdapter) == DXGI_ERROR_NOT_FOUND) break;

		DXGI_ADAPTER_DESC1 curAdapterDesc{};
		if (FAILED(pCurAdapter->GetDesc1(&curAdapterDesc))) __debugbreak();

		size_t curMemory = curAdapterDesc.DedicatedVideoMemory / (1024 * 1024);

		if (curMemory > bestMemory)
		{
			bestMemory = curMemory;
			pdxgiAdapter = pCurAdapter;
			adapterDesc = curAdapterDesc;
		}
	}

	for (int i = 0; i < _countof(featureLevels); ++i)
	{
		if (SUCCEEDED(D3D12CreateDevice(pdxgiAdapter, featureLevels[i], IID_PPV_ARGS(&m_pd3dDevice)))) break;
	}

	// If failed to create device that featuring D3D_FEATURE_LEVEL_12_0 or over, create WARP device
	if (!pdxgiAdapter)
	{
		m_pdxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pdxgiAdapter));
		D3D12CreateDevice(pdxgiAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pd3dDevice));
	}

	// Release adapter (We don't need it after creating device)
	if (pdxgiAdapter) pdxgiAdapter->Release();

	// Set MSAA level
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS d3dMsaaQualityLevels;
	{
		d3dMsaaQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		d3dMsaaQualityLevels.SampleCount = 4;	// MSAA 4x Multisampling
		d3dMsaaQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
		d3dMsaaQualityLevels.NumQualityLevels = 0;
	}

	// Check device supported multi-sample quality level
	if (SUCCEEDED(m_pd3dDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &d3dMsaaQualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS))))
	{
		m_nMsaa4xQualityLevels = d3dMsaaQualityLevels.NumQualityLevels;
		m_bMsaa4xEnable = (m_nMsaa4xQualityLevels > 1) ? true : false;
	}
	else
	{
		::OutputDebugString(L"Device doesn't support MSAA4x");
	}


	// Create fence and set fence value to 0
	if (FAILED(m_pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pd3dFence)))) __debugbreak();

	m_hFenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

void CGameFramework::CreateSwapChain()
{
	RECT rcClient{};
	::GetClientRect(m_hWnd, &rcClient);
	m_nWndClientWidth = rcClient.right - rcClient.left;
	m_nWndClientHeight = rcClient.bottom - rcClient.top;

	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	{
		dxgiSwapChainDesc.BufferCount = m_nSwapChainBuffers;
		dxgiSwapChainDesc.BufferDesc.Width = m_nWndClientWidth;
		dxgiSwapChainDesc.BufferDesc.Height = m_nWndClientHeight;
		dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		dxgiSwapChainDesc.OutputWindow = m_hWnd;
		dxgiSwapChainDesc.SampleDesc.Count = (m_bMsaa4xEnable) ? 4 : 1;
		dxgiSwapChainDesc.SampleDesc.Quality = (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;
		dxgiSwapChainDesc.Windowed = TRUE;

		/// Set backbuffer resolution as fullscreen resolution.
		dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
	}

	IDXGISwapChain* pSwapChain;
	HRESULT hr = m_pdxgiFactory->CreateSwapChain(m_pd3dCommandQueue, &dxgiSwapChainDesc, &pSwapChain);
	pSwapChain->QueryInterface(IID_PPV_ARGS(&m_pdxgiSwapChain));

	m_nSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();

	hr = m_pdxgiFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER);

#ifndef _WITH_SWAPCHAIN_FULLSCREEN_STATE
	CreateRenderTargetViews();
#endif
}

void CGameFramework::CreateRTVAndDSVDescriptorHeaps()
{
	HRESULT hr{};

	// Create DescriptorHeap for RTV
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc{};
	::ZeroMemory(&d3dDescriptorHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
	{
		d3dDescriptorHeapDesc.NumDescriptors = m_nSwapChainBuffers;
		d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		d3dDescriptorHeapDesc.NodeMask = 0;
	}
	hr = m_pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, IID_PPV_ARGS(&m_pd3dRTVDescriptorHeap));
	if (FAILED(hr)) __debugbreak();

	// Set descriptor increment size
	m_nRTVDescriptorIncrementSize = m_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// Create DescriptorHeap for DSV
	{
		d3dDescriptorHeapDesc.NumDescriptors = 1;
		d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	}
	hr = m_pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, IID_PPV_ARGS(&m_pd3dDSVDescriptorHeap));
	if (FAILED(hr)) __debugbreak();

	// Set descriptor increment size
	m_nDSVDescriptorIncrementSize = m_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

}

void CGameFramework::CreateCommandQueueAndList()
{
	HRESULT hr{};

	// Create Command Queue
	D3D12_COMMAND_QUEUE_DESC d3dCommandQueueDesc{};
	::ZeroMemory(&d3dCommandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
	{
		d3dCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		d3dCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	}
	hr = m_pd3dDevice->CreateCommandQueue(&d3dCommandQueueDesc, IID_PPV_ARGS(&m_pd3dCommandQueue));
	if (FAILED(hr)) __debugbreak();

	// Create Command Allocator
	hr = m_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pd3dCommandAllocator));
	if (FAILED(hr)) __debugbreak();

	// Create Command List
	hr = m_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pd3dCommandAllocator, NULL, IID_PPV_ARGS(&m_pd3dCommandList));
	if (FAILED(hr)) __debugbreak();

	// Close Command List(default is opened)
	hr = m_pd3dCommandList->Close();
}

void CGameFramework::CreateRenderTargetViews()
{
	HRESULT hr{};

	D3D12_CPU_DESCRIPTOR_HANDLE d3dRTVCPUDescriptorHandle = m_pd3dRTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	for (UINT i = 0; i < m_nSwapChainBuffers; ++i)
	{
		hr = m_pdxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_ppd3dRenderTargetBuffers[i]));
		if (FAILED(hr)) __debugbreak();

		m_pd3dDevice->CreateRenderTargetView(m_ppd3dRenderTargetBuffers[i], NULL, d3dRTVCPUDescriptorHandle);
		d3dRTVCPUDescriptorHandle.ptr += m_nRTVDescriptorIncrementSize;
	}
}

void CGameFramework::CreateDepthStencilView()
{
	D3D12_RESOURCE_DESC d3dResourceDesc{};
	{
		d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		d3dResourceDesc.Alignment = 0;
		d3dResourceDesc.Width = m_nWndClientWidth;
		d3dResourceDesc.Height = m_nWndClientHeight;
		d3dResourceDesc.DepthOrArraySize = 1;
		d3dResourceDesc.MipLevels = 1;
		d3dResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		d3dResourceDesc.SampleDesc.Count = (m_bMsaa4xEnable) ? 4 : 1;
		d3dResourceDesc.SampleDesc.Quality = (m_bMsaa4xEnable) ? (m_nMsaa4xQualityLevels - 1) : 0;
		d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	}

	D3D12_HEAP_PROPERTIES d3dHeapProperties;
	::ZeroMemory(&d3dHeapProperties, sizeof(D3D12_HEAP_PROPERTIES));
	{
		d3dHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
		d3dHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		d3dHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		d3dHeapProperties.CreationNodeMask = 1;
		d3dHeapProperties.VisibleNodeMask = 1;
	}

	D3D12_CLEAR_VALUE d3dClearValue;
	d3dClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dClearValue.DepthStencil.Depth = 1.f;
	d3dClearValue.DepthStencil.Stencil = 0;

	// Create Depth Stencil Buffer
	m_pd3dDevice->CreateCommittedResource(
		&d3dHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&d3dResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&d3dClearValue,
		IID_PPV_ARGS(&m_pd3dDepthStencilBuffer)
	);

	// Create Depth Stencil Buffer View
	D3D12_CPU_DESCRIPTOR_HANDLE d3dDSVCPUDescriptorHandle = m_pd3dDSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, NULL, d3dDSVCPUDescriptorHandle);
}

void CGameFramework::BuildObjects()
{
	m_pd3dCommandList->Reset(m_pd3dCommandAllocator, NULL);

	// 따라하기8: Create Camera instance and set viewport, scissor rect, view/proj matrix
	m_pCamera = new CCamera();
	m_pCamera->SetViewport(0, 0, m_nWndClientWidth, m_nWndClientHeight, 0.f, 1.f);
	m_pCamera->SetScissorRect(0, 0, m_nWndClientWidth, m_nWndClientHeight);
	m_pCamera->GenerateProjectionMatrix(1.0f, 500.0f, float(m_nWndClientWidth) / float(m_nWndClientWidth), 90.0f);
	m_pCamera->GenerateViewMatrix(XMFLOAT3(0.f, 15.f, -25.0f), XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(0.f, 1.f, 0.f));

	// Create Scene object and contained GameObjects
	m_pScene = new CScene();
	m_pScene->BuildObjects(m_pd3dDevice, m_pd3dCommandList);

	/// Add command lists to command queue for create scene's GameObjects
	m_pd3dCommandList->Close();
	ID3D12CommandList* ppd3dCommandLists[] = { m_pd3dCommandList };
	m_pd3dCommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	WaitForGPUComplete();

	// Destroy upload buffers used for creating graphic resources
	if (m_pScene) m_pScene->ReleaseUploadBuffers();

	m_GameTimer.Reset();
}

void CGameFramework::ReleaseObjects()
{
	if (m_pScene) m_pScene->ReleaseObjects();
	if (m_pScene) delete m_pScene;
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID) {
	
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		break;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		break;

	case WM_MOUSEMOVE:
		break;

	default:
		break;
	}
}

void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID) {
		
	case WM_KEYUP:
		switch (wParam) {

		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;

		case VK_RETURN:
			break;

		case VK_F8:
			break;

			// if F9 pressed, Process change between fullscreen mode and windowed mode
		case VK_F9:
			ChangeSwapChainState();
			break;

		default:
			break;
		}
		break;
	default:
		break;
	}
}

LRESULT CALLBACK CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID) {

	case WM_SIZE:
	{
		m_nWndClientHeight = LOWORD(lParam);
		m_nWndClientHeight = HIWORD(lParam);
		break;
	}

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;

	default:
		break;
	}

	return 0;
}

void CGameFramework::ChangeSwapChainState()
{
	WaitForGPUComplete();

	BOOL bFullScreenState = FALSE;
	m_pdxgiSwapChain->GetFullscreenState(&bFullScreenState, NULL);
	m_pdxgiSwapChain->SetFullscreenState(!bFullScreenState, NULL);

	DXGI_MODE_DESC dxgiTargetParameters;
	{
		dxgiTargetParameters.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		dxgiTargetParameters.Width = m_nWndClientWidth;
		dxgiTargetParameters.Height = m_nWndClientHeight;
		dxgiTargetParameters.RefreshRate.Numerator = 60;
		dxgiTargetParameters.RefreshRate.Denominator= 1;
		dxgiTargetParameters.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		dxgiTargetParameters.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	}
	m_pdxgiSwapChain->ResizeTarget(&dxgiTargetParameters);

	for (int i = 0; i < m_nSwapChainBuffers; ++i) {
		if (m_ppd3dRenderTargetBuffers[i])
			m_ppd3dRenderTargetBuffers[i]->Release();
	}

	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	m_pdxgiSwapChain->GetDesc(&dxgiSwapChainDesc);
	m_pdxgiSwapChain->ResizeBuffers(m_nSwapChainBuffers, m_nWndClientWidth, m_nWndClientHeight, dxgiSwapChainDesc.BufferDesc.Format, dxgiSwapChainDesc.Flags);

	m_nSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();

	CreateRenderTargetViews();
}

void CGameFramework::MoveToNextFrame()
{
	m_nSwapChainBufferIndex = m_pdxgiSwapChain->GetCurrentBackBufferIndex();

	UINT64 nFenceValue = ++m_nFenceValues[m_nSwapChainBufferIndex];
	HRESULT hr = m_pd3dCommandQueue->Signal(m_pd3dFence, nFenceValue);
	if (m_pd3dFence->GetCompletedValue() < nFenceValue)
	{
		hr = m_pd3dFence->SetEventOnCompletion(nFenceValue, m_hFenceEvent);
		::WaitForSingleObject(m_hFenceEvent, INFINITE);
	}

	if (FAILED(hr)) __debugbreak();

}

void CGameFramework::ProcessInput()
{

}

void CGameFramework::AnimateObjects()
{
	if (m_pScene) 
		m_pScene->AnimateObjects(m_GameTimer.GetTimeElapsed());
}

void CGameFramework::FrameAdvance()
{
	m_GameTimer.Tick(0.0f);

	ProcessInput();
	AnimateObjects();

	RenderBegin();
	Render();
	RenderEnd();
	Present();
	MoveToNextFrame();


	m_GameTimer.GetFrameRate(m_pszFrameRate + 12, 37);
	::SetWindowText(m_hWnd, m_pszFrameRate);
}

void CGameFramework::RenderBegin()
{
	HRESULT hr;
	
	// Reset Command Allocator & Command List
	hr = m_pd3dCommandAllocator->Reset();
	hr = m_pd3dCommandList->Reset(m_pd3dCommandAllocator, NULL);
	if (FAILED(hr)) __debugbreak();

	// Change presented render target's resource state from D3D12_RESOURCE_STATE_PRESENT to D3D12_RESOURCE_STATE_RENDER_TARGET 
	D3D12_RESOURCE_BARRIER d3dResourceBarrier;
	::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
	{
		d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		d3dResourceBarrier.Transition.pResource = m_ppd3dRenderTargetBuffers[m_nSwapChainBufferIndex];
		d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		d3dResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	}
	m_pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier);

	// Set Viewport & Scissor Rect -> Removed in LabProject07
	//	m_pd3dCommandList->RSSetViewports(1, &m_d3dViewport);
	//	m_pd3dCommandList->RSSetScissorRects(1, &m_d3dScissorRect);

	// Compute CPU descriptor handle(address) of current render target
	D3D12_CPU_DESCRIPTOR_HANDLE d3dRTVCPUDescriptorHandle = m_pd3dRTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	d3dRTVCPUDescriptorHandle.ptr += (m_nSwapChainBufferIndex * m_nRTVDescriptorIncrementSize);

	// Compute CPU Descriptor Handle(address) of DSV
	D3D12_CPU_DESCRIPTOR_HANDLE d3dDSVDescriptorHandle = m_pd3dDSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	// Link RTV and DSV descriptor handle to Output Merger(OM)
	m_pd3dCommandList->OMSetRenderTargets(1, &d3dRTVCPUDescriptorHandle, TRUE, &d3dDSVDescriptorHandle);
		
	// Clear render target as desired color
	float pfClearColor[4] = { 0.f, 0.125f, 0.3f, 1.0f };
	m_pd3dCommandList->ClearRenderTargetView(d3dRTVCPUDescriptorHandle, pfClearColor, 0, NULL);

	// Clear DSV as desired value
	m_pd3dCommandList->ClearDepthStencilView(d3dDSVDescriptorHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.f, 0, 0, NULL);
}

void CGameFramework::Render()
{
	if (m_pScene) m_pScene->Render(m_pd3dCommandList, m_pCamera);
}

void CGameFramework::RenderEnd()
{
	HRESULT hr;

	// Change rendered render target's resource state from D3D12_RESOURCE_STATE_RENDER_TARGET to D3D12_RESOURCE_STATE_PRESENT
	D3D12_RESOURCE_BARRIER d3dResourceBarrier;
	::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
	{
		d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		d3dResourceBarrier.Transition.pResource = m_ppd3dRenderTargetBuffers[m_nSwapChainBufferIndex];
		d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		d3dResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	}
	m_pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier);

	hr = m_pd3dCommandList->Close();

	ID3D12CommandList* ppd3dCommandLists[] = { m_pd3dCommandList };
	m_pd3dCommandQueue->ExecuteCommandLists(1, ppd3dCommandLists);

	WaitForGPUComplete();
}

void CGameFramework::Present()
{
	// Present SwapChain
	// This will filp back buffer to front buffer and change render target index
	m_pdxgiSwapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING);

}

void CGameFramework::WaitForGPUComplete()
{
	// Increase CPU Fence value
	const UINT64 nFenceValue = ++m_nFenceValues[m_nSwapChainBufferIndex];
	HRESULT hResult = m_pd3dCommandQueue->Signal(m_pd3dFence, nFenceValue);
	if (m_pd3dFence->GetCompletedValue() < nFenceValue)
	{
		hResult = m_pd3dFence->SetEventOnCompletion(nFenceValue, m_hFenceEvent);
		::WaitForSingleObject(m_hFenceEvent, INFINITE);
	}
}


