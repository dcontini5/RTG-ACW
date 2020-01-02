#pragma once
#include "Shape.h"

class Particle : public Shape{

public:
	
	Particle(ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ShapeCoordinates& coords) :
		Shape(vertexShader, pixelShader, coords), _position(coords.Pos), _timeToLive(10.0f), _radius(0.5f) {
	}

	~Particle() = default;


private:

	DirectX::XMFLOAT3 _position;
	DirectX::XMFLOAT3 _velocity{};
	float _timeToLive;
	float _radius;
	
};

