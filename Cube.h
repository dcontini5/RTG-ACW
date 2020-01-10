#pragma once
#include "Shape.h"

using namespace DirectX;

class Cube : public Shape {

public:
	
	Cube(ID3D11VertexShader* const vertexShader, ID3D11PixelShader* const pixelShader, ShapeCoordinates& coords,const Type type) : Shape(vertexShader, pixelShader, coords), _type(type) {}
	~Cube();
	void CreateOBB();
	void Collide(State& state,const float& radius) override;
	void CollisionWithBox (State& state,const float& radius)const;
	void CollisionWithCube(State& state,const float& radius)const;

private:

	Type _type;
	OBB _obb = {};
	
	
};

