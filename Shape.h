#pragma once
#include <DirectXMath.h>
#include <vector>
#include <d3d11_1.h>
#include "SettingLoader.h"
#include "DDSTextureLoader.h"

class Shape {

public:

	Shape(ID3D11VertexShader* const vertexShader, ID3D11PixelShader* const pixelShader,const ShapeCoordinates& coords,const Material& mat) :
		_noOfIndices(0),
		_vertexShader(
			vertexShader),
		_pixelShader(
			pixelShader),
		_coordinates(
			coords),
		_material(mat){
		
	}
	Shape(const Shape& s);
	virtual ~Shape();
	
	HRESULT CreateBuffers(HRESULT& hr, ID3D11Device* const pd3dDevice, const std::vector<SimpleVertex>& vertices, const std::vector<UINT16>& indices);
	HRESULT CreateRasterState(ID3D11Device* const pd3dDevice, const D3D11_RASTERIZER_DESC& rasterDesc) { return  pd3dDevice->CreateRasterizerState(&rasterDesc, &_rasterState); };
	HRESULT CreateTextureResource(ID3D11Device* const pd3dDevice, const wchar_t* const filename) { return DirectX::CreateDDSTextureFromFile(pd3dDevice, filename, nullptr, &_textureRV); };
	HRESULT CreateBumpResource(ID3D11Device* const pd3dDevice, const wchar_t* const filename) { return DirectX::CreateDDSTextureFromFile(pd3dDevice, filename, nullptr, &_bumpRV); };
	HRESULT CreateDepthStencil(ID3D11Device* const pd3dDevice, const D3D11_DEPTH_STENCIL_DESC& depthStencilDesc) { return pd3dDevice->CreateDepthStencilState(&depthStencilDesc, &_pDepthStencilState); };
	

	void Draw(ID3D11DeviceContext* const pImmediateContext, const Camera* const camera, const DirectX::XMMATRIX& Projection, const float t) const;
	void Draw(ID3D11DeviceContext* const pImmediateContext, Light* const lightManager,const  Camera* const camera, const DirectX::XMMATRIX& projection, const float& t) const;
	void DrawShadow(ID3D11DeviceContext* const pImmediateContext, Light* const lightManager,const Camera* const camera, const DirectX::XMMATRIX& projection, const float& t) const;
	UINT16 GetNumberOfIndices() const { return _noOfIndices; }
	const ShapeCoordinates& GetCoords() const { return _coordinates; }
	void SetPos(const DirectX::XMFLOAT3& newPos) { _coordinates.Pos = newPos; }
	virtual void Collide(State& state,const float& radius) = 0;
	Shape& operator= (const Shape& s);
	void StopDrawing() { _drawable = false; };
	void StartDrawing() { _drawable = true; };
	bool IsDrawable() const { return _drawable; };
	void SetMaterial(const Material& mat) { _material = mat; };
	void SetVS(ID3D11VertexShader* const vertexShader) { _vertexShader = vertexShader; };
	void SetPS(ID3D11PixelShader* const pixelShader) { _pixelShader = pixelShader; };
	
private:

	UINT16 _noOfIndices; 
	ID3D11Buffer* _pVertexBuffer = nullptr;
	ID3D11Buffer* _pIndexBuffer = nullptr;
	ID3D11VertexShader* _vertexShader = nullptr;
	ID3D11PixelShader* _pixelShader = nullptr;
	ID3D11Buffer* _pConstantBuffer = nullptr;
	ShapeCoordinates _coordinates = {};
	ID3D11RasterizerState* _rasterState = nullptr;
	ID3D11ShaderResourceView* _textureRV = nullptr;
	ID3D11ShaderResourceView* _bumpRV = nullptr;
	ID3D11DepthStencilState* _pDepthStencilState = nullptr;
	Material _material;
	bool _drawable = true;


};




