#pragma once
#include <DirectXMath.h>
#include <vector>
#include <d3d11_1.h>


struct ShapeCoordinates {

	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Rot;
	DirectX::XMFLOAT3 Scal;


};

struct SimpleVertex {

	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 TexCoord;

};

struct ConstantBuffer
{
	DirectX::XMMATRIX World;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Projection;
	DirectX::XMVECTOR LightPos;
	DirectX::XMVECTOR Eye;
	float Time;
	
};

class Shape {

public:

	Shape(ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ShapeCoordinates& coords) : _vertexShader(vertexShader), _pixelShader(pixelShader), _coordinates(coords) {}
	~Shape() = default;
	HRESULT CreateBuffers(HRESULT&, ID3D11Device*, std::vector<SimpleVertex>, std::vector<UINT16>);
	void Draw();
	void Draw(ID3D11DeviceContext* pImmediateContext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection, float t) const;
	void Draw(ID3D11DeviceContext* pImmediateContext, DirectX::XMMATRIX view, DirectX::XMVECTOR eye, DirectX::XMMATRIX projection, float t) const;
	UINT16 GetNumberOfIndices() const { return _noOfIndices; }

private:

	UINT16 _noOfIndices;
	ID3D11Buffer* _pVertexBuffer = nullptr;
	ID3D11Buffer* _pIndexBuffer = nullptr;
	ID3D11VertexShader* _vertexShader = nullptr;
	ID3D11PixelShader* _pixelShader = nullptr;
	ID3D11Buffer* _pConstantBuffer = nullptr;
	ShapeCoordinates _coordinates;

};




