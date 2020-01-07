#pragma once
#include <vector>
#include <d3d11.h>
#include <windows.h>
#include <d3dcompiler.h>
#include <iostream>
#include <fstream>
#include <strstream>
#include <string>
#include <DirectXMath.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Camera.h"


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

struct PointLight {
	
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT3 Pos;
	
};

struct SpotLight {

	DirectX::XMFLOAT4 Pos;
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT3 Direction;
	float Spot;
	
};

struct Material {

	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;

};

struct ConstantBuffer
{
	DirectX::XMMATRIX World;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Projection;
	DirectX::XMVECTOR Eye;
	PointLight PointLight;
	float Time;
	SpotLight SpotLights[4];
	Material Material;
	
	
};

struct ShapeGeometry {


	std::vector<SimpleVertex> vertices;
	std::vector<UINT16> indices;

	
};

struct OBB {

	DirectX::XMVECTOR AxisOrientation[3];
	DirectX::XMVECTOR Center;
	float BoxHalfwidth[3];
};



class SettingLoader
{
public:
	SettingLoader();
	~SettingLoader();
	HRESULT CreateVertexShaders(HRESULT hr, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext, std::string fileName);
	HRESULT CreatePixelShaders(HRESULT hr, ID3D11Device* pd3dDevice, std::string fileName);
	ID3D11PixelShader* GetPs(const int i) const { return _pixelShaderList[i]; };
	ID3D11VertexShader* GetVs(const int i) const { return _vertexShaderList[i]; };
	ID3D11InputLayout* GetVertexLayout() const { return _pVertexLayout; };
	void FileLoader(HRESULT, ID3D11Device*, ID3D11DeviceContext*);
	void ObjLoader(std::string);
	std::vector<SimpleVertex> GetVertices(int i) const { return _shapeGeometries[i].vertices; };
	std::vector<UINT16> GetIndices(int i) const { return _shapeGeometries[i].indices; };
	std::vector<CameraCoordinates> GetCameraCoords() const { return _cameraCoordinates; };
	std::vector<ShapeCoordinates> GetObjectsCoords() const { return _objectCoordinates; }
	void CreateParticleGeometry();

	

private:

	std::vector<ID3D11VertexShader*> _vertexShaderList;
	std::vector<ID3D11PixelShader*> _pixelShaderList;
	ID3D11InputLayout* _pVertexLayout;
	std::vector<ShapeCoordinates> _objectCoordinates;
	std::vector<CameraCoordinates> _cameraCoordinates;
	std::vector<ShapeGeometry> _shapeGeometries;

	
	static HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);



	//std::vector<WCHAR*> _vertexShaderFileList = { L"vertex_shader.hlsl" };
	//std::vector<WCHAR*> _pixelShaderFileList = {  L"pixel_shader.hlsl" };

};

