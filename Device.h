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
	//ID3D11RasterizerState* _rasterStateBox = nullptr;
	//ID3D11RasterizerState* _rasterStateShape = nullptr;
	//ID3D11RasterizerState* _rasterStateParticle = nullptr;
	//ID3D11ShaderResourceView* _particleTextureRV = nullptr;
	//ID3D11ShaderResourceView* _shapeTextureRV = nullptr;
	//ID3D11ShaderResourceView* _shapeBumpRV = nullptr;
	//ID3D11ShaderResourceView* _skyBoxBumpRV = nullptr;
	ID3D11SamplerState* _sampler = nullptr;
	//ID3D11SamplerState* _boxSampler;
	ID3D11BlendState* _pBlendStateNoBlend = nullptr;
	ID3D11BlendState* _pBlendStateBlend = nullptr;
	ID3D11DepthStencilState* _pDepthStencilStateParticle = nullptr;
	ID3D11DepthStencilState* _pDepthStencilStateShape = nullptr;
	
	DirectX::XMMATRIX _projection;

	SettingLoader* _settingLoader;
	std::vector<Shape*> _shapeList;
	//std::vector<Shape*> _particleList;
	std::vector<Particle*> _particleList;
	Camera* _cameraManager;
	
};

