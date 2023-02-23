#include "glm/glm.hpp"

#include "Engine/Physics/Collision/Manifold.h"

class PhysicsObject;
struct Transform;
class BallCollider;

namespace Collisions 
{

Manifold CircleVsCircle(const BallCollider* a, const Transform* ta, const BallCollider* b, const Transform* tb);

bool PointVsCircle(const glm::vec2& a, const BallCollider* b, const Transform* tb);

}