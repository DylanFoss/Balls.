#include "glm/glm.hpp"

class PhysicsObject;

struct Manifold
{
	PhysicsObject* objectOne;
	PhysicsObject* objectTwo;
	glm::vec2 collisionNormal;
	float overlap;
};

namespace Collisions {

bool CircleVsCircle(Manifold& manifold);

bool PointVsCircle(const glm::vec2& point, PhysicsObject* object);

}