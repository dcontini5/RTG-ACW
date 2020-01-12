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
#include "Light.h"


struct State {

	DirectX::XMFLOAT3 Velocity;
	DirectX::XMFLOAT3 Acceleration = { 0.f, -9.8f, 0.f };
	DirectX::XMFLOAT3 Position;

	State(const DirectX::XMFLOAT3& velocity, const DirectX::XMFLOAT3& position): Velocity(velocity), Position(position) {}
	
};

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
	DirectX::XMFLOAT3 Tangent;
	DirectX::XMFLOAT3 Binormal;

};

enum struct Type {

	box,
	cube,
	sphere,
	
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

struct Effect {


	int VertexShader;
	int PixelShader;
	Material material;
	

};


class SettingLoader
{
public:
	SettingLoader();
	SettingLoader(const SettingLoader& sl);
	~SettingLoader();
	
	HRESULT CreateVertexShaders( HRESULT hr,  ID3D11Device* const pd3dDevice, ID3D11DeviceContext* const pImmediateContext, const std::string& fileName);
	HRESULT CreatePixelShaders(HRESULT hr, ID3D11Device* const pd3dDevice, const std::string& fileName);
	ID3D11PixelShader* GetPs(const int i) const { return _pixelShaderList[i]; };
	ID3D11VertexShader* GetVs(const int i) const { return _vertexShaderList[i]; };
	ID3D11InputLayout* GetVertexLayout() const { return _pVertexLayout; };
	void FileLoader(HRESULT, ID3D11Device* const, ID3D11DeviceContext* const);
	void ObjLoader(const std::string&);
	std::vector<SimpleVertex> GetVertices(const int i) const { return _shapeGeometries[i].vertices; };
	std::vector<UINT16> GetIndices(const int i) const { return _shapeGeometries[i].indices; };
	const std::vector<CameraCoordinates>& GetCameraCoords() const { return _cameraCoordinates; };
	const std::vector<ShapeCoordinates>& GetObjectsCoords() const { return _objectCoordinates; }
	void CreateParticleGeometry();
	const std::vector<SpotLight>& GetSpotCoords() const { return _spotLightsCoordinates; }
	const std::vector<PointLight>& GetPointCoords() const { return _pointLightsCoordinates; }
	void CreateEffects();
	Effect GetRandomEffect();
	const Effect& GetBoxEffect() const  { return _boxEffect; };
	const Effect& GetEnvMap() const { return _envMap; };
	const Effect& GetParticleEffect() const { return  _particleEffect; };
	SettingLoader& operator= (const SettingLoader& sl);
	void ResetAvaiableEffects();
	

private:

	std::vector<ID3D11VertexShader*> _vertexShaderList;
	std::vector<ID3D11PixelShader*> _pixelShaderList;
	ID3D11InputLayout* _pVertexLayout = nullptr;
	std::vector<ShapeCoordinates> _objectCoordinates;
	std::vector<CameraCoordinates> _cameraCoordinates;
	std::vector<ShapeGeometry> _shapeGeometries;
	std::vector<SpotLight> _spotLightsCoordinates;
	std::vector<PointLight> _pointLightsCoordinates;

	Effect _boxEffect{};
	Effect _envMap{};
	Effect _particleEffect{};
	std::vector<Effect> _shapeEffects;
	std::vector<int> _avaiableEffects = {0, 1, 2, 3, 4};
	
	static HRESULT CompileShaderFromFile(const WCHAR* const szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** const ppBlobOut);



};

