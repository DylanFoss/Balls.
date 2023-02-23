#include "glm/glm.hpp"

class PhysicsObject;
struct Transform;
class BallCollider;

struct Manifold
{
	PhysicsObject* a;
	PhysicsObject* b;
	glm::vec2 collisionNormal;
	float overlap;

	bool collision = false;
};

namespace Collisions 
{

Manifold CircleVsCircle(const BallCollider* a, const Transform* ta, const BallCollider* b, const Transform* tb);

//bool PointVsCircle(const glm::vec2& point, PhysicsObject* object);

}