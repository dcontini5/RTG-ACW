#include "Light.h"

void Light::SwapLight(float t) {

	if (t >= _changeCD) {
		
		_activeLight = ++_activeLight % 3;
		_changeCD = t + 0.6f;
		
	}
	
}
