#include "glm/glm.hpp"

class PhysicsObject;

struct Manifold
{
	PhysicsObject* a;
	PhysicsObject* b;
	glm::vec2 collisionNormal;
	float overlap;
	bool collision;

	Manifold()
		:a(nullptr), b(nullptr), collisionNormal({}), overlap{}, collision(false)
	{};
};