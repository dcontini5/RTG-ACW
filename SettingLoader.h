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

struct ShapeCoordinates {

	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Rot;
	DirectX::XMFLOAT3 Scal;
	

};

struct CameraCoordinates {

	DirectX::XMFLOAT3 Eye;
	DirectX::XMFLOAT3 At;
	DirectX::XMFLOAT3 Up;
	
};


class SettingLoader
{
public:
	SettingLoader();
	~SettingLoader();
	HRESULT CreateVertexShaders(HRESULT hr, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext, std::vector<std::string> fileName);
	HRESULT CreatePixelShaders(HRESULT hr, ID3D11Device* pd3dDevice, std::string fileName);
	ID3D11PixelShader* GetPs() const { return _pixelShaderList[0]; };
	ID3D11VertexShader* GetVs() const { return _vertexShaderList[0]; };
	ID3D11InputLayout* GetvertexLayout() const { return _pVertexLayout; };
	void FileLoader();
	void ObjLoader(std::string);

	
private:

	std::vector<ID3D11VertexShader*> _vertexShaderList;
	std::vector<ID3D11PixelShader*> _pixelShaderList;
	ID3D11InputLayout* _pVertexLayout;
	std::vector<ShapeCoordinates> _objectCoordinates;
	std::vector<CameraCoordinates> _cameraCoordinates;

	static HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);


	//std::vector<WCHAR*> _vertexShaderFileList = { L"vertex_shader.hlsl" };
	//std::vector<WCHAR*> _pixelShaderFileList = {  L"pixel_shader.hlsl" };

};

