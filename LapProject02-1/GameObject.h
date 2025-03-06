#pragma once
#include "Mesh.h"

class CGameObject
{
public:
	CGameObject() {}
	~CGameObject();

private:
	// World position of GameObject
	float m_fxPosition = 0.0f;
	float m_fyPosition = 0.0f;
	float m_fzPosition = 0.0f;

	// x, y, z Rotation of GameObject(CCW about each axis)
	float m_fxRotation = 0.0f;
	float m_fyRotation = 0.0f;
	float m_fzRotation = 0.0f;

	// Rotation Speed of GameObject
	float m_fxRotationSpeed = 0.0f;
	float m_fyRotationSpeed = 0.0f;
	float m_fzRotationSpeed = 0.0f;

	// Shape of GameObject(Mesh / Model)
	CMesh* m_pMesh = NULL;

	// Color of GameObject(color of line)
	DWORD m_dwColor = RGB(255, 0, 0);

public:
	void SetMesh(CMesh* pMesh)
	{
		m_pMesh = pMesh;
		if (pMesh) pMesh->AddRef();
	}

	void SetColor(DWORD dwColor) { m_dwColor = dwColor; }

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
	
	void SetPosition(float x, float y, float z)
	{
		m_fxPosition = x;
		m_fyPosition = y;
		m_fzPosition = z;
	}

	void SetRotationSpeed(float x, float y, float z)
	{
		m_fxRotationSpeed = x;
		m_fyRotationSpeed = y;
		m_fzRotationSpeed = z;
	}

	// Move GameObject by x, y, z axis
	void Move(float x, float y, float z)
	{
		m_fxPosition += x;
		m_fyPosition += y;
		m_fzPosition += z;
	}

	// Rotate GameObject by x, y, z axis
	void Rotate(float x, float y, float z)
	{
		m_fxRotation += x;
		m_fyRotation += y;
		m_fzRotation += z;
	}

	// Transfrom one vertex of mesh using GameObject's world position and rotation
	CPoint3D WorldTransform(CPoint3D& f3Model);
	
	// Animate GameObject
	virtual void Animate(float fElapsedTime);
	
	// Render GameObject
	virtual void Render(HDC hDCFrameBuffer);



};

