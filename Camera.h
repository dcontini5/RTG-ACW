#pragma once
#include <d3d11_1.h>
#include <DirectXMath.h>

struct CamCoords {

	DirectX::XMVECTOR Eye;
	DirectX::XMVECTOR At;
	DirectX::XMVECTOR Up;
	
};


class Camera
{
public:
	Camera() {}
	Camera(const CamCoords camera1, const CamCoords camera2): _camera1(camera1), _camera2(camera2) {};
	~Camera();
	void InitCamera(DirectX::XMFLOAT4, DirectX::XMFLOAT4, bool);
	void SwapCamera(const bool sel) { _view = sel ? DirectX::XMMatrixLookAtLH(_camera1.Eye, _camera1.At, _camera1.Up) : DirectX::XMMatrixLookAtLH(_camera2.Eye, _camera2.At, _camera2.Up); }
	DirectX::XMMATRIX GetCamera() const { return _view; }
	//todo camera movements
	
private:

	CamCoords _camera1;
	CamCoords _camera2;
	DirectX::XMMATRIX _view;

};

