#pragma once
#include "Mesh.h"

class CViewport
{
public:
	CViewport() = default;
	CViewport(int nLeft, int nTop, int nWidth, int nHeight);
	virtual ~CViewport() {}

	void SetViewport(int nLeft, int nTop, int nWidth, int nHeight);

	int m_nLeft;
	int m_nTop;
	int m_nWidth;
	int m_nHeight;
};

class CPlayer;

class CCamera
{
public:
	CCamera() {}
	virtual ~CCamera() {}

private:
	// Position vector(world space)
	XMFLOAT3 m_xmf3Position = XMFLOAT3{ 0.f, 0.f, 0.f };

	// Local x(right) / y(up) / z(look) vectors
	XMFLOAT3 m_xmf3Right	= XMFLOAT3{ 1.f, 0.f, 0.f };
	XMFLOAT3 m_xmf3Up		= XMFLOAT3{ 0.f, 1.f, 0.f };
	XMFLOAT3 m_xmf3Look		= XMFLOAT3{ 0.f, 0.f, 1.f };

	// Field of view / distance to projection rect
	float m_fFOVAngle = 90.f;
	float m_fProjectRectDistance = 1.f;

	// Aspect ratio of viewport
	float m_fAspectRatio = float(FRAMEBUFFER_WIDTH) / float(FRAMEBUFFER_HEIGHT);

public:
	// View matrix
	XMFLOAT4X4 m_xmf4x4View = Matrix4x4::Identity();

	// Perspective projection matrix
	XMFLOAT4X4 m_xmf4x4Project = Matrix4x4::Identity();

	// view * proj
	XMFLOAT4X4 m_xmf4x4ViewProject = Matrix4x4::Identity();

	// Viewport
	CViewport m_Viewport = CViewport{};

public:
	void SetFOVAngle(float fFOVAngle);

	// Creates view transform matrix
	void GenerateViewMatrix();

	// Creates perspective projection matrix
	void GeneratePerspectiveProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fFOVAngle);
	void SetViewport(int nLeft, int nTop, int nWidth, int nHeight);

	// Set look direction if 3rd persoon camera
	// Normaliy, set to look player model
	void SetLookAt(const XMFLOAT3& xmf3LookAt, const XMFLOAT3& xmf3Up);
	void SetLookAt(const XMFLOAT3& xmf3Position, const XMFLOAT3& xmf3LookAt, const XMFLOAT3& xmf3Up);

	// Moves camera by xmf3Shift
	void Move(const XMFLOAT3& xmf3Shift);
	void Move(const float x, float y, float z);

	// Rotates camera
	void Rotate(float fPitch = 0.f, float fYaw = 0.f, float fRoll = 0.f);

	// virtual function which is updates camera's position / rotation.
	void Update(CPlayer* pPlayer, const XMFLOAT3& xmf3LookAt, float fTimeElapsed = 0.016f);
};

