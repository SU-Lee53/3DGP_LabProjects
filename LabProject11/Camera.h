#pragma once

// Constants for camera mode
#define FIRST_PERSON_CAMERA		0x01
#define SPACESHIP_CAMERA		0x02
#define THIRD_PERSON_CAMERA		0x03

// Constant for aspect ratio
#define ASPECT_RATIO	(float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT))

class CPlayer;

struct VS_CB_CAMERA_INFO  {
	XMFLOAT4X4 m_xmf4x4View;
	XMFLOAT4X4 m_xmf4x4Projection;
};

class CCamera {
protected:
	// Camera's world position vector
	XMFLOAT3		m_xmf3Position = {};

	// Camera's local x-y-z axis
	XMFLOAT3		m_xmf3Right = {};
	XMFLOAT3		m_xmf3Up = {};
	XMFLOAT3		m_xmf3Look = {};

	// Angles representing how much rotated for each angles
	float			m_fPitch = 0.f;
	float			m_fRoll = 0.f;
	float			m_fYaw = 0.f;

	DWORD			m_nMode;				// Camera mode

	XMFLOAT3		m_xmf3LookAtWorld = {}; // LookAt-pos for 3rd person camera
	XMFLOAT3		m_xmf3Offset = {};		// Camera offset for 3rd person camera(mostly)
	float			m_fTimeLag = 0.f;		// Time Delay when camera rotated

	XMFLOAT4X4		m_xmf4x4View = {};
	XMFLOAT4X4		m_xmf4x4Projection = {};

	D3D12_VIEWPORT	m_d3dViewport = {};
	D3D12_RECT		m_d3dScissorRect = {};

	CPlayer*		m_pPlayer = nullptr;

public:
	CCamera();
	CCamera(CCamera* pCamera);
	virtual ~CCamera();

public:
	// Create & Update constant buffer for passing camera's info
	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	// Create camera transform matrix
	void GenerateViewMatrix();
	void GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3 xmf3Up);

	// To avoid floating point error, Regenerate view matrix
	void RegenerateViewMatrix();

	// Generate projection matrix
	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);

	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom);

	virtual void SetViewportsAndScissorRects(ID3D12GraphicsCommandList* pd3dCommandList);

	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	CPlayer* GetPlayer() { return m_pPlayer; }

	void SetMode(DWORD nMode) {m_nMode = nMode;}
	DWORD GetMode() {return m_nMode;}

	void SetPosition(XMFLOAT3 xmf3Position) {m_xmf3Position = xmf3Position;}
	XMFLOAT3& GetPosition() {return m_xmf3Position;}
	
	void SetLookAtPosition(XMFLOAT3 xmf3LookAtWorld) { m_xmf3LookAtWorld = xmf3LookAtWorld; }
	XMFLOAT3& SetLookAtPosition() { return m_xmf3LookAtWorld; }
	XMFLOAT3& GetRightVector() { return m_xmf3Right; }
	XMFLOAT3& GetUpVector() { return m_xmf3Up; }
	XMFLOAT3& GetLookVector() { return m_xmf3Look; }

	float& GetPitch() { return m_fPitch; }
	float& GetRoll() { return m_fRoll; }
	float& GetYaw() { return m_fYaw; }

	void SetOffset(XMFLOAT3 xmf3Offset) { m_xmf3Offset = xmf3Offset; }
	XMFLOAT3& GetOffset() { return m_xmf3Offset; }

	void SetTimeLag(float fTimeLag) { m_fTimeLag = fTimeLag; }
	float GetTimeLag() { return m_fTimeLag; }

	XMFLOAT4X4 GetViewMatrix() { return m_xmf4x4View; }
	XMFLOAT4X4 GetProjectionMatrix() { return m_xmf4x4Projection; }
	D3D12_VIEWPORT GetViewport() { return m_d3dViewport; }
	D3D12_RECT GetScissorRect() { return m_d3dScissorRect; }

	// Move Camera amount of xmf3Shift
	virtual void Move(const XMFLOAT3& xmf3Shift) {
		m_xmf3Position.x += xmf3Shift.x;
		m_xmf3Position.y += xmf3Shift.y;
		m_xmf3Position.z += xmf3Shift.z;
	}

	virtual void Rotate(float fPitch = 0.f, float fYaw = 0.f, float fRoll = 0.f) {}		// rotates ca,era
	virtual void Update(const XMFLOAT3& xmf3LookAt, float fTimeElapsed) {}					// Update camera's info
	virtual void SetLookAt(const XMFLOAT3& xmf3LookAt) {}										// Set LookAt in 3rd person mode



};

//-----------------
// CSpaceShipCamera
//-----------------

class CSpaceShipCamera : public CCamera {
public:
	CSpaceShipCamera(CCamera* pCamera);
	virtual ~CSpaceShipCamera() {}

	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
};

//-----------------
// CFirstPersonCamera
//-----------------

class CFirstPersonCamera : public CCamera {
public:
	CFirstPersonCamera(CCamera* pCamera);
	virtual ~CFirstPersonCamera() {}

	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
};

//-----------------
// CThirdPersonCamera
//-----------------

class CThirdPersonCamera : public CCamera {
public:
	CThirdPersonCamera(CCamera* pCamera);
	virtual ~CThirdPersonCamera() {}

	virtual void Update(XMFLOAT3& xmf3LookAt, float fTimeElapsed);
	virtual void SetLookAt(XMFLOAT3& xmf3LookAt);
};
