#pragma once
#include "Engine/Physics/Collision/BallCollider.h"

BallCollider::BallCollider(float radius)
	:m_Radius(radius)
{
	m_Type = ColliderType::BALL;
}

// debating this being here, since it is now a collider
// (seperate shape class? too much abstraction?)
float BallCollider::CalculateMass() const
{
	//Obviously not the mass of a circle, but will do for now.
	return m_Radius * m_Radius;
}