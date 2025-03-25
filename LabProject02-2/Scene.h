#pragma once
#include "GameObject.h"
#include "Camera.h"

class CScene
{
public:
	CScene(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	virtual ~CScene() {}

private:
	// Numbers of GameObjects and array of GameObjects
	// Changed to std::vector
	std::vector<CGameObject*> m_pGameObjects = {};

	CPlayer* m_pPlayer = NULL;

public:
	// Create and Destroy GameObjects
	virtual void BuildObjects();
	virtual void ReleaseObjects();

	// Animate GameObjects
	virtual void Animate(float fElapsedTime);

	// Render GameObjects
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);

	// Processing Window Message
	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {}
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {}
};

