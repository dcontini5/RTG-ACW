#include "Camera.h"


Camera::~Camera()
{
}

void Camera::InitCamera(const DirectX::XMFLOAT3 eye, const DirectX::XMFLOAT3 up, const bool sel) {

	if (sel) {
		
		_camera1.Eye = DirectX::XMVectorSet(eye.x, eye.y, eye.z, 0);
		_camera1.At = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		_camera1.Up = DirectX::XMVectorSet(up.x, up.y, up.z, 0);

	}else {

		_camera2.Eye = DirectX::XMVectorSet(eye.x, eye.y, eye.z, 0);
		_camera2.At = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		_camera2.Up = DirectX::XMVectorSet(up.x, up.y, up.z, 0);

	}
}

void Camera::InitCamera(CameraCoordinates coords, const bool sel) {

	if (sel) {
		
		_camera1.Eye = coords.Eye;
		_camera1.At = coords.At;
		_camera1.Up = coords.Up;
		_view = DirectX::XMMatrixLookAtLH(_camera1.Eye, _camera1.At, _camera1.Up);
		
	}//else {

	//	_camera2.Eye = coords.Eye;
	//	_camera2.At = coords.At;
	//	_camera2.Up = coords.Up;
	//	_view = DirectX::XMMatrixLookAtLH(_camera2.Eye, _camera1.At, _camera1.Up);

	//}
	
}
