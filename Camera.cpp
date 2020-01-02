#include "Camera.h"


Camera::~Camera()
{
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
