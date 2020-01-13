#include "Light.h"

void Light::CreateShadow() {

	
	_pointShadowLights = _pointLights[0];
	_pointShadowLights.Color = { 0.0f, 0.0f, 0.0f, 1.0f };
	_spotShaodwlights = _spotlights;
	_spotShaodwlights[0].Color = { 0.0f, 0.0f, 0.0f, 1.0f };
	_spotShaodwlights[1].Color = { 0.0f, 0.0f, 0.0f, 1.0f };
	_spotShaodwlights[2].Color = { 0.0f, 0.0f, 0.0f, 1.0f };
	_spotShaodwlights[3].Color = { 0.0f, 0.0f, 0.0f, 1.0f };
	
	
}

void Light::SwapLight(const float t) {

	if (t >= _changeCD) {
		
		_activeLight = ++_activeLight % 3;
		_changeCD = t + 0.6f;
		
	}
	
}
