#pragma once
#include <DirectXMath.h>
#include <utility>
#include <vector>

struct PointLight {

	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT3 Pos;

};

struct SpotLight {

	DirectX::XMFLOAT4 Pos;
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT3 Direction;
	float Spot;

};


class Light {

	
public:

	Light(std::vector<PointLight> &pointLights, std::vector<SpotLight> &spotLights) : _pointLights(std::move(pointLights)), _spotlights(
		                                                                                std::move(spotLights)), _activeLight(0){};
	~Light() = default;
 	void SetLight(PointLight& pLight, std::vector<SpotLight>& sLight) { if (!_activeLight) sLight = _spotlights; else pLight = _pointLights[_activeLight - 1]; };
	void SwapLight(float t);
	void SelectLight(const int i) { _activeLight = i; };

	
private:
	
	std::vector<PointLight> _pointLights;
	std::vector<SpotLight> _spotlights;
	int _activeLight;
	float _changeCD = 0.f;
	
};
