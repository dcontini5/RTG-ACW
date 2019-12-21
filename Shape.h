#pragma once
#include <DirectXMath.h>
#include <vector>
#include <d3d11_1.h>


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
	float Time;
	DirectX::XMVECTOR LightPos;
	DirectX::XMVECTOR Eye;
};

class Shape {

public:

	Shape(ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader) : _vertexShader(vertexShader), _pixelShader(pixelShader) {}
	~Shape() = default;
	HRESULT Shape::CreateBuffers(HRESULT, ID3D11Device*, ID3D11DeviceContext*, D3D11_BUFFER_DESC&, std::vector<SimpleVertex>, std::vector<UINT16>);
	void Draw();
	void Shape::Draw(ID3D11DeviceContext* pImmediateContext, ID3D11Buffer* pConstantBuffer, DirectX::XMMATRIX World, DirectX::XMMATRIX View, DirectX::XMMATRIX Projection, float t) const;
	UINT16 getNumberOfIndices() { return _noOfIndices; };

private:

	//std::vector<SimpleVertex> _vertices;
	//std::vector<UINT16> _indices;
	UINT16 _noOfIndices;
	ID3D11Buffer* _pVertexBuffer = nullptr;
	ID3D11Buffer* _pIndexBuffer = nullptr;
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	DirectX::XMFLOAT3 _position;
	DirectX::XMFLOAT3 _scale;
	DirectX::XMFLOAT3 _rotation;

};