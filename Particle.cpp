#include "Particle.h"

void Particle::Integrate(const float& t, const float& dt) {


	const auto vel = XMVectorSet(_velocity0.x, _velocity0.y, _velocity0.z, 0.f);
	const auto acc = XMVectorSet(_acceleration.x, _acceleration.y, _acceleration.z, 0.f);
	auto pos = XMVectorSet(_position0.x, _position0.y, _position0.z, 0.f);


	pos += pow(t, 2) / 2 * acc + t * vel;


	this->SetPos({pos.m128_f32[0], pos.m128_f32[1], pos.m128_f32[2]});
	
	
}

void Particle::CollisionWithBox(const OBB cube) {

	const auto pPos = XMVectorSet(this->GetCoords().Pos.x, this->GetCoords().Pos.y, this->GetCoords().Pos.z, 0.f);

	const auto v = pPos - cube.Center;
	auto sqDist = 0.f;

	for(auto i = 0; i < 3; i++) {
		
		const auto vProj = XMVector3Dot(v, cube.AxisOrientation[i]).m128_f32[0];
		

		if (vProj - _radius <= -cube.BoxHalfwidth[i] && vProj + _radius >= cube.BoxHalfwidth[i]) {
			
			_velocity0 = { 0.f, 0.f, 0.f };
			_acceleration = { 0.f, 0.f, 0.f };
			break;
			
		}

		
	}
	
}

void Particle::CollisionWithCube(const OBB cube) {
	
	const auto pPos = XMVectorSet(this->GetCoords().Pos.x, this->GetCoords().Pos.y, this->GetCoords().Pos.z, 0.f);

	const auto v = pPos - cube.Center;
	auto sqDist = 0.f;

	for (auto i = 0; i < 3; i++) {

		const auto vProj = XMVector3Dot(v, cube.AxisOrientation[i]).m128_f32[0];
		auto excess = 0.f;

		if (vProj < -cube.BoxHalfwidth[i]) excess = vProj + cube.BoxHalfwidth[i];
		else if (vProj > cube.BoxHalfwidth[i]) excess = vProj - cube.BoxHalfwidth[i];

		sqDist += excess * excess;

	}

	if (sqDist < _radius * _radius) {

		_velocity0 = { 0.f, 0.f, 0.f };
		_acceleration = { 0.f, 0.f, 0.f };
	}
		

}

void Particle::CollisionWithSphere(const XMVECTOR sPos, const float sRadius) {

	const auto pPos = XMVectorSet(this->GetCoords().Pos.x, this->GetCoords().Pos.y, this->GetCoords().Pos.z, 0.f);

	
	if( XMVector3Length(pPos - sPos).m128_f32[0] - (_radius + sRadius) <= 0) {

		_velocity0 = { 0.f, 0.f, 0.f };
		_acceleration = { 0.f, 0.f, 0.f };

	};


	
}