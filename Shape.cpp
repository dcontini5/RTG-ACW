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


void Shape::Draw(ID3D11DeviceContext* pImmediateContext, DirectX::XMMATRIX view, DirectX::XMVECTOR eye, DirectX::XMMATRIX Projection, float t) const {

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
	pImmediateContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);



	auto world = DirectX::XMMatrixIdentity();

	world *= DirectX::XMMatrixScaling(_coordinates.Scal.x, _coordinates.Scal.y, _coordinates.Scal.z);
	world *= DirectX::XMMatrixRotationRollPitchYaw(_coordinates.Rot.x, _coordinates.Rot.y, _coordinates.Rot.z);
	world *= DirectX::XMMatrixTranslation(_coordinates.Pos.x, _coordinates.Pos.y, _coordinates.Pos.z);

	Material shapeMat;
	shapeMat.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	shapeMat.diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	//shapeMat.specular = { 0.5f, 0.5f, 0.5f, 2.0f };

	
	//toon mat

	shapeMat.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
	shapeMat.diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	shapeMat.specular = { 0.9f, 0.9f, 0.9f, 128.0f };
	
	PointLight light;

	//light.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	//light.Pos = { 0.0f, 5.0f, 0.f };
	light.Pos = { 0.0f, 5.0f, 3.5f };
	light.Color = { 0.0f, 0.0f, 0.0f, 0.0f };
	//light.Pos = { 0.0f, 0.0f, 0.0f };

	SpotLight sLights[4];

	//sLights[0].Pos = { 5.0f, 5.0f, -5.0f, 1.0f };
	//sLights[0].Color = { 0.0f, 0.0f, 0.0f, 0.0f };
	//sLights[0].Direction = { -5.0f, -5.0f, 5.0f };
	//sLights[0].Spot = 20.f;
	//
	//sLights[1].Pos = { 5.0f, 5.0f, 5.0f, 1.0f };
	//sLights[1].Color = { 0.0f, 0.0f, 0.0f, 0.0f };
	//sLights[1].Direction = { -5.0f, -5.0f, -5.0f };
	//sLights[1].Spot = 20.f;
	//
	//sLights[2].Pos = { -5.0f, 5.0f, -5.0f, 1.0f };
	//sLights[2].Color = { 0.0f, 0.0f, 0.0f, 0.0f };
	//sLights[2].Direction = { 5.0f, -5.0f, 5.0f };
	//sLights[2].Spot = 20.f;
	//
	//sLights[3].Pos = { -5.0f, 5.0f, 5.0f, 1.0f };
	//sLights[3].Color = { 0.0f, 0.0f, 0.0f, 0.0f };
	//sLights[3].Direction = { 5.0f, -5.0f, -5.0f };
	//sLights[3].Spot = 20.f;

	sLights[0].Pos = { 5.0f, 5.0f, -5.0f, 1.0f };
	sLights[0].Color = { 0.5f, 0.5f, 0.5f, 0.5f };
	sLights[0].Direction = { -1.0f, -1.0f, 1.0f };
	sLights[0].Spot = 20.f;
	
	sLights[1].Pos = { 5.0f, 5.0f, 5.0f, 1.0f };
	sLights[1].Color = { 0.5f, 0.5f, 0.5f, 0.5f };
	sLights[1].Direction = { -1.0f, -1.0f, -1.0f };
	sLights[1].Spot = 20.f;
	
	sLights[2].Pos = { -5.0f, 5.0f, -5.0f, 1.0f };
	sLights[2].Color = { 0.5f, 0.5f, 0.5f, 0.5f };
	sLights[2].Direction = { 1.0f, -1.0f, 1.0f };
	sLights[2].Spot = 20.f;
	
	sLights[3].Pos = { -5.0f, 5.0f, 5.0f, 1.0f };
	sLights[3].Color = { 0.5f, 0.5f, 0.5f, 0.5f };
	sLights[3].Direction = { 1.0f, -1.0f, -1.0f };
	sLights[3].Spot = 20.f;

	ConstantBuffer cb;
	cb.World = DirectX::XMMatrixTranspose(world);
	cb.View = DirectX::XMMatrixTranspose(view);
	cb.Projection = DirectX::XMMatrixTranspose(Projection);
	cb.Eye = eye;
	cb.PointLight = light;
	cb.Time = t;
	cb.SpotLights[0] = sLights[0];
	cb.SpotLights[1] = sLights[1];
	cb.SpotLights[2] = sLights[2];
	cb.SpotLights[3] = sLights[3];
	cb.Material = shapeMat;

	pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

	//
	// Renders shape
	//
	if(_textureRV) pImmediateContext->PSSetShaderResources(0, 1, &_textureRV);
	if(_bumpRV) pImmediateContext->PSSetShaderResources(1, 1, &_bumpRV);
	pImmediateContext->RSSetState(_rasterState);
	pImmediateContext->VSSetShader(_vertexShader, nullptr, 0);
	pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
	pImmediateContext->PSSetShader(_pixelShader, nullptr, 0);
	pImmediateContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
	pImmediateContext->DrawIndexed(_noOfIndices, 0, 0);



}

void Shape::DrawShadow(ID3D11DeviceContext* pImmediateContext, DirectX::XMMATRIX view, DirectX::XMVECTOR eye,
	DirectX::XMMATRIX Projection, float t) const {


	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
	pImmediateContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);


	Material shadowMat;
	shadowMat.ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
	shadowMat.diffuse = { 0.0f, 0.0f, 0.0f, 0.5f };
	shadowMat.specular = { 0.0f, 0.0f, 0.0f, 16.0f };


	PointLight light;

	light.Color = { 0.0f, 0.0f, 0.0f, 0.0f };
	light.Pos = { 0.0f, 0.0f, 0.0f };
	//light.Color = { 0.0f, 0.0f, 0.0f, 0.0f };
	//light.Pos = { 0.0f, 0.0f, 0.0f };

	SpotLight sLights[4];

	sLights[0].Pos = { 5.0f, 5.0f, -5.0f, 1.0f };
	sLights[0].Color = { 0.0f, 0.0f, 0.0f, 0.0f };
	sLights[0].Direction = { -5.0f, -5.0f, 5.0f };
	sLights[0].Spot = 20.f;
	
	sLights[1].Pos = { 5.0f, 5.0f, 5.0f, 1.0f };
	sLights[1].Color = { 0.0f, 0.0f, 0.0f, 0.0f };
	sLights[1].Direction = { -5.0f, -5.0f, -5.0f };
	sLights[1].Spot = 20.f;
	
	sLights[2].Pos = { -5.0f, 5.0f, -5.0f, 1.0f };
	sLights[2].Color = { 0.0f, 0.0f, 0.0f, 0.0f };
	sLights[2].Direction = { 5.0f, -5.0f, 5.0f };
	sLights[2].Spot = 20.f;
	
	sLights[3].Pos = { -5.0f, 5.0f, 5.0f, 1.0f };
	sLights[3].Color = { 0.0f, 0.0f, 0.0f, 0.0f };
	sLights[3].Direction = { 5.0f, -5.0f, -5.0f };
	sLights[3].Spot = 20.f;

	//sLights[0].Pos = { 5.0f, 5.0f, -5.0f, 1.0f };
	//sLights[0].Color = { 0.5f, 0.5f, 0.5f, 1.0f };
	//sLights[0].Direction = { -1.0f, -1.0f, 1.0f };
	//sLights[0].Spot = 20.f;
	//
	//sLights[1].Pos = { 5.0f, 5.0f, 5.0f, 1.0f };
	//sLights[1].Color = { 0.5f, 0.5f, 0.5f, 1.0f };
	//sLights[1].Direction = { -1.0f, -1.0f, -1.0f };
	//sLights[1].Spot = 20.f;
	//
	//sLights[2].Pos = { -5.0f, 5.0f, -5.0f, 1.0f };
	//sLights[2].Color = { 0.5f, 0.5f, 0.5f, 1.0f };
	//sLights[2].Direction = { 1.0f, -1.0f, 1.0f };
	//sLights[2].Spot = 20.f;
	//
	//sLights[3].Pos = { -5.0f, 5.0f, 5.0f, 1.0f };
	//sLights[3].Color = { 0.5f, 0.5f, 0.5f, 1.0f };
	//sLights[3].Direction = { 1.0f, -1.0f, -1.0f };
	//sLights[3].Spot = 20.f;

	//for (auto& sLight : sLights) {
	//
	//	auto world = DirectX::XMMatrixIdentity();
	//
	//	world *= DirectX::XMMatrixScaling(_coordinates.Scal.x, _coordinates.Scal.y, _coordinates.Scal.z);
	//	world *= DirectX::XMMatrixRotationRollPitchYaw(_coordinates.Rot.x, _coordinates.Rot.y, _coordinates.Rot.z);
	//	world *= DirectX::XMMatrixTranslation(_coordinates.Pos.x, _coordinates.Pos.y, _coordinates.Pos.z);
	//
	//
	//	DirectX::XMVECTOR shadowPlane = DirectX::XMVectorSet(0.f, 1.f, 0.f, 5.f);
	//	//DirectX::XMVECTOR mainLight = DirectX::XMVectorSet(0.f, 5.f, 0.f, 0.f);
	//	DirectX::XMVECTOR mainLight = DirectX::XMLoadFloat4(&sLight.Pos);
	//	DirectX::XMVECTOR pos = DirectX::XMVectorSet(_coordinates.Pos.x, _coordinates.Pos.y, _coordinates.Pos.z, 0.f);
	//	DirectX::XMVECTOR toMainLight = DirectX::XMVectorSubtract(mainLight, pos);
	//	DirectX::XMMATRIX S = DirectX::XMMatrixShadow(shadowPlane, toMainLight);
	//	DirectX::XMMATRIX shadowOffsetY = DirectX::XMMatrixTranslation(0.0f, 0.004f, 0.0f);
	//
	//	world = world * S * shadowOffsetY;
	//
	//
	//
	//	ConstantBuffer cb;
	//	cb.World = DirectX::XMMatrixTranspose(world);
	//	cb.View = DirectX::XMMatrixTranspose(view);
	//	cb.Projection = DirectX::XMMatrixTranspose(Projection);
	//	cb.Eye = eye;
	//	cb.PointLight = light;
	//	cb.Time = t;
	//	cb.SpotLights[0] = sLights[0];
	//	cb.SpotLights[1] = sLights[1];
	//	cb.SpotLights[2] = sLights[2];
	//	cb.SpotLights[3] = sLights[3];
	//	cb.Material = shadowMat;
	//	
	//	pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	//
	//	//
	//	// Renders shape
	//	//
	//
	//	pImmediateContext->VSSetShader(_vertexShader, nullptr, 0);
	//	pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
	//	pImmediateContext->PSSetShader(_pixelShader, nullptr, 0);
	//	pImmediateContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
	//	pImmediateContext->DrawIndexed(_noOfIndices, 0, 0);
	//
	//}	
	



	
}




