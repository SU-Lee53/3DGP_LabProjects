#pragma once
#include "Mesh.h"

class CGameObject
{
public:
	CGameObject() {}
	~CGameObject();

public:
	bool m_bActive = true;

	// Shape of GameObject(Mesh / Model)
	CMesh* m_pMesh = NULL;

	// World transform matrix of GameObject
	XMFLOAT4X4 m_xmf4x4World{ Matrix4x4::Identity() };

	// Color of GameObject(color of line)
	DWORD m_dwColor = RGB(255, 0, 0);

	// vector represents GameObject's moving direction
	XMFLOAT3 m_xmf3MovingDirection = XMFLOAT3{ 0.f, 0.f, 1.f };
	float m_fMovingSpeed = 0.f;
	float m_fMovingRange = 0.f;

	// vector represents GameObject's rotation axis
	XMFLOAT3 m_xmf3RotationAxis = XMFLOAT3{ 0.f, 1.f, 0.f };
	float m_fRotationSpeed = 0.f;

public:
	void SetMesh(CMesh* pMesh)
	{
		m_pMesh = pMesh;
		if (pMesh) pMesh->AddRef();
	}

	void SetColor(DWORD dwColor) { m_dwColor = dwColor; }

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3& xmf3Position);
	
	void SetMovingDirection(XMFLOAT3& xmf3MovingDirection);
	void SetMovingSpeed(float fSpeed) { m_fMovingSpeed = fSpeed; }
	void SetMovingRange(float fRange) { m_fMovingRange = fRange; }

	void SetRotationAxis(XMFLOAT3& xmf3RotationAxis);
	void SetRotationSpeed(float fSpeed) { m_fRotationSpeed = fSpeed; }

	void Move(XMFLOAT3& vDirection, float fSpeed);

	void Rotate(float fPitch, float fYaw, float fRoll);
	void Rotate(XMFLOAT3& xmf3Axis, float fAngle);

	virtual void OnUpdateTransfrom() {}

	// Animate GameObject
	virtual void Animate(float fElapsedTime);
	
	// Render GameObject
	virtual void Render(HDC hDCFrameBuffer);



};

