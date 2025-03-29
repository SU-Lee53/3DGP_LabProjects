#pragma once
#include "Timer.h"

class CScene
{
public:
	CScene();
	~CScene();

public:
	BOOL OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	BOOL OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);
	void CreateGraphicsPipelineState(ID3D12Device* pd3dDevice);

	void BuildObjects(ID3D12Device* pd3dDevice);
	void ReleaseObjects();

	bool ProcessInput();
	void AnimateObjects(float fTimeElapsed);

	void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList);

	// Interface pointer to Root Signature
	ID3D12RootSignature* m_pd3dGraphicsRootSignature = nullptr;

	// Interface pointer to Pipeline state object
	ID3D12PipelineState* m_pd3dPipelineState = nullptr;

};

