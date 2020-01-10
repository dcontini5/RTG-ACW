#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <directxcolors.h>
#include "Resource.h"
//#include "SettingLoader.h"
#include <wrl.h>
#include "DDSTextureLoader.h"
#include "Shape.h"
#include "Particle.h"
#include "Light.h"
#include "Cube.h"

class Device
{
public:

	Device();
	Device(const Device& device) : _hInst(device._hInst), _hWnd(device._hWnd), _pd3dDevice(device._pd3dDevice), _pd3dDevice1(device._pd3dDevice1), _pImmediateContext(device._pImmediateContext), _pImmediateContext1(device._pImmediateContext1),
		_pSwapChain(device._pSwapChain), _pSwapChain1(device._pSwapChain1), _pRenderTargetView(device._pRenderTargetView), _pDepthStencil(device._pDepthStencil), _pDepthStencilView(device._pDepthStencilView), _sampler(device._sampler),
		_pBlendStateNoBlend(device._pBlendStateNoBlend), _pBlendStateBlend(device._pBlendStateBlend), _projection(device._projection), _cameraManager(device._cameraManager), _lightManager(device._lightManager), _settingLoader(device._settingLoader),
		_shapeList(device._shapeList), _particleList(device._particleList) {}

	~Device();

	void Integrate() { for (auto i : _particleList) i->Integrate(dt); }
	void CollisionDetection();
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();
	void CleanupDevice();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Render();

	Device& operator= (const Device& d);

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
	ID3D11SamplerState* _sampler = nullptr;
	ID3D11BlendState* _pBlendStateNoBlend = nullptr;
	ID3D11BlendState* _pBlendStateBlend = nullptr;
	
	DirectX::XMMATRIX _projection;
	Camera* _cameraManager = nullptr;
	Light* _lightManager = nullptr;
	SettingLoader* _settingLoader = nullptr;
	std::vector<Shape*> _shapeList;
	std::vector<Particle*> _particleList;

	float dt = 0.f;
	
	
	
};

