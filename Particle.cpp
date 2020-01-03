#include "Particle.h"

void Particle::Integrate(const float& t, const float& dt) {

	const auto acc = glm::vec3(0.f, 9.8f, 0.f);

	_newState.Position = pow(t, 2) / 2 * acc + t * _state.Velocity + _state.Position;
	_newState.Velocity = t * acc + _state.Velocity;

	
	
}

void Particle::CollisionWithBox(glm::vec3 normal, glm::vec3 pos, float time) {

	//auto dist = glm::dot(normal, this->GetPos());

	if (pos.x < -5.f + _radius || pos.x > 5.f - _radius
		|| pos.z < -5.f + _radius || pos.z > 5.f - _radius) _state.Velocity = glm::vec3(0.f);

	if (pos.y < -5.f) _accelleration = glm::vec3(0.f);


	
	if (glm::abs(dist) <= _radius) {


		if (inside == 3) {

			ManifoldPoint mp;
			mp.contactID1 = this;
			mp.contactID2 = nullptr;
			mp.pointOfImpact = this->GetPos() - dist * plane.normal;
			mp.timeAfterCollision = time;
			mp.contactNormal = plane.normal;
			contactManifold->Add(mp);

		}

	}
	else {

		const auto denom = glm::dot(plane.normal, this->GetVel());
		if (denom * dist < 0) {

			const auto r = dist > 0.0f ? this->GetRadius() : -this->GetRadius();
			const auto timeOfImpact = (r - dist) / denom;
			if (timeOfImpact <= time) {

				const auto pointOfImpact = this->GetPos() + timeOfImpact * this->GetVel() - r * plane.normal;

				if (inside == 3) {


					ManifoldPoint mp;
					mp.contactID1 = this;
					mp.contactID2 = nullptr;
					mp.pointOfImpact = pointOfImpact;
					mp.timeAfterCollision = time - timeOfImpact;
					mp.contactNormal = plane.normal;
					contactManifold->Add(mp);


				}

			}

		}

	}

}

void Particle::CollisionWithCube(float size, glm::vec3 pos) {

	
	
	auto x = glm::max(-1.f + size, glm::max(_state.Position.x, 1.f + size));


	
}


void Particle::CollisionWithSphere(glm::vec3 pos,const float radius,const float time){

	
	const glm::vec3 pos1 = this->GetNewPos();
	float dist = glm::distance(pos1, pos) - (this->GetRadius() + radius);
	
	if (dist < 0.0f)
	{
		ManifoldPoint mp;
		mp.contactID1 = this;
		mp.contactID2 = otherSphere;
		mp.contactNormal = glm::normalize(abs(pos2 - pos1));
		mp.dist = dist / 2;
		contactManifold->Add(mp);
	}

}

void Particle::CollisionResponse( float e)
{


	const glm::vec3 colNormal = point.contactNormal;
	
	point.contactID1->SetNewPos(point.contactID1->GetNewPos() + point.dist * colNormal);
	point.contactID1->SetNewVel((point.contactID1->GetVel() - (1.0f + e) * colNormal * glm::dot(colNormal, point.contactID1->GetVel())));
	

}