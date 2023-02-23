#pragma once
#ifndef COLLISIONS_COLLIDER_H_INCLUDED
#define COLLISIONS_COLLIDER_H_INCLUDED
#include "Engine/Physics/Collision/Collider.h"
#endif

class BallCollider
	: public Collider
{
public:
	BallCollider(float radius);

	virtual Manifold TestCollision(const Transform* transform, const Collider* collider, const Transform* colliderTransform) const override;
	virtual Manifold TestCollision(const Transform* transform, const BallCollider* ball, const Transform* ballTransform) const override;

	float GetRadius() const { return m_Radius; }

	virtual float CalculateMass() const override;
private:
	float m_Radius;
};