#include "glm/glm.hpp"

struct Manifold
{
	size_t a;
	size_t b;

	glm::vec2 collisionNormal = { 0,0 };
	float overlap = 0;
};
