#include "Engine/Physics/Collision/ColliderType.h"

struct Transform;
struct Manifold;

class BallCollider;

/*
* A container for a particualr shapes bounds, to be used in rendering, mass calulation
* and collision detection.
*/
class Collider
{
public:
	virtual ~Collider() {};

	virtual Manifold TestCollision(const Transform* transform, const Collider* collider, const Transform* colliderTransform) const = 0;
	virtual Manifold TestCollision(const Transform* transform, const BallCollider* ball, const Transform* ballTransform) const = 0;

	virtual float CalculateMass() const = 0;
	ColliderType GetType() const { return m_Type; };

	virtual Collider* Clone() const = 0;

protected:
	ColliderType m_Type;
};

//Shape::Type Shape::GetType() const { return m_Type; };