#pragma once
#include "Shape.h"
#include "Cube.h"

using namespace DirectX;



class Particle : public Shape{

public:
	
	Particle(ID3D11VertexShader* const vertexShader, ID3D11PixelShader* const pixelShader,ShapeCoordinates& coords,const DirectX::XMFLOAT3& velocity, Material& mat,const float ttl) :
		Shape(vertexShader, pixelShader, coords, mat), _state(velocity, coords.Pos), _timeToLive(ttl + 5.0f),
		_radius(0.3f){
		mat.ambient.w = _timeToLive;
		SetMaterial(mat);
		
	}

	~Particle();

	const State& GetState() const { return _state; }
	void SetState(const State& newState) { _state = newState; }
	float GetRadius() const { return _radius; }
	void Integrate(const float& t);
	void Collide(State& state, const float& radius) override;
	
private:

	
	State _state;
	float _timeToLive;
	float _radius;
	
	
	
};