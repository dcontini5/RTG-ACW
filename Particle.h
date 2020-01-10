#pragma once
#include "Shape.h"
#include "Cube.h"

using namespace DirectX;



class Particle : public Shape{

public:
	
	Particle(ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ShapeCoordinates& coords, DirectX::XMFLOAT3 velocity) :
		Shape(vertexShader, pixelShader, coords), _state(velocity, coords.Pos), _timeToLive(10.0f),
		_radius(0.3f){
	}

	~Particle() = default;

	State GetState() const { return _state; }
	void SetState(State newState) { _state = newState; }
	float GetRadius() const { return _radius; }
	void Integrate(const float& t);
	
private:

	
	State _state;
	float _timeToLive;
	float _radius;
	
	
	
};