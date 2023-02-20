#pragma once
#include "Engine/Physics/Shapes/Circle.h"

Circle::Circle(float radius)
	:m_Radius(radius)
{
	m_Type = ShapeType::CIRCLE;
}

float Circle::CalculateMass() const
{
	//Obviously not the mass of a circle, but will do for now.
	return m_Radius * m_Radius;
}