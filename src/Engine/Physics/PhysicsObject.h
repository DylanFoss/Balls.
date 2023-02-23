class Body;
struct BodyDefinition;
class Collider;

#include "glm/glm.hpp"

/*
* A physics object that contains a body and a shape, which combine to have a presence
* in the world.
*/
class PhysicsObject
{
public:

	PhysicsObject(Collider* shape, const BodyDefinition& body);
	~PhysicsObject();

	void Update(float deltaTime);

	glm::vec2 GetPosition() const;
	glm::vec2 GetVelocity() const;

	void OffsetPosition(const glm::vec2& pos);
	void SetPosition(const glm::vec2& pos);
	void Accelerate(const glm::vec2& acc);

	float GetMass();
	float GetInverseMass();

	Collider* GetShape() { return m_Shape; };

private:
	Body* m_Body;
	Collider* m_Shape;
};