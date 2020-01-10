#pragma once
#include "Shape.h"

using namespace DirectX;

class Sphere : public Shape{

public:
	
	Sphere(ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ShapeCoordinates& coords) : Shape(vertexShader, pixelShader, coords), _radius(coords.Scal.x) {}
	~Sphere() = default;
	void Collide(State& state, float& radius) override { CollisionWithSphere(state, radius); }
	void CollisionWithSphere(State& state, float& radius);
	

private:

	float _radius;
	
	
};

