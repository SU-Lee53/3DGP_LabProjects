#include "stdafx.h"
#include "Scene.h"
#include <string>
#include <algorithm>

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
{
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	{
		d3dRootSignatureDesc.NumParameters = 0;
		d3dRootSignatureDesc.pParameters = NULL;
		d3dRootSignatureDesc.NumStaticSamplers = 0;
		d3dRootSignatureDesc.pStaticSamplers = NULL;
		d3dRootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	}

	ID3DBlob* pd3dSignatureBlob = NULL;
	ID3DBlob* pd3dErrorBlob = NULL;
	HRESULT hr = ::D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);

	if (FAILED(hr))
	{
		std::string error;
		error.resize(pd3dErrorBlob->GetBufferSize(), '/0');
		memcpy((void*)error.data(), pd3dErrorBlob->GetBufferPointer(), pd3dErrorBlob->GetBufferSize());
		OutputDebugStringA(error.c_str());
	}

	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&m_pd3dGraphicsRootSignature));

	
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

}

void CScene::CreateGraphicsPipelineState(ID3D12Device* pd3dDevice)
{
	ID3DBlob* pd3dVertexShaderBlob = NULL;
	ID3DBlob* pd3dPixelShaderBlob = NULL;

	UINT nCompilerFlags = 0;
#ifdef _DEBUG
	nCompilerFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	D3DCompileFromFile(L"Shaders.hlsl", NULL, NULL, "VSMain", "vs_5_1", nCompilerFlags, 0, &pd3dVertexShaderBlob, NULL);
	D3DCompileFromFile(L"Shaders.hlsl", NULL, NULL, "PSMain", "ps_5_1", nCompilerFlags, 0, &pd3dPixelShaderBlob, NULL);

	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	{
		d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
		d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
		d3dRasterizerDesc.FrontCounterClockwise = FALSE;
		d3dRasterizerDesc.DepthBias = 0;
		d3dRasterizerDesc.DepthBiasClamp = 0.0f;
		d3dRasterizerDesc.DepthClipEnable = TRUE;
		d3dRasterizerDesc.MultisampleEnable = FALSE;
		d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
		d3dRasterizerDesc.ForcedSampleCount = 0;
		d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	}

	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	{
		d3dBlendDesc.AlphaToCoverageEnable = FALSE;
		d3dBlendDesc.IndependentBlendEnable = FALSE;
		d3dBlendDesc.RenderTarget[0].BlendEnable = FALSE;
		d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
		d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
		d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;

		d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
		d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
		d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	{
		d3dPipelineStateDesc.pRootSignature = m_pd3dGraphicsRootSignature;
		d3dPipelineStateDesc.VS = D3D12_SHADER_BYTECODE{ pd3dVertexShaderBlob->GetBufferPointer(), pd3dVertexShaderBlob->GetBufferSize() };
		d3dPipelineStateDesc.PS = D3D12_SHADER_BYTECODE{ pd3dPixelShaderBlob->GetBufferPointer(), pd3dPixelShaderBlob->GetBufferSize() };
		d3dPipelineStateDesc.RasterizerState = d3dRasterizerDesc;
		d3dPipelineStateDesc.BlendState = d3dBlendDesc;
		d3dPipelineStateDesc.DepthStencilState.DepthEnable = FALSE;
		d3dPipelineStateDesc.DepthStencilState.StencilEnable = FALSE;
		d3dPipelineStateDesc.InputLayout.pInputElementDescs = NULL;
		d3dPipelineStateDesc.InputLayout.NumElements = NULL;
		d3dPipelineStateDesc.SampleMask = UINT_MAX;
		d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		d3dPipelineStateDesc.NumRenderTargets = 1;
		d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		d3dPipelineStateDesc.SampleDesc.Count = 1;
		d3dPipelineStateDesc.SampleDesc.Quality = 0;
	}
	pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, IID_PPV_ARGS(&m_pd3dPipelineState));

	if (pd3dVertexShaderBlob) pd3dVertexShaderBlob->Release();
	if (pd3dPixelShaderBlob) pd3dPixelShaderBlob->Release();


}

BOOL CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

BOOL CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

void CScene::BuildObjects(ID3D12Device* pd3dDevice)
{
	CreateGraphicsRootSignature(pd3dDevice);
	CreateGraphicsPipelineState(pd3dDevice);
}

void CScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();
	if (m_pd3dPipelineState) m_pd3dPipelineState->Release();
}

bool CScene::ProcessInput()
{
	return false;
}

void CScene::AnimateObjects(float fTimeElapsed) 
{
}

void CScene::PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
	// Set Graphics Root Signature
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	// Set Pipeline State
	pd3dCommandList->SetPipelineState(m_pd3dPipelineState);

	// Set primitive topology
	pd3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void CScene::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	PrepareRender(pd3dCommandList);
	pd3dCommandList->DrawInstanced(3, 1, 0, 0);
}
