#pragma once
#include "GameObject.h"
#include "Camera.h"

class CPlayer : public CGameObject
{
public:
	CPlayer() {}
	virtual ~CPlayer() { if (m_pCamera) delete m_pCamera; }

private:
	// Camera that included in player object
	CCamera* m_pCamera = NULL;

public:
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	void Move(float x, float y, float z);
	void Rotate(float x, float y, float z);

	void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }
	CCamera* GetCamera() { return m_pCamera; }
};

