#include "stdafx.h"
#include "Scene.h"
#include "GraphicsPipeline.h"

void CScene::BuildObjects()
{
	CCubeMesh* pCubeMesh = new CCubeMesh(4.0f, 4.0f, 4.0f);
	
	m_pGameObjects.resize(5);

	m_pGameObjects[0] = new CGameObject();
	m_pGameObjects[0]->SetMesh(pCubeMesh);
	m_pGameObjects[0]->SetColor(RGB(255, 0, 0));
	m_pGameObjects[0]->SetPosition(-13.5f, 0.0f, 14.0f);
	m_pGameObjects[0]->SetRotationAxis(XMFLOAT3{ 1.0f, 1.0f, 0.0f });
	m_pGameObjects[0]->SetRotationSpeed(90.0f);
	m_pGameObjects[0]->SetMovingDirection(XMFLOAT3{ 1.f, 0.f, 0.f });
	m_pGameObjects[0]->SetMovingSpeed(0.5f);
	
	m_pGameObjects[1] = new CGameObject();
	m_pGameObjects[1]->SetMesh(pCubeMesh);
	m_pGameObjects[1]->SetColor(RGB(0, 0, 255));
	m_pGameObjects[1]->SetPosition(13.5f, 0.0f, 14.0f);
	m_pGameObjects[1]->SetRotationAxis(XMFLOAT3{ 0.0f, 1.0f, 1.0f });
	m_pGameObjects[1]->SetRotationSpeed(180.0f);
	m_pGameObjects[1]->SetMovingDirection(XMFLOAT3{ -1.f, 0.f, 0.f });
	m_pGameObjects[1]->SetMovingSpeed(1.5f);
	
	m_pGameObjects[2] = new CGameObject();
	m_pGameObjects[2]->SetMesh(pCubeMesh);
	m_pGameObjects[2]->SetColor(RGB(0, 255, 0));
	m_pGameObjects[2]->SetPosition(0.f, 5.0f, 20.0f);
	m_pGameObjects[2]->SetRotationAxis(XMFLOAT3{ 1.0f, 0.0f, 1.0f });
	m_pGameObjects[2]->SetRotationSpeed(30.15f);
	m_pGameObjects[2]->SetMovingDirection(XMFLOAT3{ 1.f, -1.f, 0.f });
	m_pGameObjects[2]->SetMovingSpeed(0.0f);
	
	m_pGameObjects[3] = new CGameObject();
	m_pGameObjects[3]->SetMesh(pCubeMesh);
	m_pGameObjects[3]->SetColor(RGB(0, 255, 255));
	m_pGameObjects[3]->SetPosition(0.f, 0.0f, 40.0f);
	m_pGameObjects[3]->SetRotationAxis(XMFLOAT3{ 0.0f, 0.0f, 1.0f });
	m_pGameObjects[3]->SetRotationSpeed(40.6f);
	m_pGameObjects[3]->SetMovingDirection(XMFLOAT3{ 0.f, 0.f, 1.f });
	m_pGameObjects[3]->SetMovingSpeed(0.0f);
	
	m_pGameObjects[4] = new CGameObject();
	m_pGameObjects[4]->SetMesh(pCubeMesh);
	m_pGameObjects[4]->SetColor(RGB(128, 0, 255));
	m_pGameObjects[4]->SetPosition(10.f, 10.0f, 50.0f);
	m_pGameObjects[4]->SetRotationAxis(XMFLOAT3{ 0.0f, 1.0f, 1.0f });
	m_pGameObjects[4]->SetRotationSpeed(50.06f);
	m_pGameObjects[4]->SetMovingDirection(XMFLOAT3{ 0.f, 1.f, 1.f });
	m_pGameObjects[4]->SetMovingSpeed(0.0f);
}

void CScene::ReleaseObjects()
{
	std::ranges::for_each(m_pGameObjects, [](CGameObject* p) { delete p; });
	m_pGameObjects.clear();

}

void CScene::Animate(float fElapsedTime)
{
	std::ranges::for_each(m_pGameObjects, [&fElapsedTime](CGameObject* p) { p->Animate(fElapsedTime); });
}

void CScene::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CGraphicsPipeline::SetViewport(&pCamera->m_Viewport);
	CGraphicsPipeline::SetViewProjectTransform(&pCamera->m_xmf4x4ViewProject);

	std::ranges::for_each(m_pGameObjects, [&hDCFrameBuffer, pCamera](CGameObject* p) { p->Render(hDCFrameBuffer, pCamera); });
}
