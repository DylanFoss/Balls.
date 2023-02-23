class Body;
struct BodyDefinition;
class Collider;

#ifndef TRANSFORM_H_INCLUDED
#include "Engine/Physics/Transform.h"
#define TRANSFORM_H_INCLUDED
#endif

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

	PhysicsObject(const PhysicsObject& other);
	PhysicsObject operator=(const PhysicsObject& other);
	PhysicsObject(PhysicsObject&& other) noexcept;
	PhysicsObject operator=(PhysicsObject&& other) noexcept;


	void Update(float deltaTime);

	const Transform* GetTransform() const;
	Collider* GetCollider() { return m_Collider; };

	glm::vec2 GetPosition() const;
	glm::vec2 GetVelocity() const;

	void OffsetPosition(const glm::vec2& pos);
	void SetPosition(const glm::vec2& pos);
	void Accelerate(const glm::vec2& acc);

	float GetMass();
	float GetInverseMass();


private:
	Body* m_Body;
	Collider* m_Collider;
	Transform m_Transform;
};