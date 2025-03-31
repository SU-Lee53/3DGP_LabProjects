#pragma once
#include "Timer.h"

class CShader;

class CScene
{
public:
	CScene();
	~CScene();

public:
	BOOL OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	BOOL OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	void ReleaseObjects();

	bool ProcessInput();
	void AnimateObjects(float fTimeElapsed);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList);

	void ReleaseUploadBuffers();

	ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);
	ID3D12RootSignature* GetGraphicsRootSignature();

	CShader** m_ppShaders = nullptr;
	int m_nShaders = 0;

	// Interface pointer to Root Signature
	ID3D12RootSignature* m_pd3dGraphicsRootSignature = nullptr;

};

