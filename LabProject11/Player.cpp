#include "stdafx.h"
#include "Player.h"
#include "Shader.h"

CPlayer::CPlayer()
{
	m_pCamera = NULL;

	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fMaxVelocityXZ = 0.0f;
	m_fMaxVelocityY = 0.0f;
	m_fFriction = 0.0f;

	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;

	m_pPlayerUpdatedContext = NULL;
	m_pCameraUpdatedContext = NULL;
}

CPlayer::~CPlayer()
{
	ReleaseShaderVariables();
	if (m_pCamera) delete m_pCamera;
}

void CPlayer::Move(DWORD dwDirection, float fDistance, bool bVelocity)
{
	XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);

	if (dwDirection & DIR_FORWARD) {
		xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, fDistance);
	}
	
	if (dwDirection & DIR_BACKWARD) {
		xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, -fDistance);
	}
	
	if (dwDirection & DIR_RIGHT) {
		xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, fDistance);
	}
	
	if (dwDirection & DIR_LEFT) {
		xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, -fDistance);
	}
	
	if (dwDirection & DIR_UP) {
		xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, fDistance);
	}
	
	if (dwDirection & DIR_DOWN) {
		xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, -fDistance);
	}

	Move(xmf3Shift, bVelocity);
}

void CPlayer::Move(const XMFLOAT3& xmf3Shift, bool bVelocity)
{
	if (bVelocity) {
		// if bUpdatedVelocity == TRUE -> Modify velocity vector, NOT move player
		m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
	}
	else {
		// if bUpdatedVelocity == FALSE -> Move player
		m_xmf3Position = Vector3::Add(m_xmf3Position, xmf3Shift);
		if (m_pCamera) {
			m_pCamera->Move(xmf3Shift);
		}
	}
}

void CPlayer::Move(float fxOffset, float fyOffset, float fzOffset)
{
}

void CPlayer::Rotate(float x, float y, float z)
{
	DWORD nCameraMode = m_pCamera->GetMode();

	if ((nCameraMode == FIRST_PERSON_CAMERA) || (nCameraMode == THIRD_PERSON_CAMERA)) {
		// To prevent gimbal lock : Set limit of x-rotation to -89 ~ -89
		if (x != 0.0f) {
			m_fPitch += x;
			if (m_fPitch > +89.0f) {
				x -= (m_fPitch - 89.0f);
				m_fPitch = +89.0f;
			}
			if (m_fPitch < -89.0f) {
				x -= (m_fPitch + 89.0f);
				m_fPitch = -89.0f;
			}
		}

		if (y != 0.0f) {
			m_fYaw += y;
			if (m_fYaw > 360.f) {
				m_fYaw -= 360.f;
			}
			if (m_fYaw < 0.f) {
				m_fYaw += 360.f;
			}
		}
		
		// limits z-rotation to -20 ~ +20
		if (z != 0.0f) {
			m_fRoll += z;
			if (m_fRoll > +20.f) {
				z -= (m_fRoll - 20.f);
				m_fRoll = +20.f;
			}
			if (m_fRoll < -20.f) {
				z -= (m_fRoll + 20.f);
				m_fRoll = -20.f;
			}
		}
		
		m_pCamera->Rotate(x, y, z);

		// Rotates player
		// y-rotation only occure in Player's Up vector (stand upright)
		if (y != 0.0f) {
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(y));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
	}
	else if (nCameraMode == SPACESHIP_CAMERA) {
		// In SPACESHIP_CAMERA : There are no limit on rotation angle

		m_pCamera->Rotate(x, y, z);

		if (x != 0.f) {
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(x));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		}
		
		if (y != 0.f) {
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(y));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
		
		if (z != 0.f) {
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(z));
			m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
	}

	// After Rotate, local x-y-z axis may not be orthogonal
	// So, makes basis orthogonal and set to unit vector
	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::CrossProduct(m_xmf3Up, m_xmf3Look, true);
	m_xmf3Up = Vector3::CrossProduct(m_xmf3Look, m_xmf3Right, true);

}

void CPlayer::Update(float fTimeElapsed)
{
	// Add Veclocity vector and Gravity vector.
	// Multiplying fTimeElapsed means apply gravity proportional to time
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Gravity, fTimeElapsed, false));

	// Get Length-XZ
	// if length is greater then max speed of XZ-plane : modify x, z element of velocity vector
	float fLength = ::sqrtf(::powf(m_xmf3Velocity.x, 2) + ::powf(m_xmf3Velocity.z, 2));
	float fMaxVelocityXZ = m_fMaxVelocityXZ * fTimeElapsed;
	if (fLength > m_fMaxVelocityXZ) {
		m_xmf3Velocity.x *= (fMaxVelocityXZ / fLength);
		m_xmf3Velocity.z *= (fMaxVelocityXZ / fLength);
	}

	// Same as Length-Y
	float fMaxVelocityY = m_fMaxVelocityY * fTimeElapsed;
	fLength = ::sqrtf(::powf(m_xmf3Velocity.y, 2));
	if (fLength > m_fMaxVelocityY) {
		m_xmf3Velocity.y *= (fMaxVelocityY / fLength);
	}

	// Move Player
	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(m_xmf3Velocity, fTimeElapsed, false);
	Move(xmf3Velocity, false);

	// Do additional task : set player's position to valid position
	// ex) player's new position can be not valid position
	// ex2) Player needs collision test
	if (m_pPlayerUpdatedContext) {
		OnPlayerUpdateCallback(fTimeElapsed);
	}

	DWORD nCameraMode = m_pCamera->GetMode();
	if (nCameraMode == THIRD_PERSON_CAMERA) {
		m_pCamera->Update(m_xmf3Position, fTimeElapsed);
	}

	if (m_pCameraUpdatedContext) {
		OnCameraUpdateCallback(fTimeElapsed);
	}

	// if camera is THIRD_PERSON_CAMERA : Set camera look to new player's position
	if (nCameraMode == THIRD_PERSON_CAMERA) {
		m_pCamera->SetLookAt(m_xmf3Position);
	}

	m_pCamera->RegenerateViewMatrix();


	// If velocity needs to decelerate because of friction : update velocity vector
	fLength = Vector3::Length(m_xmf3Velocity);
	float fDeceleration = (m_fFriction * fTimeElapsed);
	if (fDeceleration > fLength) {
		fDeceleration = fLength;
	}
	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Velocity, -fDeceleration, true));

}


void CPlayer::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	CGameObject::CreateShaderVariables(pd3dDevice, pd3dCommandList);
	if (m_pCamera) {
		m_pCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);
	}
}

void CPlayer::ReleaseShaderVariables()
{
	CGameObject::ReleaseShaderVariables();

	if (m_pCamera) {
		m_pCamera->ReleaseShaderVariables();
	}
}

void CPlayer::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	CGameObject::UpdateShaderVariables(pd3dCommandList);
}

CCamera* CPlayer::OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCamraMode)
{
	CCamera* pNewCamera = NULL;
	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		pNewCamera = new CFirstPersonCamera(m_pCamera);
		break;
		
	case THIRD_PERSON_CAMERA:
		pNewCamera = new CThirdPersonCamera(m_pCamera);
		break;
		
	case SPACESHIP_CAMERA:
		pNewCamera = new CSpaceShipCamera(m_pCamera);
		break;
	}

	// If camera mode was SPACESHIP_CAMERA : makes camera stand upright
	// If camera mode is changed into THIRD or FIRST : camera position must be on land -> Right.y, Look.y = 0 and normalize
	if (nCurrentCamraMode == SPACESHIP_CAMERA) {
		m_xmf3Right = Vector3::Normalize(XMFLOAT3(m_xmf3Right.x, 0.f, m_xmf3Right.z));
		m_xmf3Up = Vector3::Normalize(XMFLOAT3(0.f, 1.f, 0.f));
		m_xmf3Look = Vector3::Normalize(XMFLOAT3(m_xmf3Look.x, 0.f, m_xmf3Look.z));

		m_fPitch = 0.f;
		m_fRoll = 0.f;

		// Get angle between look and world z-axis and set m_fYaw
		m_fYaw = Vector3::Angle(XMFLOAT3(0.f, 0.f, 1.f), m_xmf3Look);
		if (m_xmf3Look.x < 0.f) {
			m_fYaw = -m_fYaw;
		}
	}
	else if ((nNewCameraMode == SPACESHIP_CAMERA) && m_pCamera) {
		m_xmf3Right = m_pCamera->GetRightVector();
		m_xmf3Up = m_pCamera->GetUpVector();
		m_xmf3Look = m_pCamera->GetLookVector();
	}

	if (pNewCamera) {
		pNewCamera->SetMode(nNewCameraMode);
		pNewCamera->SetPlayer(this);
	}

	if (m_pCamera) {
		delete m_pCamera;
	}

	return pNewCamera;
}

void CPlayer::OnPrepareRender()
{
	m_xmf4x4World._11 = m_xmf3Right.x;		m_xmf4x4World._12 = m_xmf3Right.y;		m_xmf4x4World._13 = m_xmf3Right.z;
	m_xmf4x4World._21 = m_xmf3Up.x;			m_xmf4x4World._22 = m_xmf3Up.y;			m_xmf4x4World._23 = m_xmf3Up.z;
	m_xmf4x4World._31 = m_xmf3Look.x;		m_xmf4x4World._32 = m_xmf3Look.y;		m_xmf4x4World._33 = m_xmf3Look.z;
	m_xmf4x4World._41 = m_xmf3Position.x;	m_xmf4x4World._42 = m_xmf3Position.y;	m_xmf4x4World._43 = m_xmf3Position.z;
}

void CPlayer::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	DWORD nCameraMode = (pCamera) ? pCamera->GetMode() : 0x00;
	if (nCameraMode == THIRD_PERSON_CAMERA) {
		if (m_pShader) {
			m_pShader->Render(pd3dCommandList, pCamera);
		}
		CGameObject::Render(pd3dCommandList, pCamera);
	}
}

CAirplanePlayer::CAirplanePlayer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	CMesh* pAirplaneMesh = new CAirplaneMeshDiffused(pd3dDevice, pd3dCommandList, 20.f, 20.f, 4.f, XMFLOAT4(0.f, 0.5f, 0.f, 0.f));

	SetMesh(pAirplaneMesh);
	m_pCamera = ChangeCamera(SPACESHIP_CAMERA, 0.f);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	SetPosition(XMFLOAT3(0.f, 0.f, -50.f));
	m_pCamera->GenerateViewMatrix(m_xmf3Position, m_xmf3Look, m_xmf3Up);

	CPlayerShader* pShader = new CPlayerShader();
	pShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	SetShader(pShader);

}

CAirplanePlayer::~CAirplanePlayer()
{
}

CCamera* CAirplanePlayer::ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed)
{
	DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
	if (nCurrentCameraMode == nNewCameraMode) {
		return m_pCamera;
	}

	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		// Set player's characteristic as FIRST_PERSON_CAMERA mode. -> Don't apply gravity
		SetFriction(200.f);
		SetGravity(XMFLOAT3(0.f, 0.f, 0.f));
		SetMaxVelocityXZ(125.f);
		SetMaxVelocityY(400.f);
		m_pCamera = OnChangeCamera(FIRST_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.f);
		m_pCamera->SetOffset(XMFLOAT3(0.f, 20.f, 0.f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.f, ASPECT_RATIO, 60.f);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.f, 1.f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
		
	case SPACESHIP_CAMERA:
		// Set player's characteristic as SPACESHIP_CAMERA mode. -> Don't apply gravity
		SetFriction(125.f);
		SetGravity(XMFLOAT3(0.f, 0.f, 0.f));
		SetMaxVelocityXZ(400.f);
		SetMaxVelocityY(400.f);
		m_pCamera = OnChangeCamera(SPACESHIP_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.f);
		m_pCamera->SetOffset(XMFLOAT3(0.f, 0.f, 0.f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.f, ASPECT_RATIO, 60.f);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.f, 1.f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
		
	case THIRD_PERSON_CAMERA:
		// Set player's characteristic as THIRD_PERSON_CAMERA mode. -> Apply time lag and offset
		SetFriction(250.f);
		SetGravity(XMFLOAT3(0.f, 0.f, 0.f));
		SetMaxVelocityXZ(125.f);
		SetMaxVelocityY(400.f);
		m_pCamera = OnChangeCamera(THIRD_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.25f);	// Compare this value 0.25, 1.0, 0.0
		m_pCamera->SetOffset(XMFLOAT3(0.f, 20.f, -50.f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.f, ASPECT_RATIO, 60.f);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.f, 1.f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
	default:
		break;
	}
	
	m_pCamera->SetPosition(Vector3::Add(m_xmf3Position, m_pCamera->GetOffset()));
	Update(fTimeElapsed);

	return m_pCamera;
}

void CAirplanePlayer::OnPrepareRender()
{
	CPlayer::OnPrepareRender();

	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(90.0f), 0.f, 0.f);
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}
