#pragma once
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <vector>

struct CameraCoordinates {

	DirectX::XMVECTOR Eye;
	DirectX::XMVECTOR At;
	DirectX::XMVECTOR Up;

};


class Camera
{
public:
	Camera() {}

	explicit Camera(const std::vector<CameraCoordinates>& cameras): _camera1(cameras[0]), _camera2(cameras[1]),
	                                                              _view(DirectX::XMMatrixLookAtLH(cameras[0].Eye, cameras[0].At, cameras[0].Up)), _activeCamera(true) {};
	~Camera();
	
	void SwapCamera() { _activeCamera = !_activeCamera; _view = _activeCamera ? DirectX::XMMatrixLookAtLH(_camera1.Eye, _camera1.At, _camera1.Up) : DirectX::XMMatrixLookAtLH(_camera2.Eye, _camera2.At, _camera2.Up); }
	DirectX::XMMATRIX GetView() const { return _view; }
	DirectX::XMVECTOR GetEye() const { return _activeCamera ? _camera1.Eye : _camera2.Eye; }
	//todo camera movements
	
private:

	CameraCoordinates _camera1;
	CameraCoordinates _camera2;
	DirectX::XMMATRIX _view;
	bool _activeCamera;
};

