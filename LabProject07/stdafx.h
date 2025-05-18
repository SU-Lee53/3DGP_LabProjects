// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#pragma comment(lib, "winmm.lib")

// Direct3D related headers
#include <string>	
#include <wrl.h>
#include <shellapi.h>	

// D3D12
#include <d3d12.h>
#include <dxgi1_4.h>


// D3DCompiler
#include <d3dcompiler.h>

// DirectXMath
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

// dxgidebug
#include <dxgidebug.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

using namespace Microsoft::WRL;


// Import libraries
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

// constants
const static DWORD FRAME_BUFFER_WIDTH = 800;
const static DWORD FRAME_BUFFER_HEIGHT = 600;

// fullscreen mode
//#define _WITH_SWAPCHAIN_FULLSCREEN_STATE

// CRT debug
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif

// Utility functions

// Creates buffer resources by buffer's heap type and initialize if initial data is available
extern ID3D12Resource* CreateBufferResources(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, 
	void* pData, UINT nBytes, D3D12_HEAP_TYPE d3dHeapType = D3D12_HEAP_TYPE_UPLOAD, 
	D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, ID3D12Resource** ppd3dUploadBuffer = nullptr);



// XMMath Helper Funtions

namespace Vector3 {

	inline XMFLOAT3 XMVectorToFloat3(XMVECTOR& xmVector) {
		XMFLOAT3 xmf3Result;
		XMStoreFloat3(&xmf3Result, xmVector);
		return xmf3Result;
	}

	inline XMFLOAT3 ScalarProduct(const XMFLOAT3 xmf3Vector, float fScalar, BOOL bNormalize = TRUE) {
		XMFLOAT3 xmf3Result;
		if (bNormalize) {
			return XMVectorToFloat3(XMVector3Normalize(XMVectorScale(XMLoadFloat3(&xmf3Vector), fScalar)));
		}
		else {
			return XMVectorToFloat3(XMVectorScale(XMLoadFloat3(&xmf3Vector), fScalar));
		}
	}

	inline XMFLOAT3 Add(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2) {
		return XMVectorToFloat3(XMVectorAdd(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2)));
	}

	// FMADD
	inline XMFLOAT3 Add(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2, float fScalar) {
		return XMVectorToFloat3(XMVectorAdd(XMLoadFloat3(&xmf3Vector1), XMVectorScale(XMLoadFloat3(&xmf3Vector2), fScalar)));
	}

	inline XMFLOAT3 Subtract(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2) {
		return XMVectorToFloat3(XMVectorSubtract(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2)));
	}

	inline float DotProduct(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2) {
		return XMVectorGetX(XMVector3Dot(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2)));
	}

	inline XMFLOAT3 CrossProduct(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2, BOOL bNormalize = TRUE) {
		if (bNormalize) {
			return XMVectorToFloat3(XMVector3Normalize(XMVector3Cross(XMVector3Normalize(XMLoadFloat3(&xmf3Vector1)), XMVector3Normalize(XMLoadFloat3(&xmf3Vector2)))));
		}
		else {
			return XMVectorToFloat3(XMVector3Cross(XMVector3Normalize(XMLoadFloat3(&xmf3Vector1)), XMVector3Normalize(XMLoadFloat3(&xmf3Vector2))));
		}
	}

	inline XMFLOAT3 Normalize(const XMFLOAT3& xmf3Vector) {
		return XMVectorToFloat3(XMVector3Normalize(XMLoadFloat3(&xmf3Vector)));
	}

	inline float Length(const XMFLOAT3& xmf3Vector) {
		return XMVectorGetX(XMVector3Length(XMLoadFloat3(&xmf3Vector)));
	}

	inline float Distance(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2) {
		return XMVectorGetX(XMVector3Length(XMVectorSubtract(XMLoadFloat3(&xmf3Vector1), XMLoadFloat3(&xmf3Vector2))));
	}

	inline float Angle(XMVECTOR& xmvVector1, XMVECTOR& xmvVector2) {
		assert(XMVectorGetX(XMVector3Length(xmvVector1)) == 1.0f);
		assert(XMVectorGetX(XMVector3Length(xmvVector2)) == 1.0f);

		return XMVectorGetX(XMVectorACos(XMVector3AngleBetweenNormals(xmvVector1, xmvVector2)));
	}

	inline float Angle(const XMFLOAT3& xmf3Vector1, const XMFLOAT3& xmf3Vector2) {
		return Angle(XMVector3Normalize(XMLoadFloat3(&xmf3Vector1)), XMVector3Normalize(XMLoadFloat3(&xmf3Vector2)));
	}

	inline XMFLOAT3 TransformNormal(const XMFLOAT3& xmf3Vector, const XMMATRIX& xmf4x4Transform) {
		return XMVectorToFloat3(XMVector3TransformNormal(XMLoadFloat3(&xmf3Vector), xmf4x4Transform));
	}

	inline XMFLOAT3 TransformNormal(const XMFLOAT3& xmf3Vector, const XMFLOAT4X4& xmmtx4x4Matrix) {
		return TransformNormal(xmf3Vector, XMLoadFloat4x4(&xmmtx4x4Matrix));
	}

	inline XMFLOAT3 TransformCoord(const XMFLOAT3& xmf3Vector, const XMMATRIX& xmxm4x4Transform) {
		return XMVectorToFloat3(XMVector3TransformCoord(XMLoadFloat3(&xmf3Vector), xmxm4x4Transform));
	}

	inline XMFLOAT3 TransformCoord(const XMFLOAT3& xmf3Vector, const XMFLOAT4X4& xmmtx4x4Matrix) {
		return TransformCoord(xmf3Vector, XMLoadFloat4x4(&xmmtx4x4Matrix));
	}

}

namespace Vector4 {

	inline XMFLOAT4 XMVectorToFloat4(XMVECTOR& xmVector) {
		XMFLOAT4 xmf3Result;
		XMStoreFloat4(&xmf3Result, xmVector);
		return xmf3Result;
	}

	inline XMFLOAT4 Add(const XMFLOAT4& xmf4Vector1, const XMFLOAT4& xmf4Vector2) {
		XMFLOAT4 xmf4Result;
		XMStoreFloat4(&xmf4Result, XMVectorAdd(XMLoadFloat4(&xmf4Vector1), XMLoadFloat4(&xmf4Vector2)));
		return xmf4Result;
	}
}

namespace Matrix4x4 {

	inline XMFLOAT4X4 XMMatrixToFloat4x4(const XMMATRIX& m) {
		XMFLOAT4X4 xmf4x4Matrix;
		XMStoreFloat4x4(&xmf4x4Matrix, m);
		return xmf4x4Matrix;
	}

	inline XMFLOAT4X4 Identity() {
		return XMMatrixToFloat4x4(XMMatrixIdentity());
	}

	inline XMFLOAT4X4 Translate(float x, float y, float z) {
		return XMMatrixToFloat4x4(XMMatrixTranslation(x, y, z));
	}

	inline XMFLOAT4X4 Translate(const XMVECTOR& xmvVector) {
		return XMMatrixToFloat4x4(XMMatrixTranslationFromVector(xmvVector));
	}

	inline XMFLOAT4X4 Translate(const XMFLOAT3& xmf3Vector) {
		return Translate(XMLoadFloat3(&xmf3Vector));
	}

	inline XMFLOAT4X4 Multiply(const XMMATRIX& xmmtxMatrix1, const XMMATRIX& xmmtxMatrix2) {
		return XMMatrixToFloat4x4(XMMatrixMultiply(xmmtxMatrix1, xmmtxMatrix2));
	}

	inline XMFLOAT4X4 Multiply(const XMFLOAT4X4& xmf4x4Matrix1, const XMMATRIX& xmmtxMatrix2) {
		return Multiply(XMLoadFloat4x4(&xmf4x4Matrix1), xmmtxMatrix2);
	}

	inline XMFLOAT4X4 Multiply(const XMMATRIX& xmmtxMatrix1, const XMFLOAT4X4& xmf4x4Matrix2) {
		return Multiply(xmmtxMatrix1, XMLoadFloat4x4(&xmf4x4Matrix2));
	}

	inline XMFLOAT4X4 Multiply(const XMFLOAT4X4& xmf4x4Matrix1, const XMFLOAT4X4& xmf4x4Matrix2) {
		return Multiply(XMLoadFloat4x4(&xmf4x4Matrix1), XMLoadFloat4x4(&xmf4x4Matrix2));
	}

	inline XMFLOAT4X4 RotationYawPitchRoll(float fPitch, float fYaw, float fRoll) {
		return XMMatrixToFloat4x4(XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll)));
	}

	inline XMFLOAT4X4 RotationAxis(const XMFLOAT3& xmf3Axis, float fAngle) {
		return XMMatrixToFloat4x4(XMMatrixRotationAxis(XMLoadFloat3(&xmf3Axis), XMConvertToRadians(fAngle)));
	}

	inline XMFLOAT4X4 Inverse(const XMFLOAT4X4& xmf4x4Matrix) {
		return XMMatrixToFloat4x4(XMMatrixInverse(NULL, XMLoadFloat4x4(&xmf4x4Matrix)));
	}

	inline XMFLOAT4X4 Transpose(const XMFLOAT4X4& xmf4x4Matrix) {
		return XMMatrixToFloat4x4(XMMatrixTranspose(XMLoadFloat4x4(&xmf4x4Matrix)));
	}

	inline XMFLOAT4X4 PerspectiveFovLH(float fFOVAngleY, float fAspectRatio, float fNearZ, float fFarZ) {
		return XMMatrixToFloat4x4(XMMatrixPerspectiveFovLH(fFOVAngleY, fAspectRatio, fNearZ, fFarZ));
	}

	inline XMFLOAT4X4 LookAtLH(const XMFLOAT3& xmf3EyePosition, const XMFLOAT3 xmf3LookAtPosition, const XMFLOAT3 xmf3UpDirection) {
		return XMMatrixToFloat4x4(XMMatrixLookAtLH(XMLoadFloat3(&xmf3EyePosition), XMLoadFloat3(&xmf3LookAtPosition), XMLoadFloat3(&xmf3UpDirection)));
	}

	inline XMFLOAT4X4 LookToLH(const XMFLOAT3& xmf3EyePosition, const XMFLOAT3& xmf3LookTo, const XMFLOAT3 xmf3UpDirection) {
		return XMMatrixToFloat4x4(XMMatrixLookToLH(XMLoadFloat3(&xmf3EyePosition), XMLoadFloat3(&xmf3LookTo), XMLoadFloat3(&xmf3UpDirection)));
	}

}
