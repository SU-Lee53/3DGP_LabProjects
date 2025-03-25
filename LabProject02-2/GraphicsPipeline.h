#pragma once
#include "GameObject.h"
#include "Camera.h"

class CGraphicsPipeline
{
private:
	// Current gameobject's world transform matrix
	static XMFLOAT4X4* m_pxmf4x4World;

	// Current camera object's transform matrix(view * proj)
	static XMFLOAT4X4* m_pxmf4x4ViewProject;
	static CViewport* m_pViewport;

public:
	static void SetWorldTransform(XMFLOAT4X4* pxmf4x4World)
	{
		m_pxmf4x4World = pxmf4x4World;
	}

	static void SetViewProjectTransform(XMFLOAT4X4* pxmf4x4ViewProject)
	{
		m_pxmf4x4ViewProject = pxmf4x4ViewProject;
	}

	static void SetViewport(CViewport* pViewport)
	{
		m_pViewport = pViewport;
	}

	static XMFLOAT3 ScreenTransform(XMFLOAT3& xmf3Project);
	static XMFLOAT3 Project(XMFLOAT3& xmf3Model);

};

