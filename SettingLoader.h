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
#include "Shape.h"
#include "Camera.h"

struct ShapeGeometry {


	std::vector<SimpleVertex> vertices;
	std::vector<UINT16> indices;

	
};

class SettingLoader
{
public:
	SettingLoader();
	~SettingLoader();
	HRESULT CreateVertexShaders(HRESULT hr, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext, std::string fileName);
	HRESULT CreatePixelShaders(HRESULT hr, ID3D11Device* pd3dDevice, std::string fileName);
	ID3D11PixelShader* GetPs() const { return _pixelShaderList[0]; };
	ID3D11VertexShader* GetVs() const { return _vertexShaderList[0]; };
	ID3D11InputLayout* GetVertexLayout() const { return _pVertexLayout; };
	void FileLoader(HRESULT, ID3D11Device*, ID3D11DeviceContext*);
	void ObjLoader(std::string);
	std::vector<SimpleVertex> GetVertices(int i) const { return _shapeGeometries[i].vertices; };
	std::vector<UINT16> GetIndices(int i) const { return _shapeGeometries[i].indices; };
	CameraCoordinates GetCameraCoords() const { return _cameraCoordinates[0]; };
	std::vector<ShapeCoordinates> GetObjectsCoords() const { return _objectCoordinates; }


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

