#include "SettingLoader.h"


SettingLoader::SettingLoader()
{
}


SettingLoader::~SettingLoader()
{
}

//HRESULT SettingLoader::CreateVertexShaders(HRESULT hr, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext) {
//
//	ID3DBlob* pVSBlob = nullptr;
//
//
//	hr = CompileShaderFromFile(L"VSTexture.hlsl", "VS", "vs_4_0", &pVSBlob);
//	if (FAILED(hr))
//	{
//		MessageBox(nullptr,
//			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
//		return hr;
//	}
//
//	ID3D11VertexShader* v = nullptr;
//
//	// Create the vertex shader
//	hr = pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &v);
//	if (FAILED(hr))
//	{
//		pVSBlob->Release();
//		return hr;
//	}
//
//	_vertexShaderList.push_back(v);
//
//	D3D11_INPUT_ELEMENT_DESC layout[] =
//	{
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	};
//	UINT numElements = ARRAYSIZE(layout);
//
//	// Create the input layout
//	hr = pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
//		pVSBlob->GetBufferSize(), &_pVertexLayout);
//	pVSBlob->Release();
//	if (FAILED(hr))
//		return hr;
//
//	// Set the input layout
//	pImmediateContext->IASetInputLayout(_pVertexLayout);
//
//
//	return S_OK;
//
//}
//
//HRESULT SettingLoader::CreatePixelShaders(HRESULT hr, ID3D11Device * pd3dDevice) {
//
//	ID3DBlob* pPSBlob = nullptr;
//	hr = CompileShaderFromFile(L"PSTexture.hlsl", "PS", "ps_4_0", &pPSBlob);
//	if (FAILED(hr))
//	{
//		MessageBox(nullptr,
//			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
//		return hr;
//	}
//
//	ID3D11PixelShader* p = nullptr;
//
//	// Create the pixel shader
//	hr = pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &p);
//	pPSBlob->Release();
//	if (FAILED(hr))
//		return hr;
//
//	_pixelShaderList.push_back(p);
//
//	return S_OK;
//
//}
//
//
//HRESULT SettingLoader::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
//{
//	HRESULT hr = S_OK;
//
//	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
//#ifdef _DEBUG
//	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
//	// Setting this flag improves the shader debugging experience, but still allows 
//	// the shaders to be optimized and to run exactly the way they will run in 
//	// the release configuration of this program.
//	dwShaderFlags |= D3DCOMPILE_DEBUG;
//
//	// Disable optimizations to further improve shader debugging
//	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
//#endif
//
//	ID3DBlob* pErrorBlob = nullptr;
//	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
//		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
//	if (FAILED(hr))
//	{
//		if (pErrorBlob)
//		{
//			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
//			pErrorBlob->Release();
//		}
//		return hr;
//	}
//	if (pErrorBlob) pErrorBlob->Release();
//
//	return S_OK;
//}



