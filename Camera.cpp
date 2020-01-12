#include "Camera.h"


Camera::~Camera()
{
}

void Camera::SetCameras(const std::vector<CameraCoordinates>& cameras) {

	_camera1 = cameras[0];
	_camera2 = cameras[1];
	_activeCamera = 1;
	
}




