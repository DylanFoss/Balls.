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

	float GetRadius() { return m_Radius; }

	virtual float CalculateMass() const override;
private:
	float m_Radius;
};