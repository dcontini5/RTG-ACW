#pragma once
#include <DirectXMath.h>
#include <vector>
#include <d3d11_1.h>
#include "SettingLoader.h"

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

};




