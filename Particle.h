#pragma once
#include "Shape.h"
#include "glm/glm.hpp"


struct State {

	glm::vec3 Position;
	glm::vec3 Velocity;

	State(const DirectX::XMFLOAT3 pos, const DirectX::XMFLOAT3 vel) : Position(pos.x, pos.y, pos.z),
		Velocity(vel.x, vel.y, vel.z) {};
	State() : Position(0.0f), Velocity(0.0f) {}
	
};



class Particle : public Shape{

public:
	
	Particle(ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ShapeCoordinates& coords, DirectX::XMFLOAT3 velocity) :
		Shape(vertexShader, pixelShader, coords), _state(coords.Pos, velocity), _newState(), _timeToLive(10.0f),
		_radius(0.5f){
	}

	~Particle() = default;

	void Update();
	void Draw();
	void Integrate(const float& t, const float& dt);
	void CollisionWithBox(glm::vec3 normal, glm::vec3 pos, float time);
	void CollisionWithCube(float size, glm::vec3 pos);
	void CollisionWithSphere(glm::vec3 pos, const float radius, const float time);
	
private:


	State _state;
	State _newState;
	float _timeToLive;
	float _radius;
	
	
	
};