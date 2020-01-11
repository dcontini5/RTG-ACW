#include "Shape.h"



HRESULT Shape::CreateBuffers(HRESULT& hr, ID3D11Device* const pd3dDevice,const std::vector<SimpleVertex>& vertices,const std::vector<UINT16>& indices) {

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

	bd.ByteWidth = sizeof(UINT16) * indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	InitData.pSysMem = indices.data();
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &_pIndexBuffer);
	if (FAILED(hr))
		return hr;

	_noOfIndices = indices.size();


	//Create Constant Buffer

	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hr = pd3dDevice->CreateBuffer(&bd, nullptr, &_pConstantBuffer);
	if (FAILED(hr))	return hr;

	return S_OK;
}


void Shape::Draw(ID3D11DeviceContext* const pImmediateContext, Camera* const camera, const DirectX::XMMATRIX& Projection,const float t) const {

	// Set vertex buffer
	const UINT stride = sizeof(SimpleVertex);
	const UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
	pImmediateContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);



	auto world = DirectX::XMMatrixIdentity();

	world *= DirectX::XMMatrixScaling(_coordinates.Scal.x, _coordinates.Scal.y, _coordinates.Scal.z);
	world *= DirectX::XMMatrixRotationRollPitchYaw(_coordinates.Rot.x, _coordinates.Rot.y, _coordinates.Rot.z);
	world *= DirectX::XMMatrixTranslation(_coordinates.Pos.x, _coordinates.Pos.y, _coordinates.Pos.z);

	const auto view =camera->GetView();

	Material shapeMat;
	shapeMat.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	shapeMat.diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	//shapeMat.specular = { 0.5f, 0.5f, 0.5f, 2.0f };

	
	//toon mat

	shapeMat.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
	shapeMat.diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	shapeMat.specular = { 0.9f, 0.9f, 0.9f, 128.0f };
	
	PointLight light;

	light.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	//light.Pos = { 0.0f, 5.0f, 0.f };
	light.Pos = { 0.0f, 5.0f, 3.5f };
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
	

	ConstantBuffer cb;
	cb.World = DirectX::XMMatrixTranspose(world);
	cb.View = DirectX::XMMatrixTranspose(view);
	cb.Projection = DirectX::XMMatrixTranspose(Projection);
	cb.Eye = camera->GetEye();
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
	pImmediateContext->OMSetDepthStencilState(_pDepthStencilState, 0);
	pImmediateContext->RSSetState(_rasterState);
	pImmediateContext->VSSetShader(_vertexShader, nullptr, 0);
	pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
	pImmediateContext->PSSetShader(_pixelShader, nullptr, 0);
	pImmediateContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
	pImmediateContext->DrawIndexed(_noOfIndices, 0, 0);



}

void Shape::Draw(ID3D11DeviceContext* const pImmediateContext, Light* const lightManager, Camera* const camera, const DirectX::XMMATRIX& Projection,const float& t) const {

	// Set vertex buffer
	const UINT stride = sizeof(SimpleVertex);
	const UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
	pImmediateContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);


	auto world = DirectX::XMMatrixIdentity();

	world *= DirectX::XMMatrixScaling(_coordinates.Scal.x, _coordinates.Scal.y, _coordinates.Scal.z);
	world *= DirectX::XMMatrixRotationRollPitchYaw(_coordinates.Rot.x, _coordinates.Rot.y, _coordinates.Rot.z);
	world *= DirectX::XMMatrixTranslation(_coordinates.Pos.x, _coordinates.Pos.y, _coordinates.Pos.z);

	auto view =camera->GetView();

	
	
	
	Material shapeMat;
	shapeMat.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	shapeMat.diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	//shapeMat.specular = { 0.5f, 0.5f, 0.5f, 2.0f };


	//toon mat

	shapeMat.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
	shapeMat.diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	shapeMat.specular = { 0.9f, 0.9f, 0.9f, 128.0f };

	PointLight light;
	ZeroMemory(&light, sizeof(light));


	std::vector<SpotLight> sLights;
	ZeroMemory(&sLights, sizeof(sLights));

	lightManager->SetLight(light, sLights);

	
	ConstantBuffer cb;
	ZeroMemory(&cb, sizeof(cb));
	
	cb.World = DirectX::XMMatrixTranspose(world);
	cb.View = DirectX::XMMatrixTranspose(view);
	cb.Projection = DirectX::XMMatrixTranspose(Projection);
	cb.Eye = camera->GetEye();
	cb.PointLight = light;
	cb.Time = t;
	if(sLights.size() == 4) {
		
		cb.SpotLights[0] = sLights[0];
		cb.SpotLights[1] = sLights[1];
		cb.SpotLights[2] = sLights[2];
		cb.SpotLights[3] = sLights[3];
		
	}

	cb.Material = shapeMat;

	pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

	//
	// Renders shape
	//
	if (_textureRV) pImmediateContext->PSSetShaderResources(0, 1, &_textureRV);
	if (_bumpRV) pImmediateContext->PSSetShaderResources(1, 1, &_bumpRV);
	pImmediateContext->OMSetDepthStencilState(_pDepthStencilState, 0);
	pImmediateContext->RSSetState(_rasterState);
	pImmediateContext->VSSetShader(_vertexShader, nullptr, 0);
	pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
	pImmediateContext->PSSetShader(_pixelShader, nullptr, 0);
	pImmediateContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
	pImmediateContext->DrawIndexed(_noOfIndices, 0, 0);



	
}

void Shape::DrawShadow(ID3D11DeviceContext* const pImmediateContext, Light* const lightManager, Camera* const camera, const DirectX::XMMATRIX& Projection,const float& t) const {


	// Set vertex buffer
	const UINT stride = sizeof(SimpleVertex);
	const UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
	pImmediateContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);


	Material shadowMat;
	shadowMat.ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
	shadowMat.diffuse = { 0.0f, 0.0f, 0.0f, 0.5f };
	shadowMat.specular = { 0.0f, 0.0f, 0.0f, 1.0f };


	PointLight light;
	ZeroMemory(&light, sizeof(light));


	std::vector<SpotLight> sLights;
	ZeroMemory(&sLights, sizeof(sLights));

	lightManager->SetLight(light, sLights);


	if(sLights.size() != 4) {

		auto world = DirectX::XMMatrixIdentity();

		
		world *= DirectX::XMMatrixScaling(_coordinates.Scal.x, _coordinates.Scal.y, _coordinates.Scal.z);
		world *= DirectX::XMMatrixRotationRollPitchYaw(_coordinates.Rot.x, _coordinates.Rot.y, _coordinates.Rot.z);
		world *= DirectX::XMMatrixTranslation(_coordinates.Pos.x, _coordinates.Pos.y, _coordinates.Pos.z);
		
		const auto view =camera->GetView();

		const DirectX::XMVECTOR shadowPlane = DirectX::XMVectorSet(0.f, 1.f, 0.f, 5.f);
		const DirectX::XMVECTOR mainLight = DirectX::XMLoadFloat3(&light.Pos);
		const DirectX::XMVECTOR pos = DirectX::XMVectorSet(_coordinates.Pos.x, _coordinates.Pos.y, _coordinates.Pos.z, 0.f);
		const DirectX::XMVECTOR toMainLight = DirectX::XMVectorSubtract(mainLight, pos);
		const DirectX::XMMATRIX S = DirectX::XMMatrixShadow(shadowPlane, toMainLight);
		const DirectX::XMMATRIX shadowOffsetY = DirectX::XMMatrixTranslation(0.0f, 0.004f, 0.0f);

		world = world * S * shadowOffsetY;


		ConstantBuffer cb;
		ZeroMemory(&cb, sizeof(cb));
		cb.World = DirectX::XMMatrixTranspose(world);
		cb.View = DirectX::XMMatrixTranspose(view);
		cb.Projection = DirectX::XMMatrixTranspose(Projection);
		cb.Eye = camera->GetEye();
		cb.PointLight = light;
		cb.Time = t;
		cb.Material = shadowMat;

		pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

		//
		// Renders shape
		//

		pImmediateContext->VSSetShader(_vertexShader, nullptr, 0);
		pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
		pImmediateContext->PSSetShader(_pixelShader, nullptr, 0);
		pImmediateContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
		pImmediateContext->DrawIndexed(_noOfIndices, 0, 0);

		return;

		
	}


	

	for ( const auto& sLight : sLights) {
	
		auto world = DirectX::XMMatrixIdentity();
		
		world *= DirectX::XMMatrixScaling(_coordinates.Scal.x, _coordinates.Scal.y, _coordinates.Scal.z);
		world *= DirectX::XMMatrixRotationRollPitchYaw(_coordinates.Rot.x, _coordinates.Rot.y, _coordinates.Rot.z);
		world *= DirectX::XMMatrixTranslation(_coordinates.Pos.x, _coordinates.Pos.y, _coordinates.Pos.z);
		const auto view =camera->GetView();
	
		const DirectX::XMVECTOR shadowPlane = DirectX::XMVectorSet(0.f, 1.f, 0.f, 5.f);
		const DirectX::XMVECTOR mainLight = DirectX::XMLoadFloat4(&sLight.Pos);
		const DirectX::XMVECTOR pos = DirectX::XMVectorSet(_coordinates.Pos.x, _coordinates.Pos.y, _coordinates.Pos.z, 0.f);
		const DirectX::XMVECTOR toMainLight = DirectX::XMVectorSubtract(mainLight, pos);
		const DirectX::XMMATRIX S = DirectX::XMMatrixShadow(shadowPlane, toMainLight);
		const DirectX::XMMATRIX shadowOffsetY = DirectX::XMMatrixTranslation(0.0f, 0.004f, 0.0f);
	
		world = world * S * shadowOffsetY;
	
	
	
		ConstantBuffer cb;
		cb.World = DirectX::XMMatrixTranspose(world);
		cb.View = DirectX::XMMatrixTranspose(view);
		cb.Projection = DirectX::XMMatrixTranspose(Projection);
		cb.Eye = camera->GetEye();
		cb.PointLight = light;
		cb.Time = t;
		cb.SpotLights[0] = sLights[0];
		cb.SpotLights[1] = sLights[1];
		cb.SpotLights[2] = sLights[2];
		cb.SpotLights[3] = sLights[3];
		cb.Material = shadowMat;
		
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
	



	
}

Shape& Shape::operator=(const Shape& s) = default;

/*
Shape& Shape::operator=(const Shape& s) {

	_noOfIndices = s._noOfIndices;
	_pVertexBuffer = s._pVertexBuffer;
	_pIndexBuffer = s._pIndexBuffer;
	_vertexShader = s._vertexShader;
	_pixelShader = s._pixelShader;
	_pConstantBuffer = s._pConstantBuffer;
	_coordinates = s._coordinates;
	_rasterState = s._rasterState;
	_textureRV = s._textureRV;
	_bumpRV = s._bumpRV;
	_pDepthStencilState = s._pDepthStencilState;
	return *this;
	
}*/


Shape::Shape(const Shape& s) {

	_noOfIndices = s._noOfIndices;
	_pVertexBuffer = s._pVertexBuffer;
	_pIndexBuffer = s._pIndexBuffer;
	_vertexShader = s._vertexShader;
	_pixelShader = s._pixelShader;
	_pConstantBuffer = s._pConstantBuffer;
	_coordinates = s._coordinates;
	_rasterState = s._rasterState;
	_textureRV = s._textureRV;
	_bumpRV	= s._bumpRV;
	_pDepthStencilState = s._pDepthStencilState;

}

Shape::~Shape() = default;
