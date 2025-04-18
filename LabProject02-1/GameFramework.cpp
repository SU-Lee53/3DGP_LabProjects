#include "stdafx.h"
#include "GameFramework.h"

void CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainHWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hMainHWnd;

	// Create Frame Buffer
	BuildFrameBuffer();

	// Create Scene(Game world)
	BuildObjects();
}

void CGameFramework::OnDestroy()
{
	ReleaseObjects();

	if (m_hBitmapFrameBuffer) ::DeleteObject(m_hBitmapFrameBuffer);
	if (m_hDCFrameBuffer) ::DeleteDC(m_hDCFrameBuffer);
}

void CGameFramework::BuildFrameBuffer()
{
	::GetClientRect(m_hWnd, &m_rcClient);

	HDC hDC = ::GetDC(m_hWnd);
	m_hDCFrameBuffer = ::CreateCompatibleDC(hDC);
	m_hBitmapFrameBuffer = ::CreateCompatibleBitmap(hDC, m_rcClient.right - m_rcClient.left, m_rcClient.bottom - m_rcClient.top);
	::SelectObject(m_hDCFrameBuffer, m_hBitmapFrameBuffer);
	::ReleaseDC(m_hWnd, hDC);
	::SetBkMode(m_hDCFrameBuffer, TRANSPARENT);
}

void CGameFramework::ClearFrameBuffer(DWORD dwColor)
{
	HPEN hPen = ::CreatePen(PS_SOLID, 0, dwColor);
	HPEN hOldPen = (HPEN)::SelectObject(m_hDCFrameBuffer, hPen);
	HBRUSH hBrush = ::CreateSolidBrush(dwColor);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(m_hDCFrameBuffer, hBrush);
	::Rectangle(m_hDCFrameBuffer, m_rcClient.left, m_rcClient.top, m_rcClient.right, m_rcClient.bottom);
	::SelectObject(m_hDCFrameBuffer, hOldBrush);
	::SelectObject(m_hDCFrameBuffer, hOldPen);
	::DeleteObject(hPen);
	::DeleteObject(hBrush);
}

void CGameFramework::PresentFrameBuffer()
{
	HDC hDC = ::GetDC(m_hWnd);
	::BitBlt(hDC, m_rcClient.left, m_rcClient.top, m_rcClient.right, m_rcClient.bottom, m_hDCFrameBuffer, m_rcClient.left, m_rcClient.top, SRCCOPY);
	::ReleaseDC(m_hWnd, hDC);
}

void CGameFramework::BuildObjects()
{
	// Create camera and set Viewport & FOV
	CCamera* pCamera = new CCamera();
	pCamera->SetViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
	pCamera->SetFOVAngle(60.0f);

	// Create player object and set camera & position
	m_pPlayer = new CPlayer();
	m_pPlayer->SetCamera(pCamera);
	m_pPlayer->SetPosition(0.0f, 0.0f, -40.0f);

	// Create Scene instance and build objects;
	m_pScene = new CScene();
	m_pScene->BuildObjects();

}

void CGameFramework::ReleaseObjects()
{
	// Destroy GameObjects in scene & destroy + destroy player
	if (m_pScene)
	{
		m_pScene->ReleaseObjects();
		delete m_pScene;
	}

	if (m_pPlayer) delete m_pPlayer;
}

void CGameFramework::ProcessInput()
{
	static UCHAR pKeyBuffer[256];
	if (::GetKeyboardState(pKeyBuffer))
	{
		float cxKeyDelta = 0.0f;
		float cyKeyDelta = 0.0f;
		float czKeyDelta = 0.0f;

		if (pKeyBuffer[VK_UP] & 0xF0) czKeyDelta = +0.125f;
		if (pKeyBuffer[VK_DOWN] & 0xF0) czKeyDelta = -0.125f;

		if (pKeyBuffer[VK_LEFT] & 0xF0) cxKeyDelta = -0.125f;
		if (pKeyBuffer[VK_RIGHT] & 0xF0) cxKeyDelta = +0.125f;

		if (pKeyBuffer[VK_PRIOR] & 0xF0) cyKeyDelta = +0.125f;
		if (pKeyBuffer[VK_NEXT] & 0xF0) cyKeyDelta = -0.125f;

		m_pPlayer->Move(cxKeyDelta, cyKeyDelta, czKeyDelta);
	}
}

void CGameFramework::AnimateObjects()
{
	if (m_pScene) m_pScene->Animate(1.0f / 600.0f);
}

void CGameFramework::FrameAdvance()
{
	// Process user input
	ProcessInput();

	// Animate scene
	AnimateObjects();

	// Clear frame buffer
	ClearFrameBuffer(RGB(90, 103, 244));

	// Render scene
	CCamera* pCamera = m_pPlayer->GetCamera();
	if (m_pScene) m_pScene->Render(m_hDCFrameBuffer, pCamera);

	// BitBlt frame buffer to client rect
	PresentFrameBuffer();

}
