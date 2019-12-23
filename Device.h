#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <directxcolors.h>
#include "resource.h"
#include "SettingLoader.h"

class Device
{
public:
	Device();
	~Device();
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();
	void CleanupDevice();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Render();

private:
	
	HINSTANCE               _hInst = nullptr;
	HWND                    _hWnd = nullptr;
	D3D_DRIVER_TYPE         _driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       _featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*           _pd3dDevice = nullptr;
	ID3D11Device1*          _pd3dDevice1 = nullptr;
	ID3D11DeviceContext*    _pImmediateContext = nullptr;
	ID3D11DeviceContext1*   _pImmediateContext1 = nullptr;
	IDXGISwapChain*         _pSwapChain = nullptr;
	IDXGISwapChain1*        _pSwapChain1 = nullptr;
	ID3D11RenderTargetView* _pRenderTargetView = nullptr;
	ID3D11Texture2D*		_pDepthStencil = nullptr;
	ID3D11DepthStencilView*	_pDepthStencilView = nullptr;
	DirectX::XMMATRIX _projection;

	SettingLoader* _settingLoader;
	Shape* _sphere;
	std::vector<Shape*> _shapeList;
	Camera* _cameraManager;
	
};

