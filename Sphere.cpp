#include "Sphere.h"

void Sphere::CollisionWithSphere (State& state,const float& radius) const{

	const auto sPos = XMVectorSet(this->GetCoords().Pos.x, this->GetCoords().Pos.y, this->GetCoords().Pos.z, 0.f);
	const auto pPos = XMLoadFloat3(&state.Position);
	
	auto vel = XMLoadFloat3(&state.Velocity);

	if (XMVector3Length(pPos - sPos).m128_f32[0] - (_radius + radius) <= 0) {

		vel = XMVector3Reflect(vel, XMVector3Normalize(pPos - sPos));
		XMStoreFloat3(&state.Velocity, vel);

		state.Velocity.x *= 0.01f;
		state.Velocity.z *= 0.01f;

	};



}Sphere::~Sphere() {
}

void Sphere::Collide(State& state, const float& radius) {

	CollisionWithSphere(state, radius);
	
}
