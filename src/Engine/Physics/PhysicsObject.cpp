#include "PhysicsObject.h"

#include "Engine/Physics/Body.h"
#include "Engine/Physics/Collision/Collider.h"

PhysicsObject::PhysicsObject(Collider* collider, const BodyDefinition& body)
	:m_Transform(Transform(body.m_Position)), m_Collider(collider)
{
	m_Body = new Body(body, m_Transform);
	m_Body->UpdateMass(m_Collider->CalculateMass());
}

PhysicsObject::~PhysicsObject()
{
	delete m_Body;
	delete m_Collider;
}

PhysicsObject::PhysicsObject(const PhysicsObject& other)
	:m_Transform(other.m_Transform)
{
	m_Body = new Body(*other.m_Body);
	m_Body->SetTransform(m_Transform);
	m_Collider = other.m_Collider->Clone();
}

PhysicsObject PhysicsObject::operator=(const PhysicsObject& other)
{
	m_Transform = other.m_Transform;
	m_Body = new Body(*other.m_Body);
	m_Collider = other.m_Collider->Clone();

	return *this;
}

PhysicsObject::PhysicsObject(PhysicsObject&& other) noexcept
	:m_Transform(std::move(other.m_Transform))
{
	m_Body = other.m_Body;
	m_Collider = other.m_Collider;

	other.m_Body = nullptr;
	other.m_Collider = nullptr;
}

PhysicsObject PhysicsObject::operator=(PhysicsObject&& other) noexcept
{
	m_Transform = std::move(other.m_Transform);
	m_Body = other.m_Body;
	m_Collider = other.m_Collider;

	other.m_Body = nullptr;
	other.m_Collider = nullptr;

	return *this;
}

void PhysicsObject::Update(float deltaTime)
{
	m_Body->Update(deltaTime);
}

const Transform* PhysicsObject::GetTransform() const
{
	return &m_Transform;
}

glm::vec2 PhysicsObject::GetPosition() const
{
	return m_Body->GetPosition();
}

glm::vec2 PhysicsObject::GetVelocity() const
{
	return m_Body->GetVelocity();
}

void PhysicsObject::OffsetPosition(const glm::vec2& pos)
{
	m_Transform.m_Position += pos;
}

void PhysicsObject::SetPosition(const glm::vec2& pos)
{
	m_Transform.m_Position = pos;
}

void PhysicsObject::SetVelocity(const glm::vec2& vel)
{
	m_Body->SetVelocity(vel);
}

void PhysicsObject::Accelerate(const glm::vec2& acc)
{
	m_Body->Accelerate(acc);
}

float PhysicsObject::GetMass()
{
	return m_Body->GetMass();
}

float PhysicsObject::GetInverseMass()
{
	return m_Body->GetInverseMass();
}
