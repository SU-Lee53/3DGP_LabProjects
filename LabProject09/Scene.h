#pragma once
#include "Timer.h"
#include "GameObject.h"

class CCamera;

class CScene {
public:
	CScene();
	~CScene();

public:
	// Process mouse and keyboard message in Scene
	BOOL OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	BOOL OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	void ReleaseObjects();

	bool ProcessInput();
	void AnimateObjects(float fTimeElapsed);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);

	void ReleaseUploadBuffers();

	// Create GraphicsRootSignature
	ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);
	ID3D12RootSignature* GetGraphicsRootSignature();

	// Removed in LabProject07
	//	CShader** m_ppShaders = nullptr;
	//	int m_nShaders = 0;


protected:
	// Scene is set of GameObjects. 
	// GameObject includes Shader
	// Added in LabProject07
	CGameObject** m_ppObjects = nullptr;
	int m_nObjects = 0;

	// Interface pointer to Root Signature
	ID3D12RootSignature* m_pd3dGraphicsRootSignature = nullptr;
};

