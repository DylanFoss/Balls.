class Body;
struct BodyDefinition;
class Shape;

#include "glm/glm.hpp"

/*
* A physics object that contains a body and a shape, which combine to have a presence
* in the world.
*/
class PhysicsObject
{
public:

	PhysicsObject(Shape* shape, const BodyDefinition& body);
	~PhysicsObject();

	void Update(float deltaTime);

	glm::vec2 GetPosition() const;
	glm::vec2 GetVelocity() const;

	void OffsetPosition(const glm::vec2& pos);
	void SetPosition(const glm::vec2& pos);
	void Accelerate(const glm::vec2& acc);

private:
	Body* m_Body;
	Shape* m_Shape;
};