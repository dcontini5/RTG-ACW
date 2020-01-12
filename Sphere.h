#pragma once
#include "Shape.h"

using namespace DirectX;

class Sphere : public Shape{

public:
	
	Sphere(ID3D11VertexShader* const vertexShader, ID3D11PixelShader* const pixelShader, ShapeCoordinates& coords, const Material& mat) : Shape(vertexShader, pixelShader, coords, mat), _radius(coords.Scal.x) {}
	~Sphere();
	void Collide(State& state, const float& radius) override;
	void CollisionWithSphere(State& state,const float& radius)const;
	

private:

	float _radius;
	
	
};

