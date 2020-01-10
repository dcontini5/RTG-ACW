#pragma once
#include "Shape.h"

using namespace DirectX;

class Cube : public Shape {

public:
	
	Cube(ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ShapeCoordinates& coords, Type type) : Shape(vertexShader, pixelShader, coords), _type(type) {}
	~Cube() = default;
	void CreateOBB();
	void Collide(State& state, float& radius) override;
	void CollisionWithBox(State& state, float& radius);
	void CollisionWithCube(State& state, float& radius);

private:

	Type _type;
	OBB _obb = {};
	
	
};

