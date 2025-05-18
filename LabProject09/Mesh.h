#pragma once

// Declare class for representing vertex
class CVertex {
protected:
	XMFLOAT3 m_xmf3Position;

public:
	CVertex() { m_xmf3Position = XMFLOAT3(0.f, 0.f, 0.f); }
	CVertex(XMFLOAT3 xmf3Position) { m_xmf3Position = xmf3Position; }
	~CVertex() { }
};

class CDiffusedVertex : public CVertex {
protected:
	// Color of vertex
	XMFLOAT4 m_xmf4Diffuse;

public:
	CDiffusedVertex() {
		m_xmf3Position = XMFLOAT3(0.f, 0.f, 0.f);
		m_xmf4Diffuse = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	}

	CDiffusedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse) {
		m_xmf3Position = XMFLOAT3(x, y, z);
		m_xmf4Diffuse = xmf4Diffuse;
	}

	CDiffusedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse) {
		m_xmf3Position = xmf3Position;
		m_xmf4Diffuse = xmf4Diffuse;
	}

	~CDiffusedVertex() { }
};

class CMesh {
public:
	CMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual ~CMesh();

private:
	int m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	void ReleaseUploadBuffers();

protected:
	ID3D12Resource* m_pd3dVertexBuffer = nullptr;
	ID3D12Resource* m_pd3dVertexUploadBuffer = nullptr;

	D3D12_VERTEX_BUFFER_VIEW m_d3dVertexBufferView;

	D3D12_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT m_nSlot = 0;
	UINT m_nVertices = 0;
	UINT m_nStride = 0;
	UINT m_nOffset = 0;

protected:
	ID3D12Resource* m_pd3dIndexBuffer = nullptr;
	ID3D12Resource* m_pd3dIndexUploadBuffer = nullptr;

	D3D12_INDEX_BUFFER_VIEW m_d3dIndexBufferView;

	UINT m_nIndices = 0;		// count of index
	UINT m_nStartIndex = 0;		// starting index for draw mesh using index buffer
	UINT m_nBaseVertex = 0;		// Base index for add index buffer's index


public:
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList);

};

class CTriangleMesh : public CMesh {
public:
	CTriangleMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual ~CTriangleMesh() { }
};

class CCubeMeshDiffused : public CMesh {
public:
	CCubeMeshDiffused(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
		float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeMeshDiffused();
};