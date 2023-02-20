#include "Engine/Physics/Shapes/ShapeType.h"

/*
* A container for a particualr shapes bounds, to be used in rendering, mass calulation
* and collision detection.
*/
class Shape
{
public:

	virtual float CalculateMass() const = 0;
	ShapeType GetType() const { return m_Type; };

protected:
	ShapeType m_Type;
};

//Shape::Type Shape::GetType() const { return m_Type; };