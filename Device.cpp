#include "Device.h"
#include "Sphere.h"
#include "Cube.h"


Device::Device(){
	
	
}


Device::~Device(){

	delete _cameraManager;
	delete _lightManager;
	delete _settingLoader;
	
}

void Device::Integrate() {

	 for (const auto i : _particleList) i->Integrate(_dt); 
	
}

void Device::CollisionDetection() {

	for (const auto shape : _shapeList) {

		if (!shape->IsDrawable()) continue;
		for (const auto particle : _particleList) {
			const auto radius = particle->GetRadius();
			auto state = particle->GetState();
			shape->Collide(state, radius);
			particle->SetState(state);
		}

	}
	
}

//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT Device::InitWindow(const HINSTANCE hInstance, const int nCmdShow) {

	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"TutorialWindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
	if (!RegisterClassEx(&wcex))
		E_FAIL;

	// Create window
	_hInst = hInstance;
	RECT rc = { 0, 0, 800, 600 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	_hWnd = CreateWindow(L"TutorialWindowClass", L"Real Time Graphics ACW",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);
	if (!_hWnd)
		return E_FAIL;

	ShowWindow(_hWnd, nCmdShow);

	return S_OK;
		
}

//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT Device::InitDevice() {

	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(_hWnd, &rc);
	const UINT width = rc.right - rc.left;
	const UINT height = rc.bottom - rc.top;
	
#ifdef _DEBUG
	
	UINT createDeviceFlags = 0;
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#else
	const UINT createDeviceFlags = 0;
	
#endif

	
	
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &_pd3dDevice, &_featureLevel, &_pImmediateContext);

		if (hr == E_INVALIDARG)
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDevice(nullptr, _driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &_pd3dDevice, &_featureLevel, &_pImmediateContext);
		}

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
	IDXGIFactory1* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = _pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}
	if (FAILED(hr))
		return hr;

	// Create swap chain
	IDXGIFactory2* dxgiFactory2 = nullptr;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
	if (dxgiFactory2)
	{
		// DirectX 11.1 or later
		hr = _pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&_pd3dDevice1));
		if (SUCCEEDED(hr))
		{
			static_cast<void>(_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&_pImmediateContext1)));
		}

		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Width = width;
		sd.Height = height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;


		hr = dxgiFactory2->CreateSwapChainForHwnd(_pd3dDevice, _hWnd, &sd, nullptr, nullptr, &_pSwapChain1);
		if (SUCCEEDED(hr))
		{
			hr = _pSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&_pSwapChain));
		}

		dxgiFactory2->Release();
	}
	else
	{
		// DirectX 11.0 systems
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = _hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(_pd3dDevice, &sd, &_pSwapChain);
	}

	// Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
	dxgiFactory->MakeWindowAssociation(_hWnd, DXGI_MWA_NO_ALT_ENTER);

	dxgiFactory->Release();

	if (FAILED(hr))
		return hr;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return hr;

	hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;


	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = _pd3dDevice->CreateTexture2D(&descDepth, nullptr, &_pDepthStencil);
	if (FAILED(hr))
		return hr;
	
	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof (descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = _pd3dDevice->CreateDepthStencilView(_pDepthStencil, &descDSV, &_pDepthStencilView);
	if (FAILED(hr))
		return hr;
	
	_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _pDepthStencilView);


	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(width);
	vp.Height = static_cast<FLOAT>(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_pImmediateContext->RSSetViewports(1, &vp);

	//Define the Rasterization State



	//rasterDesc.CullMode = D3D11_CULL_NONE;
	//rasterDesc.FillMode = D3D11_FILL_SOLID;
	//rasterDesc.ScissorEnable = false;
	//rasterDesc.DepthBias = 0;
	//rasterDesc.DepthBiasClamp = 0.0f;
	//rasterDesc.DepthClipEnable = false;
	//rasterDesc.MultisampleEnable = false;
	//rasterDesc.SlopeScaledDepthBias = 0.0f;
	//
	//hr = _pd3dDevice->CreateRasterizerState(&rasterDesc, &_rasterStateParticle);
	

	D3D11_DEPTH_STENCIL_DESC depthStencilDescDisabled;
	ZeroMemory(&depthStencilDescDisabled, sizeof(D3D11_DEPTH_STENCIL_DESC));
	
	depthStencilDescDisabled.DepthEnable = FALSE;
	depthStencilDescDisabled.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDescDisabled.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDescDisabled.StencilEnable = FALSE;
	depthStencilDescDisabled.StencilReadMask = 0xFF;
	depthStencilDescDisabled.StencilWriteMask = 0xFF;
	
	// Stencil operations if pixel is front-facing
	depthStencilDescDisabled.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescDisabled.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDescDisabled.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescDisabled.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	//
	//// Stencil operations if pixel is back-facing
	//depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	//depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	//_pd3dDevice->CreateDepthStencilState(&depthStencilDescDisabled, &_pDepthStencilStateParticle);
	

	D3D11_DEPTH_STENCIL_DESC depthStencilDescEnabled;
	ZeroMemory(&depthStencilDescEnabled, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilDescEnabled.DepthEnable = TRUE;
	depthStencilDescEnabled.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDescEnabled.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDescEnabled.StencilEnable = FALSE;
	depthStencilDescEnabled.StencilReadMask = 0xFF;
	depthStencilDescEnabled.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	depthStencilDescEnabled.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescEnabled.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescEnabled.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	depthStencilDescEnabled.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	//
	// Stencil operations if pixel is back-facing
	depthStencilDescEnabled.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescEnabled.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDescEnabled.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescEnabled.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//_pd3dDevice->CreateDepthStencilState(&depthStencilDescEnabled, &_pDepthStencilStateShape);


	D3D11_BLEND_DESC blendDesc;

	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	
	
	hr = _pd3dDevice->CreateBlendState(&blendDesc, &_pBlendStateBlend);
	
	
	blendDesc.RenderTarget[0].BlendEnable = FALSE;

	hr = _pd3dDevice->CreateBlendState(&blendDesc, &_pBlendStateNoBlend);
	
	
	_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	_settingLoader = new SettingLoader();
	_settingLoader->FileLoader(hr, _pd3dDevice, _pImmediateContext);
	_settingLoader->CreateParticleGeometry();
	_settingLoader->CreateEffects();
	
	_cameraManager = new Camera(_settingLoader->GetCameraCoords());

	auto pointCoords = _settingLoader->GetPointCoords();
	auto spotCoords = _settingLoader->GetSpotCoords();
	
	_lightManager = new Light(pointCoords, spotCoords);

	auto j = 0;
	auto coordList = _settingLoader->GetObjectsCoords();
	
	for (auto i : coordList) {

		if (!j) {

			D3D11_RASTERIZER_DESC rasterDesc;
			ZeroMemory(&rasterDesc, sizeof(rasterDesc));
			
			rasterDesc.CullMode = D3D11_CULL_NONE;
			rasterDesc.FillMode = D3D11_FILL_SOLID;
			rasterDesc.ScissorEnable = false;
			rasterDesc.DepthBias = 0;
			rasterDesc.DepthBiasClamp = 0.0f;
			rasterDesc.DepthClipEnable = true;
			rasterDesc.MultisampleEnable = false;
			rasterDesc.SlopeScaledDepthBias = 0.0f;

			auto envMap = _settingLoader->GetEnvMap();
			const auto sky = new Cube(_settingLoader->GetVs(envMap.VertexShader), _settingLoader->GetPs(envMap.PixelShader), i, Type::box, envMap.material);
			hr = sky->CreateBuffers(hr, _pd3dDevice, _settingLoader->GetVertices(0), _settingLoader->GetIndices(0));
			hr = sky->CreateRasterState(_pd3dDevice, rasterDesc);
			hr = sky->CreateTextureResource(_pd3dDevice, L"skymap.dds");
			hr = sky->CreateDepthStencil(_pd3dDevice, depthStencilDescDisabled);
			
			//_shapeList.push_back(sky);
			
			j++;
			continue;

		}if(j==1) {

			D3D11_RASTERIZER_DESC rasterDesc;
			ZeroMemory(&rasterDesc, sizeof(rasterDesc));

			rasterDesc.CullMode = D3D11_CULL_FRONT;
			rasterDesc.FillMode = D3D11_FILL_SOLID;
			rasterDesc.ScissorEnable = false;
			rasterDesc.DepthBias = 0;
			rasterDesc.DepthBiasClamp = 0.0f;
			rasterDesc.DepthClipEnable = false;
			rasterDesc.MultisampleEnable = false;
			rasterDesc.SlopeScaledDepthBias = 0.0f;

			auto boxEff = _settingLoader->GetBoxEffect();
			
			const auto box = new Cube(_settingLoader->GetVs(boxEff.VertexShader), _settingLoader->GetPs(boxEff.PixelShader), i, Type::box, boxEff.material);
			box->CreateOBB();
			hr = box->CreateBuffers(hr, _pd3dDevice, _settingLoader->GetVertices(0), _settingLoader->GetIndices(0));
			hr = box->CreateRasterState(_pd3dDevice, rasterDesc);
			hr = box->CreateDepthStencil(_pd3dDevice, depthStencilDescEnabled);

			_shapeList.push_back(box);
			j++;
			continue;
			
		}
		
		D3D11_RASTERIZER_DESC rasterDesc;
		ZeroMemory(&rasterDesc, sizeof(rasterDesc));
		rasterDesc.CullMode = D3D11_CULL_NONE;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.ScissorEnable = false;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		auto randEff = _settingLoader->GetRandomEffect();
	
		
		if(!((j + 1) % 2)) {
			
			const auto randshape = new Cube(_settingLoader->GetVs(randEff.VertexShader), _settingLoader->GetPs(randEff.PixelShader), i, Type::cube, randEff.material);
			randshape->CreateOBB();
			
		}else {
			
		}
		
		const auto randshape = new Sphere(_settingLoader->GetVs(randEff.VertexShader), _settingLoader->GetPs(randEff.PixelShader), i, randEff.material);
		hr = randshape->CreateBuffers(hr, _pd3dDevice, _settingLoader->GetVertices((j + 1) % 2 ), _settingLoader->GetIndices((j + 1) % 2));
		hr = randshape->CreateRasterState(_pd3dDevice, rasterDesc);
		hr = randshape->CreateDepthStencil(_pd3dDevice, depthStencilDescEnabled);
		hr = randshape->CreateTextureResource(_pd3dDevice, L"stones.dds");
		hr = randshape->CreateBumpResource(_pd3dDevice, L"stones_NM_height.dds");
		_shapeList.push_back(randshape);
		j++;
	}

	
	_settingLoader->ResetAvaiableEffects();
		

	_projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, width / static_cast<FLOAT>(height), 0.01f, 200.0f);
	

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof (sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	hr = _pd3dDevice->CreateSamplerState(&sampDesc, &_sampler);
	_pImmediateContext->PSSetSamplers(0, 1, &_sampler);
	
	
	
	if (FAILED(hr))
		return hr;
	
	return S_OK;
	
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void Device::CleanupDevice() {

	if (_pImmediateContext) _pImmediateContext->ClearState();
	if (_pRenderTargetView) _pRenderTargetView->Release();
	if (_pSwapChain1) _pSwapChain1->Release();
	if (_pSwapChain) _pSwapChain->Release();
	if (_pImmediateContext1) _pImmediateContext1->Release();
	if (_pImmediateContext) _pImmediateContext->Release();
	if (_pd3dDevice1) _pd3dDevice1->Release();
	if (_pd3dDevice) _pd3dDevice->Release();
	
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT Device::WndProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		// Note that this tutorial does not handle resizing (WM_SIZE) requests,
		// so we created the window without the resize border.

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//--------------------------------------------------------------------------------------
// Render the frame
//--------------------------------------------------------------------------------------
void Device::Render() {

	// Update our time
	
	static float lastTime = 0.0f;
	if (_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		_t += static_cast<float>(DirectX::XM_PI) * 0.0125f * 2;
	}
	else
	{
		static ULONGLONG timeStart = 0;
		const ULONGLONG timeCur = GetTickCount64();
		if (timeStart == 0)
			timeStart = timeCur;
		lastTime = _t;
		_t = (timeCur - timeStart) / 1000.0f;
		
		_dt = _t - lastTime;
		
	}




	
	// Just clear the backbuffer
	_pImmediateContext->ClearRenderTargetView(_pRenderTargetView, DirectX::Colors::DarkSlateBlue);
	_pImmediateContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	_pImmediateContext->OMSetBlendState(_pBlendStateNoBlend, nullptr, 1);

	auto x = true;
	for (const auto i : _shapeList) {

		//i->Draw(_pImmediateContext, _cameraManager->GetView(), _cameraManager->GetEye(), _projection, t);
		i->Draw(_pImmediateContext, _lightManager, _cameraManager, _projection,_t);
		if(x) {
			
			x = false;
			continue;
		}

		i->DrawShadow(_pImmediateContext, _lightManager, _cameraManager, _projection,_t);

	}


	
	
	_pImmediateContext->OMSetBlendState(_pBlendStateBlend, nullptr, 1);
	
	for (const auto i : _particleList) {

		i->Draw(_pImmediateContext, _lightManager, _cameraManager, _projection,_t);
		
	}

	
	
	
	_pSwapChain->Present(0, 0);
}

void Device::ExlpodeShape(const int i) {

	D3D11_DEPTH_STENCIL_DESC depthStencilDescDisabled;
	ZeroMemory(&depthStencilDescDisabled, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthStencilDescDisabled.DepthEnable = FALSE;
	depthStencilDescDisabled.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDescDisabled.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDescDisabled.StencilEnable = FALSE;
	depthStencilDescDisabled.StencilReadMask = 0xFF;
	depthStencilDescDisabled.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	depthStencilDescDisabled.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescDisabled.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDescDisabled.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescDisabled.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	//
	HRESULT hr = S_OK;


	
	if(_shapeList[i]->IsDrawable()) {

		_shapeList[i]->StopDrawing();
		ShapeCoordinates coords  = _settingLoader->GetObjectsCoords()[0];
		coords.Pos = _shapeList[i]->GetCoords().Pos;
		
		D3D11_RASTERIZER_DESC rasterDesc;
		rasterDesc.CullMode = D3D11_CULL_NONE;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.ScissorEnable = false;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		auto particleEff = _settingLoader->GetParticleEffect();
		
		
		for (auto j = 0; j < 100; j++) {
					   			
		const auto randV = static_cast<float>(fmod(rand() + j, 6.5f) + 1.f);

		particleEff.material.ambient.w = _t + 5.f;
		const auto particle = new Particle(_settingLoader->GetVs(particleEff.VertexShader), _settingLoader->GetPs(particleEff.PixelShader), coords, { cos(DirectX::XM_2PI / 100 * j) * randV, randV , sin(DirectX::XM_2PI / 100 * j) * randV }, particleEff.material);
		hr = particle->CreateBuffers(hr, _pd3dDevice, _settingLoader->GetVertices(2), _settingLoader->GetIndices(2));
		hr = particle->CreateRasterState(_pd3dDevice, rasterDesc);
		hr = particle->CreateDepthStencil(_pd3dDevice, depthStencilDescDisabled);
		hr = particle->CreateTextureResource(_pd3dDevice, L"sand.dds");
			
		_particleList.push_back(particle);

		}


		
	}


	
}

void Device::ChangeEffects() {

	if (_t < _effChangeCD) return;

	for (auto i = 1; i < _shapeList.size(); i++) {

		if (!_shapeList[i]->IsDrawable()) continue;
		
		const auto newEff = _settingLoader->GetRandomEffect();
		
		_shapeList[i]->SetVS(_settingLoader->GetVs(newEff.VertexShader));
		_shapeList[i]->SetPS(_settingLoader->GetPs(newEff.PixelShader));
		_shapeList[i]->SetMaterial(newEff.material);
		
	}

	_settingLoader->ResetAvaiableEffects();
	_effChangeCD = _t + 1.0f;
}


void Device::ResetScene() {

	for(const auto i : _shapeList) {

		i->StartDrawing();
		
	}
	ChangeEffects();
	_cameraManager->SetCameras(_settingLoader->GetCameraCoords());
	_lightManager->SelectLight(1);
	_particleList.clear();
	
	
	
}

Device& Device::operator=(const Device& d)
{
	_hInst = d._hInst;
	_hInst = d._hInst;
	_driverType = d._driverType;
	_featureLevel = d._featureLevel;
	_hWnd = d._hWnd;
	_pd3dDevice = d._pd3dDevice;
	_pd3dDevice1 = d._pd3dDevice1;
	_pImmediateContext = d._pImmediateContext;
	_pImmediateContext1 = d._pImmediateContext1;
	_pSwapChain = d._pSwapChain;
	_pSwapChain1 = d._pSwapChain1;
	_pRenderTargetView = d._pRenderTargetView;
	_pDepthStencil = d._pDepthStencil;
	_pDepthStencilView = d._pDepthStencilView;
	_sampler = d._sampler;
	_pBlendStateNoBlend = d._pBlendStateNoBlend;
	_pBlendStateBlend = d._pBlendStateBlend;
	_projection = d._projection;
	_cameraManager = d._cameraManager;
	_lightManager = d._lightManager;
	_settingLoader = d._settingLoader;
	_shapeList = d._shapeList;
	_particleList = d._particleList;
	_dt = d._dt;
	_effChangeCD = d._effChangeCD;
	_timeScale = d._timeScale;
	_t = d._t;
	
	return *this;
}


