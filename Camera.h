#pragma once
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

struct CameraCoordinates {

	XMVECTOR Eye;
	XMVECTOR Front;
	XMVECTOR Up;
	XMVECTOR Rot = {0.f, 0.f, 0.f, 0.f};
	
};


class Camera
{
public:
	
	explicit Camera(const std::vector<CameraCoordinates>& cameras): _camera1(cameras[0]), _camera2(cameras[1]), _activeCamera(1) {};
	~Camera();

	
	
	void SwapCamera(const int& i) { _activeCamera = i; }
	XMVECTOR GetEye() const { return _activeCamera == 1 ? _camera1.Eye : _camera2.Eye; }
	XMVECTOR GetFront() const { return _activeCamera == 1 ? _camera1.Front : _camera2.Front; }
	XMVECTOR GetUp() const { return _activeCamera == 1 ? _camera1.Up : _camera2.Up; }
	XMVECTOR GetRot() const { return _activeCamera == 1 ? _camera1.Rot : _camera2.Rot; }
	XMMATRIX GetView() const {	return XMMatrixLookAtLH(GetEye(), GetFront() + GetEye(), GetUp()) * XMMatrixRotationRollPitchYawFromVector(GetRot()); }
	void SetCameras(const std::vector<CameraCoordinates>& cameras);
	
	void MoveCameraLeftRight(const float& dir) { if (_activeCamera) _camera1.Eye += dir * _camSpeed * XMVector3Normalize(XMVector3Cross(_camera1.Front, _camera1.Up)); else  _camera2.Eye += dir * _camSpeed * XMVector3Normalize(XMVector3Cross(_camera2.Front, _camera2.Up));}
	void MoveCameraBackForward(const float& dir) { if(_activeCamera) _camera1.Eye += dir * _camSpeed * _camera1.Front; else  _camera2.Eye += dir * _camSpeed * _camera2.Front; }
	void MoveCameraUpDown(const float& dir) { if(_activeCamera) _camera1.Eye += dir * _camSpeed * _camera1.Up; else  _camera2.Eye += dir * _camSpeed * _camera2.Up; }
	void RotateCameraLeftRight(const float& dir) { if (_activeCamera) _camera1.Rot.m128_f32[1] += _camRotSpeed * dir; else  _camera2.Rot.m128_f32[1] += _camRotSpeed * dir; }
	void RotateCameraUpDown(const float& dir) { if(_activeCamera) _camera1.Rot.m128_f32[0] += _camRotSpeed * dir; else   _camera2.Rot.m128_f32[0] += _camRotSpeed * dir;
	}
	
	
private:

	CameraCoordinates _camera1;
	CameraCoordinates _camera2;
	int _activeCamera;
	float _camSpeed = 0.1f;
	float _camRotSpeed = 0.01f;
};

