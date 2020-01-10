#include "Cube.h"

Cube::~Cube() {
}

void Cube::CreateOBB() {

 this->GetCoords();
	
	_obb.BoxHalfwidth[0] = this->GetCoords().Scal.x;
	_obb.BoxHalfwidth[1] = this->GetCoords().Scal.y;
	_obb.BoxHalfwidth[2] = this->GetCoords().Scal.z;

	_obb.Center = XMVectorSet(this->GetCoords().Pos.x, this->GetCoords().Pos.y, this->GetCoords().Pos.z, 0.f);

	const auto rot = XMQuaternionRotationRollPitchYaw(0.f, this->GetCoords().Rot.y, 0.f);
	
	_obb.AxisOrientation[0] = XMVector3Rotate({1.f, 0.f, 0.f, 0.f}, rot);
	_obb.AxisOrientation[1] = XMVector3Rotate({0.f, 1.f, 0.f, 0.f}, rot);
	_obb.AxisOrientation[2] = XMVector3Rotate({0.f, 0.f, 1.f, 0.f}, rot);
	
	
}

void Cube::Collide(State& state,const float& radius) {

	if (_type == Type::cube) {

		CollisionWithCube(state, radius);
		
	}else {

		CollisionWithBox(state, radius);
		
	}

}


void Cube::CollisionWithBox(State& state, const float& radius)const {

	const auto pPos = XMLoadFloat3(&state.Position);

	const auto v = pPos - _obb.Center;

	for (auto i = 1; i < 4; i++) {

		const auto j = i % 3;
		
		const auto vProj = XMVector3Dot(v, _obb.AxisOrientation[j]).m128_f32[0];


		if (vProj - radius <= -_obb.BoxHalfwidth[j] || vProj + radius >= _obb.BoxHalfwidth[j]) {

			if (_obb.AxisOrientation[j].m128_f32[1] >= 1.f) {

				state.Velocity = { 0.f, 0.f, 0.f };
				state.Acceleration = { 0.f, 0.f, 0.f };

			}
			else {

				state.Velocity.x = 0.f;
				state.Velocity.z = 0.f;

			}

			break;

		}


	}

}

void Cube::CollisionWithCube(State& state,const float& radius) const{

	const auto pPos = XMLoadFloat3(&state.Position);
	

	const auto v = pPos - _obb.Center;
	auto sqDist = 0.f;

	for (auto i = 0; i < 3; i++) {

		const auto vProj = XMVector3Dot(v, _obb.AxisOrientation[i]).m128_f32[0];
		auto excess = 0.f;

		if (vProj < -_obb.BoxHalfwidth[i]) excess = vProj + _obb.BoxHalfwidth[i];
		else if (vProj > _obb.BoxHalfwidth[i]) excess = vProj - _obb.BoxHalfwidth[i];

		sqDist += excess * excess;

	}

	if (sqDist < radius * radius) {

		if (_obb.BoxHalfwidth[1] > this->GetCoords().Pos.y + radius) {


			state.Velocity.x = 0.f;
			state.Velocity.z = 0.f;
		}
		else {

			state.Velocity = { 0.f, 0.f, 0.f };
			state.Acceleration = { 0.f, 0.f, 0.f };

		}
	}


}