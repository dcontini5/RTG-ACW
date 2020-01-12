#include "Particle.h"

Particle::~Particle() {
}

void Particle::Integrate(const float& t) {

	auto acc = XMLoadFloat3(&_state.Acceleration);
	auto vel = XMLoadFloat3(&_state.Velocity);
	auto pos = XMVectorSet(this->GetCoords().Pos.x, this->GetCoords().Pos.y, this->GetCoords().Pos.z, 0.f);
	const auto drag = 0.080605f *  vel * vel;
	
	acc -= drag;
	pos += vel * t;
	vel += acc * t;
	
	this->SetPos({pos.m128_f32[0], pos.m128_f32[1], pos.m128_f32[2]});
	XMStoreFloat3(&_state.Position, pos);
	XMStoreFloat3(&_state.Velocity, vel);
	
}

void Particle::Collide(State& state, const float& radius) {

	state.Acceleration.x = 0.f;
	
	return;
	
}
