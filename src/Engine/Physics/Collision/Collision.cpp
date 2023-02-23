#include "Collision.h"

#include "Engine/Physics/PhysicsObject.h"
#include "Engine/Physics/Collision/Collisionpch.h"
#include "Engine/Physics/Body.h"

namespace Collisions {
bool CircleVsCircle(Manifold& manifold)
{
	PhysicsObject* objectOne = manifold.objectOne;
	PhysicsObject* objectTwo = manifold.objectTwo;

	BallCollider* circleOne;
	BallCollider* circleTwo;
	circleOne = static_cast<BallCollider*>(objectOne->GetShape());
	circleTwo = static_cast<BallCollider*>(objectTwo->GetShape());

	glm::vec2 collisionAxis = objectOne->GetPosition() - objectTwo->GetPosition();
	float radii = (circleOne->GetRadius() + circleTwo->GetRadius());

	//use cheaper squared distance first
	float sqrDistance = glm::dot(collisionAxis, collisionAxis);
	if (sqrDistance > radii * radii)
		return false;

	float distance = glm::length(collisionAxis);
	glm::vec2 collisionNormal = glm::normalize(collisionAxis);
	float overlap = radii - distance;

	manifold.collisionNormal = collisionNormal;
	manifold.overlap = overlap;

	return true;
}

bool PointVsCircle(const glm::vec2& point, PhysicsObject* object)
{
	#ifdef  _DEBUG
	assert(object->GetShape()->GetType() == ColliderType::BALL);
	#endif

	BallCollider* circle = static_cast<BallCollider*>(object->GetShape());

	glm::vec2 difference = point - object->GetPosition();
	float sqrDistance = glm::dot(difference, difference);


	if (sqrDistance > circle->GetRadius() * circle->GetRadius())
		return false;

	return true;
}
}
