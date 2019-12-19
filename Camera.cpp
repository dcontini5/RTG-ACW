#include "Camera.h"


Camera::~Camera()
{
}

void Camera::InitCamera(const DirectX::XMFLOAT4 eye, const DirectX::XMFLOAT4 up, const bool sel) {

	if (sel) {
		
		_camera1.Eye = DirectX::XMVectorSet(eye.x, eye.y, eye.z, eye.w);
		_camera1.At = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		_camera1.Up = DirectX::XMVectorSet(up.x, up.y, up.z, up.w);

	}else {

		_camera2.Eye = DirectX::XMVectorSet(eye.x, eye.y, eye.z, eye.w);
		_camera2.At = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		_camera2.Up = DirectX::XMVectorSet(up.x, up.y, up.z, up.w);

	}
}
