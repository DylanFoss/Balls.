#include "Collision.h"

Manifold Collisions::CircleVsCircle(const float radiusA, const glm::vec2& posA, const float radiusB, const glm::vec2& posB)
{
	Manifold m;

	glm::vec2 collisionAxis = posA - posB;
	float radii = radiusA + radiusB;

	//use cheaper squared distance first
	float sqrDistance = glm::dot(collisionAxis, collisionAxis);
	if (sqrDistance > radii * radii)
		return m;

	float distance = glm::length(collisionAxis);
	glm::vec2 collisionNormal = glm::normalize(collisionAxis);
	float overlap = radii - distance;

	m.collisionNormal = collisionNormal;
	m.overlap = overlap;

	return m;
}

//bool Collisions::PointVsCircle(const glm::vec2& a, const BallCollider* b, const Transform* tb)
//{
//	glm::vec2 difference = a - tb->m_Position;
//	float sqrDistance = glm::dot(difference, difference);
//
//
//	if (sqrDistance > b->GetRadius() * b->GetRadius())
//		return false;
//
//	return true;
//}

