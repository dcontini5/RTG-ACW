#pragma once
#include <DirectXMath.h>
#include <vector>
#include <d3d11_1.h>
#include "SettingLoader.h"
#include "DDSTextureLoader.h"

class Shape {

public:

	Shape(ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ShapeCoordinates& coords) : _noOfIndices(0),
	                                                                                                    _vertexShader(
		                                                                                                    vertexShader),
	                                                                                                    _pixelShader(
		                                                                                                    pixelShader),
	                                                                                                    _coordinates(
		                                                                                                    coords) {
	}

	~Shape() = default;
	HRESULT CreateBuffers(HRESULT&, ID3D11Device*, std::vector<SimpleVertex>, std::vector<UINT16>);
	HRESULT CreateRasterState(ID3D11Device* pd3dDevice, const D3D11_RASTERIZER_DESC& rasterDesc) { return  pd3dDevice->CreateRasterizerState(&rasterDesc, &_rasterState); };
	HRESULT CreateTextureResource(ID3D11Device* pd3dDevice, const wchar_t* filename) { return DirectX::CreateDDSTextureFromFile(pd3dDevice, filename, nullptr, &_textureRV); };
	HRESULT CreateBumpResource(ID3D11Device* pd3dDevice, const wchar_t* filename) { return DirectX::CreateDDSTextureFromFile(pd3dDevice, filename, nullptr, &_bumpRV); };
	HRESULT CreateDepthStencil(ID3D11Device* pd3dDevice, const D3D11_DEPTH_STENCIL_DESC& depthStencilDesc) { return pd3dDevice->CreateDepthStencilState(&depthStencilDesc, &_pDepthStencilState); };
		

	void Draw();
	void Draw(ID3D11DeviceContext* pImmediateContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection, float t) const;
	void Draw(ID3D11DeviceContext* pImmediateContext, DirectX::XMMATRIX view, DirectX::XMVECTOR eye, DirectX::XMMATRIX projection, float t) const;
	void DrawShadow(ID3D11DeviceContext* pImmediateContext, DirectX::XMMATRIX view, DirectX::XMVECTOR eye, DirectX::XMMATRIX projection, float t) const;
	UINT16 GetNumberOfIndices() const { return _noOfIndices; }
	ShapeCoordinates GetCoords() const { return _coordinates; }
	void SetPos(const DirectX::XMFLOAT3 newPos) { _coordinates.Pos = newPos; }

private:

	UINT16 _noOfIndices;
	ID3D11Buffer* _pVertexBuffer = nullptr;
	ID3D11Buffer* _pIndexBuffer = nullptr;
	ID3D11VertexShader* _vertexShader = nullptr;
	ID3D11PixelShader* _pixelShader = nullptr;
	ID3D11Buffer* _pConstantBuffer = nullptr;
	ShapeCoordinates _coordinates;
	ID3D11RasterizerState* _rasterState = nullptr;
	ID3D11ShaderResourceView* _textureRV = nullptr;
	ID3D11ShaderResourceView* _bumpRV = nullptr;
	ID3D11DepthStencilState* _pDepthStencilState = nullptr;


};




