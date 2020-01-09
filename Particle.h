#pragma once
#include "Shape.h"

using namespace DirectX;



class Particle : public Shape{

public:
	
	Particle(ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ShapeCoordinates& coords, DirectX::XMFLOAT3 velocity) :
		Shape(vertexShader, pixelShader, coords), _velocity0(velocity), _position0(coords.Pos), _timeToLive(10.0f),
		_radius(0.5f){
	}

	~Particle() = default;

	void Update();
	//void Draw();
	void Integrate(const float& t);
	void CollisionWithBox(const OBB cube);
	void CollisionWithCube(const OBB cube);
	void CollisionWithSphere(const XMVECTOR sPos, const float sRadius);
	//void CollisionWithSphere(glm::vec3 pos, const float radius, const float time);
	
private:


	XMFLOAT3 _velocity0;
	XMFLOAT3 _acceleration = XMFLOAT3(0.f, -9.8f, 0.f);
	XMFLOAT3 _position0;
	float _timeToLive;
	float _radius;
	
	
	
};