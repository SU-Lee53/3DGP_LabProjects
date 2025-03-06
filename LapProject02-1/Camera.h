#pragma once
#include "Mesh.h"

class CViewport
{
public:
	CViewport(int nLeft, int nTop, int nWidth, int nHeight)
		: m_nLeft(nLeft), m_nTop(nTop), m_nWidth(nWidth), m_nHeight(nHeight) 
	{
	}

	virtual ~CViewport() {}

	int m_nLeft;
	int m_nTop;
	int m_nWidth;
	int m_nHeight;
};

class CCamera
{
public:
	CCamera() {}
	virtual ~CCamera() { if (m_pViewport) delete m_pViewport; }

private:
	// Position of Camera
	float m_fxPosition = 0.0f;
	float m_fyPosition = 0.0f;
	float m_fzPosition = 0.0f;

	// Rotation of Camera
	float m_fxRotation = 0.0f;
	float m_fyRotation = 0.0f;
	float m_fzRotation = 0.0f;

	// Field of view
	float m_fFOVAngle = 90.f;
	float m_fProjectRectDistance = 1.0f;

	// Viewport
	CViewport* m_pViewport = NULL;

	// Aspect ratio of Viewport (width / height)
	float m_fAspectRatio = float(FRAMEBUFFER_WIDTH) / float(FRAMEBUFFER_HEIGHT);

public:
	// Compute View, Projection, Screen, Transform
	CPoint3D CameraTransform(CPoint3D& f3World);
	CPoint3D ProjectionTranform(CPoint3D& f3Camera);
	CPoint3D ScreenTransform(CPoint3D& f3Projection);

	void SetPosition(float x, float y, float z)
	{
		m_fxPosition = x;
		m_fyPosition = y;
		m_fzPosition = z;
	}

	void SetRotation(float x, float y, float z)
	{
		m_fxRotation = x;
		m_fyRotation = y;
		m_fzRotation = z;
	}

	// Set Viewport and FOV of Camera
	void SetViewport(int xStart, int yStart, int nWidth, int nHeight);
	void SetFOVAngle(float fFOVAngle);

	// Move and Rotate Camera
	void Move(float x, float y, float z);
	void Rotate(float fPitch, float fYaw, float fRoll);
};

