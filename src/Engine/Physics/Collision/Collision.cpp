#include "Collision.h"

#include "Engine/Physics/Collision/Collisionpch.h"
#include "Engine/Physics/Body.h"


Manifold Collisions::CircleVsCircle(const BallCollider* a, const Transform* ta, const BallCollider* b, const Transform* tb)
{
	Manifold m;

	glm::vec2 collisionAxis = ta->m_Position - tb->m_Position;
	float radii = (a->GetRadius() + b->GetRadius());

	//use cheaper squared distance first
	float sqrDistance = glm::dot(collisionAxis, collisionAxis);
	if (sqrDistance > radii * radii)
		return m;

	m.collision = true;

	float distance = glm::length(collisionAxis);
	glm::vec2 collisionNormal = glm::normalize(collisionAxis);
	float overlap = radii - distance;

	m.collisionNormal = collisionNormal;
	m.overlap = overlap;

	return m;
}

bool Collisions::PointVsCircle(const glm::vec2& a, const BallCollider* b, const Transform* tb)
{
	glm::vec2 difference = a - tb->m_Position;
	float sqrDistance = glm::dot(difference, difference);


	if (sqrDistance > b->GetRadius() * b->GetRadius())
		return false;

	return true;
}

