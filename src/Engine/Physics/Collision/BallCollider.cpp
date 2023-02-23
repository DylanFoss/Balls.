#pragma once
#include "Engine/Physics/Collision/BallCollider.h"

#include "Engine/Physics/Collision/Collision.h"

BallCollider::BallCollider(float radius)
	:m_Radius(radius)
{
	m_Type = ColliderType::BALL;
}

Manifold BallCollider::TestCollision(const Transform* transform, const Collider* collider, const Transform* colliderTransform) const
{
	return collider->TestCollision(colliderTransform, this, transform);
}

//on double dispatch, the colliders are 'flipped', so pass 'this' 2nd,
//to keep the initally called collider as collider 'a'.
Manifold BallCollider::TestCollision(const Transform* transform, const BallCollider* ball, const Transform* ballTransform) const
{
	return Collisions::CircleVsCircle(ball, ballTransform, this, transform);
}

// debating this being here, since it is now a collider
// (seperate shape class? too much abstraction?)
float BallCollider::CalculateMass() const
{
	//Obviously not the mass of a circle, but will do for now.
	return m_Radius * m_Radius;
}