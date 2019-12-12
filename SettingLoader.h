#pragma once
#include <vector>
#include <d3d11.h>
#include <windows.h>
#include <d3dcompiler.h>

class SettingLoader
{
public:
	SettingLoader();
	~SettingLoader();
	//HRESULT SettingLoader::CreateVertexShaders(HRESULT hr, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext);
	//HRESULT SettingLoader::CreatePixelShaders(HRESULT hr, ID3D11Device* pd3dDevice);
	//ID3D11PixelShader* getPS() { return _pixelShaderList[0]; };
	//ID3D11VertexShader* getVS() { return _vertexShaderList[0]; };
	//ID3D11InputLayout* getvertexLayout() { return _pVertexLayout; };

private:

	std::vector<ID3D11VertexShader*> _vertexShaderList;
	std::vector<ID3D11PixelShader*> _pixelShaderList;
	ID3D11InputLayout* _pVertexLayout;


	//HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

};

