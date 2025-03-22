#pragma once
#include "GameObject.h"
#include "Camera.h"

class CPlayer : public CGameObject
{
public:
	CPlayer() {}
	virtual ~CPlayer() { if (m_pCamera) delete m_pCamera; }

public:
	// Player's position, x/y/z axis vectors
	XMFLOAT3 m_xmf3Position = XMFLOAT3{ 0.f, 0.f, 0.f };
	XMFLOAT3 m_xmf3Right	= XMFLOAT3{ 1.f, 0.f, 0.f };
	XMFLOAT3 m_xmf3Up		= XMFLOAT3{ 0.f, 1.f, 0.f };
	XMFLOAT3 m_xmf3Look		= XMFLOAT3{ 0.f, 0.f, 1.f };

	// Offset of 3rd person camera
	XMFLOAT3 m_xmf3CameraOffset = XMFLOAT3{ 0.f, 0.f, 0.f };
	
	// Movement speed
	XMFLOAT3 m_xmf3Velocity = XMFLOAT3{ 0.f ,0.f, 0.f };

	// Friction force
	float m_fFriction = 125.0f;

	// Local rotation
	float m_fPitch	= 0.f;
	float m_fYaw	= 0.f;
	float fRoll		= 0.f;

	// Current setted camera
	CCamera* m_pCamera = nullptr;

public:
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	
	// Set player's look direction
	void LookAt(const XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up);

	// Move player
	void Move(DWORD dwDirection, float fDistance);
	void Move(const XMFLOAT3& xmf3Shift, bool bUpdateVelocity);
	void Move(float x, float y, float z);

	// Rotate player
	void Rotate(float fPitch, float fYaw, float fRoll);
	void SetCameraOffset(const XMFLOAT3& xmf3CameraOffset);

	// Updates player's position and rotation info by time
	void Update(float fTimeElapsed = 0.016f);

	// Creates world transform matrix from player's position and rotation axis
	virtual void OnUpdateTransform();
	virtual void Animate(float fElapsedTime);

	void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }
	CCamera* GetCamera() { return m_pCamera; }

};

class CAirplanePlayer : public CPlayer
{
public:
	CAirplanePlayer() {}
	virtual ~CAirplanePlayer() {}

	virtual void OnUpdateTransform();
};