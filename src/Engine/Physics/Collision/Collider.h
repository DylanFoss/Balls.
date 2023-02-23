#include "Engine/Physics/Collision/ColliderType.h"

/*
* A container for a particualr shapes bounds, to be used in rendering, mass calulation
* and collision detection.
*/
class Collider
{
public:

	virtual float CalculateMass() const = 0;
	ColliderType GetType() const { return m_Type; };

protected:
	ColliderType m_Type;
};

//Shape::Type Shape::GetType() const { return m_Type; };