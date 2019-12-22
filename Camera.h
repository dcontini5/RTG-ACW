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
	Camera(const CameraCoordinates camera1, const CameraCoordinates camera2): _camera1(camera1), _camera2(camera2) {};
	Camera(const std::vector<CameraCoordinates>& cameras): _camera1(cameras[0]), _camera2(cameras[1]) {};
	~Camera();
	void InitCamera(const DirectX::XMFLOAT3,const DirectX::XMFLOAT3,const bool);
	void InitCamera(const CameraCoordinates,const bool);
	void SwapCamera(const bool sel) { _view = sel ? DirectX::XMMatrixLookAtLH(_camera1.Eye, _camera1.At, _camera1.Up) : DirectX::XMMatrixLookAtLH(_camera2.Eye, _camera2.At, _camera2.Up); }
	DirectX::XMMATRIX GetCamera() const { return _view; }
	//todo camera movements
	
private:

	CameraCoordinates _camera1;
	CameraCoordinates _camera2;
	DirectX::XMMATRIX _view;

};

