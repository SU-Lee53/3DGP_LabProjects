#include "stdafx.h"
#include "Camera.h"
#include "Player.h"

// -----------
//  CViewport
// -----------

void CViewport::SetViewport(int nLeft, int nTop, int nWidth, int nHeight)
{
	m_nLeft = nLeft;
	m_nTop = nTop;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}

// ---------
//  CCamera
// ---------

void CCamera::GenerateViewMatrix()
{
	XMVECTOR xmvLook = XMVector3Normalize(XMLoadFloat3(&m_xmf3Look));
	XMVECTOR xmvUp = XMVector3Normalize(XMLoadFloat3(&m_xmf3Up));
	XMVECTOR xmvRight = XMVector3Normalize(XMVector3Cross(xmvUp, xmvLook));
	xmvUp = XMVector3Normalize(XMVector3Cross(xmvLook, xmvRight));

	XMStoreFloat3(&m_xmf3Look, xmvLook);
	XMStoreFloat3(&m_xmf3Right, xmvRight);
	XMStoreFloat3(&m_xmf3Up, xmvUp);

	// View matrix is inverse of cam's world transform
	m_xmf4x4View._11 = m_xmf3Right.x;
	m_xmf4x4View._12 = m_xmf3Up.x;
	m_xmf4x4View._13 = m_xmf3Look.x;

	m_xmf4x4View._21 = m_xmf3Right.y;
	m_xmf4x4View._22 = m_xmf3Up.y;
	m_xmf4x4View._23 = m_xmf3Look.y;

	m_xmf4x4View._31 = m_xmf3Right.z;
	m_xmf4x4View._32 = m_xmf3Up.z;
	m_xmf4x4View._33 = m_xmf3Look.z;

	XMVECTOR xmvPosition = XMLoadFloat3(&m_xmf3Position);
	m_xmf4x4View._41 = -XMVectorGetX(XMVector3Dot(xmvPosition, xmvRight));
	m_xmf4x4View._42 = -XMVectorGetX(XMVector3Dot(xmvPosition, xmvUp));
	m_xmf4x4View._43 = -XMVectorGetX(XMVector3Dot(xmvPosition, xmvLook));

	XMStoreFloat4x4(&m_xmf4x4ViewProject, XMMatrixMultiply(XMLoadFloat4x4(&m_xmf4x4View), XMLoadFloat4x4(&m_xmf4x4Project)));
}

void CCamera::SetLookAt(XMFLOAT3& xmf3Position, XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up)
{
	m_xmf3Position = xmf3Position;
	XMStoreFloat4x4(&m_xmf4x4View, XMMatrixLookAtLH(XMLoadFloat3(&m_xmf3Position), XMLoadFloat3(&xmf3LookAt), XMLoadFloat3(&xmf3Up)));
	
	XMVECTORF32 xmf32vRight{ m_xmf4x4View._11, m_xmf4x4View._21, m_xmf4x4View._31, 0.f };
	XMVECTORF32 xmf32vUp{ m_xmf4x4View._12, m_xmf4x4View._22, m_xmf4x4View._32, 0.f };
	XMVECTORF32 xmf32vLook{ m_xmf4x4View._13, m_xmf4x4View._23, m_xmf4x4View._33, 0.f };

	XMStoreFloat3(&m_xmf3Right, XMVector3Normalize(xmf32vRight));
	XMStoreFloat3(&m_xmf3Up, XMVector3Normalize(xmf32vUp));
	XMStoreFloat3(&m_xmf3Look, XMVector3Normalize(xmf32vLook));
}

void CCamera::SetLookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up)
{
	// Creates view matrix from current camera position.
	SetLookAt(m_xmf3Position, xmf3LookAt, xmf3Up);
}

void CCamera::GeneratePerspectiveProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fFOVAngle)
{
	float fAspectRatio = (float(m_Viewport.m_nWidth) / float(m_Viewport.m_nHeight));
	XMStoreFloat4x4(&m_xmf4x4Project, XMMatrixPerspectiveFovLH(XMConvertToRadians(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance));
}

void CCamera::SetViewport(int nLeft, int nTop, int nWidth, int nHeight)
{
	m_Viewport.SetViewport(nLeft, nTop, nWidth, nHeight);
	m_fAspectRatio = float(m_Viewport.m_nWidth) / float(m_Viewport.m_nHeight);
}

void CCamera::SetFOVAngle(float fFOVAngle)
{
	m_fFOVAngle = fFOVAngle;
	m_fProjectRectDistance = float(1.f / tan(XMConvertToDegrees(fFOVAngle * 0.5f)));
}

void CCamera::Move(XMFLOAT3& xmf3Shift)
{
	XMStoreFloat3(&m_xmf3Position, XMVectorAdd(XMLoadFloat3(&m_xmf3Position), XMLoadFloat3(&xmf3Shift)));
}

void CCamera::Move(float x, float y, float z)
{
	XMFLOAT3 shift{ x,y,z };
	Move(shift);
}

void CCamera::Rotate(float fPitch, float fYaw, float fRoll)
{
	if (fPitch != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(fPitch));
		XMStoreFloat3(&m_xmf3Look, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Look), xmmtxRotate));
		XMStoreFloat3(&m_xmf3Up, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Up), xmmtxRotate));
	}
	if (fYaw != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(fYaw));
		XMStoreFloat3(&m_xmf3Look, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Look), xmmtxRotate));
		XMStoreFloat3(&m_xmf3Right, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Right), xmmtxRotate));
	}
	if (fRoll != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(fRoll));
		XMStoreFloat3(&m_xmf3Up, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Up), xmmtxRotate));
		XMStoreFloat3(&m_xmf3Right, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Right), xmmtxRotate));
	}
}

// Camera will follow player slowly.
// Delayed follow? idk how to say in easy word.
void CCamera::Update(CPlayer* pPlayer, XMFLOAT3& xmf3LookAt, float fTimeElapsed)
{
	XMMATRIX xmmtx4Rotate;
	xmmtx4Rotate.r[0] = XMVectorSet(pPlayer->m_xmf3Right.x, pPlayer->m_xmf3Right.y, pPlayer->m_xmf3Right.z, 0.0f);
	xmmtx4Rotate.r[1] = XMVectorSet(pPlayer->m_xmf3Up.x, pPlayer->m_xmf3Up.y, pPlayer->m_xmf3Up.z, 0.0f);
	xmmtx4Rotate.r[2] = XMVectorSet(pPlayer->m_xmf3Look.x, pPlayer->m_xmf3Look.y, pPlayer->m_xmf3Look.z, 0.0f);
	xmmtx4Rotate.r[3] = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);

	XMVECTOR xmvPosition = XMLoadFloat3(&m_xmf3Position);

	// Rotates camera offset vector
	XMVECTOR xmvOffset = XMVector3TransformCoord(XMLoadFloat3(&pPlayer->m_xmf3CameraOffset), xmmtx4Rotate);

	// Rotated camera's position will be (pPlayer's position + camera offset vector)
	XMVECTOR xmvNewPosition = XMVectorAdd(XMLoadFloat3(&pPlayer->m_xmf3Position), xmvOffset);

	// Vector represents distance and direction of current camera's position to rotatied cameras position.
	XMVECTOR xmvDirection = XMVectorSubtract(xmvNewPosition, xmvPosition);
	float fLength = XMVectorGetX(XMVector3Length(xmvDirection));
	xmvDirection = XMVector3Normalize(xmvDirection);

	// Camera Lag : effect that camera follows player with little delay.
	
	float fTimeLagScale = fTimeElapsed * 4.0f;	// modify this value for see diffrence.
	float fDistance = fLength * fTimeLagScale;
	if (fDistance > fLength) fDistance = fLength;
	if (fLength < 0.01f) fDistance = fLength;
	if (fDistance > 0)
	{
		// Camera moves without revolving.
		// if rotation angle value is small, rotation movement is almost same as linear movement.
		XMStoreFloat3(&m_xmf3Position, XMVectorAdd(xmvPosition, XMVectorScale(xmvDirection, fDistance)));
		SetLookAt(pPlayer->m_xmf3Position, pPlayer->m_xmf3Up);


	}
}
