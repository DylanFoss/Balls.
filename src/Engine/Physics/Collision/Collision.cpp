#include "Collision.h"

#include "Engine/Physics/PhysicsObject.h"
#include "Engine/Physics/Shapes/ShapePCH.h"
#include "Engine/Physics/Body.h"

namespace Collisions {
bool CircleVsCircle(Manifold& manifold)
{
	PhysicsObject* objectOne = manifold.objectOne;
	PhysicsObject* objectTwo = manifold.objectTwo;

	Circle* circleOne;
	Circle* circleTwo;
	circleOne = static_cast<Circle*>(objectOne->GetShape());
	circleTwo = static_cast<Circle*>(objectTwo->GetShape());

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
	assert(object->GetShape()->GetType() == ShapeType::CIRCLE);
	#endif

	Circle* circle = static_cast<Circle*>(object->GetShape());

	glm::vec2 difference = point - object->GetPosition();
	float sqrDistance = glm::dot(difference, difference);


	if (sqrDistance > circle->GetRadius() * circle->GetRadius())
		return false;

	return true;
}
}
