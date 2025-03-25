#pragma once
#include "GameObject.h"
#include "Camera.h"

class CScene
{
public:
	CScene() {}
	virtual ~CScene() {}

private:
	// Numbers of GameObjects and array of GameObjects
	int m_nObjects				= 0;
	CGameObject** m_ppObjects	= NULL;

public:
	// Create and Destroy GameObjects
	virtual void BuildObjects();
	virtual void ReleaseObjects();

	// Animate GameObjects
	virtual void Animate(float fElapsedTime);

	// Render GameObjects
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);
};

