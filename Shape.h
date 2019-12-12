#pragma once
#include <DirectXMath.h>
#include <vector>
#include <d3d11_1.h>



struct SimpleVertex	{
	
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 TexCoord;
	
};

class Shape {
	
public:
	
	Shape() = default;
	~Shape() = default;
	HRESULT Shape::CreateBuffers(HRESULT hr, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext, std::vector<SimpleVertex> vertices, std::vector<UINT16> indices);
	void Draw();

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
	
};
