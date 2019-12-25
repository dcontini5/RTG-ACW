#include "Shape.h"

HRESULT Shape::CreateBuffers(HRESULT& hr, ID3D11Device* pd3dDevice, std::vector<SimpleVertex> vertices, std::vector<UINT16> indices) {

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices.data();
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &_pVertexBuffer);
	if (FAILED(hr))
		return hr;

	// Create index buffer

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT16) * indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices.data();
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &_pIndexBuffer);
	if (FAILED(hr))
		return hr;

	_noOfIndices = indices.size();


	//Create Constant Buffer
	
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = pd3dDevice->CreateBuffer(&bd, nullptr, &_pConstantBuffer);
	if (FAILED(hr))	return hr;

	return S_OK;
}


void Shape::Draw(ID3D11DeviceContext* pImmediateContext, DirectX::XMMATRIX view, DirectX::XMMATRIX Projection, float t) const {

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
	pImmediateContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);


	ConstantBuffer cb;
	auto world = DirectX::XMMatrixIdentity();

	world *= DirectX::XMMatrixScaling(_coordinates.Scal.x, _coordinates.Scal.y, _coordinates.Scal.z);
	//world *= DirectX::XMMatrixRotationRollPitchYaw(_coordinates.Rot.x, _coordinates.Rot.y, _coordinates.Rot.z);
	world *= DirectX::XMMatrixTranslation(_coordinates.Pos.x, _coordinates.Pos.y, _coordinates.Pos.z);
	
	
	cb.World = DirectX::XMMatrixTranspose(world);
	cb.View = DirectX::XMMatrixTranspose(view);
	cb.Projection = DirectX::XMMatrixTranspose(Projection);
	cb.Time = t;
	//cb.LightPos = DirectX::XMVectorSet(0.0f, 2.49f, 2.5f, 0.0f);
	cb.LightPos = DirectX::XMVectorSet(0.0f, 4.5f, 0.0f, 0.0f);
	
	cb.Eye = DirectX::XMVectorSet(0.0f, 4.0f, 10.0f, 0.0f);
	pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

	//
	// Renders shape
	//

	pImmediateContext->VSSetShader(_vertexShader, nullptr, 0);
	pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
	pImmediateContext->PSSetShader(_pixelShader, nullptr, 0);
	pImmediateContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
	pImmediateContext->DrawIndexed(_noOfIndices, 0, 0);



}




