#include "Shape.h"

HRESULT Shape::CreateBuffers(HRESULT hr, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext, D3D11_BUFFER_DESC& bd, std::vector<SimpleVertex> vertices, std::vector<UINT16> indices) {


	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &vertices[0];
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &_pVertexBuffer);
	if (FAILED(hr))
		return hr;



	//pImmediateContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);

	// Create index buffer

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * indices.size();        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = &indices[0];
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &_pIndexBuffer);
	if (FAILED(hr))
		return hr;

	_noOfIndices = indices.size();


	// Set index buffer
	//pImmediateContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);



}

//void Shape::Draw(ID3D11DeviceContext* pImmediateContext, ID3D11Buffer* pConstantBuffer) {
//
//	// Set vertex buffer
//	UINT stride = sizeof(SimpleVertex);
//	UINT offset = 0;
//	pImmediateContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
//	pImmediateContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
//
//	
//	ConstantBuffer cb;
//	cb.mWorld = XMMatrixTranspose(World);
//	cb.mView = XMMatrixTranspose(View);
//	cb.mProjection = XMMatrixTranspose(Projection);
//	cb.time = t;
//	cb.lightPos = DirectX::XMVectorSet(-2.0f, 2.0f, -2.0f, 0.0f);
//	cb.Eye = DirectX::XMVectorSet(3.0f, 3.0f, 0.0f, 0.0f);
//	pImmediateContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb, 0, 0);
//
//	//
//	// Renders a triangle
//	//
//
//	pImmediateContext->VSSetShader(shaders.getVS(), nullptr, 0);
//	pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
//	pImmediateContext->PSSetShader(shaders.getPS(), nullptr, 0);
//	pImmediateContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
//	pImmediateContext->DrawIndexed(shape.getNumberOfIndices(), 0, 0);
//
//
//	
//}

void Shape::Draw(ID3D11DeviceContext* pImmediateContext, ID3D11Buffer* pConstantBuffer, DirectX::XMMATRIX World, DirectX::XMMATRIX View, DirectX::XMMATRIX Projection, float t) const {

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
	pImmediateContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);


	ConstantBuffer cb;
	cb.mWorld = DirectX::XMMatrixTranspose(World);
	cb.mView = DirectX::XMMatrixTranspose(View);
	cb.mProjection = DirectX::XMMatrixTranspose(Projection);
	cb.time = t;
	cb.lightPos = DirectX::XMVectorSet(-2.0f, 2.0f, -2.0f, 0.0f);
	cb.Eye = DirectX::XMVectorSet(3.0f, 3.0f, 0.0f, 0.0f);
	pImmediateContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb, 0, 0);

	//
	// Renders a triangle
	//

	pImmediateContext->VSSetShader(_vertexShader, nullptr, 0);
	pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	pImmediateContext->PSSetShader(_pixelShader, nullptr, 0);
	pImmediateContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
	pImmediateContext->DrawIndexed(_noOfIndices, 0, 0);



}
