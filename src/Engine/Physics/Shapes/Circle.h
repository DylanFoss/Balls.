#pragma once
#ifndef SHAPES_SHAPE_H_INCLUDED
#define SHAPES_SHAPE_H_INCLUDED
#include "Engine/Physics/Shapes/Shape.h"
#endif

class Circle
	: public Shape
{
public:
	Circle(float radius);

	virtual float CalculateMass() const override;
private:
	float m_Radius;
};