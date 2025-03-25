#pragma once
#include "Player.h"
#include "Scene.h"
#include "Timer.h"

class CGameTimer;

class CGameFramework
{
public:
	CGameFramework() {}
	~CGameFramework() {}

private:
	// Wndow program's instance / main window handle 
	HINSTANCE m_hInstance	= NULL;
	HWND m_hWnd				= NULL;

	// Client rect of main window
	RECT m_rcClient = {};

	// Bitmap and DC of render target(in D3D word)
	HDC m_hDCFrameBuffer			= NULL;
	HBITMAP m_hBitmapFrameBuffer	= NULL;
	HBITMAP m_hBitmapSelect = NULL;

	// Instance of Player GameObject
	CPlayer* m_pPlayer = NULL;

	// Scene class of gather GameObjects
	CScene* m_pScene = NULL;

	// Object for managing framerate
	CGameTimer m_GameTimer = {};

	// Last mouse clicked position
	POINT m_ptOldCursorPos = {};

	// TCHAR string for print framerate
	_TCHAR m_pszFrameRate[50] = {};

public:
	// Create Framework (Called when main window created)
	void OnCreate(HINSTANCE hInstance, HWND hMainHWnd);

	// Destroy Framework (Called when main widnow destroyed)
	void OnDestroy();

	// Create frame buffer(back buffer or render target) and BitBlt to client 
	void BuildFrameBuffer();
	void ClearFrameBuffer(DWORD dwColor);
	void PresentFrameBuffer();

	// Create and Destroy Renderable mesh & GameObjects
	void BuildObjects();
	void ReleaseObjects();

	// Core of framework(Process user input, animation, rendering)
	void ProcessInput();
	void AnimateObjects();
	void FrameAdvance();

	// Processing Window Message
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};

