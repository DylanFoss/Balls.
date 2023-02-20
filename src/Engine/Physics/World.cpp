#include "World.h"

#include "PhysicsObject.h"

#include "Engine/Physics/Shapes/Circle.h"
#include "Engine/Physics/Body.h"

#include "Engine/Physics/Shapes/ShapeType.h"

World::World(const glm::vec2& gravity)
	:m_Gravity(gravity), m_StepDuration(0.00694f)
{
}

World::~World()
{
	for (PhysicsObject* obj : m_Physics)
		delete obj;
	m_Physics.clear();
}

void World::Update(float deltaTime)
{
	m_StepTime += deltaTime;

	if (m_StepTime > m_StepDuration)
	{
		ApplyGravity();
		ApplyConstraints();
		SolveCollisions();
		UpdatePositions(m_StepDuration);
		m_StepTime = 0;
	}
}

void World::UpdatePositions(float deltaTime)
{
	for (PhysicsObject* obj : m_Physics)
		obj->Update(deltaTime);
}

void World::ApplyGravity()
{
	for (PhysicsObject* obj : m_Physics)
		obj->Accelerate(m_Gravity);
}

void World::ApplyConstraints()
{
	for (PhysicsObject* obj : m_Physics)
	{
		const glm::vec2 toObj = obj->GetPosition() - glm::vec2{ 0.f, 0.f };
		const float dist = glm::length(toObj);
		if (dist > 300.f - 40.f)
		{
			obj->SetPosition((toObj/dist) * (300.f - 40.f));
		}
	}
}

//to be seperated into their own classes, but fun to mess around with now!
void World::SolveCollisions()
{
	for (int i = 0; i < m_Physics.size(); i++)
	{
		PhysicsObject* lhs = m_Physics[i];
		for (int j = i + 1; j < m_Physics.size(); j++)
		{
			PhysicsObject* rhs = m_Physics[j];
			glm::vec2 collisionAxis = lhs->GetPosition() - rhs->GetPosition();
			float distance = glm::length(collisionAxis);
			if (distance < 80.f)
			{
				glm::vec2 collisionNormal = collisionAxis / distance;
				float overlap = 80.0f - distance;

				glm::vec2 offset = 0.5f * overlap * collisionNormal;

				lhs->OffsetPosition(offset);
				rhs->OffsetPosition(-offset);
			}
		}
	}
}

void World::CreatePhysicsObject(const glm::vec2 pos, Shape* shape)
{
	Shape* concreteShape;
	switch (shape->GetType())
	{
	case(ShapeType::CIRCLE):
		concreteShape = static_cast<Circle*>(shape);
		break;
	case(ShapeType::BOX):
		break;
	default:
		std::cout << "No valid type provided." << '\n';
		break;
	}

	BodyDefinition bd;
	bd.m_Position = pos;

	m_Physics.emplace_back(new PhysicsObject(shape, bd));
}

void World::CreateBall(const glm::vec2 pos, float radius)
{
	BodyDefinition bd;
	bd.m_Position = pos;

	Circle* c = new Circle(radius);
	m_Physics.emplace_back(new PhysicsObject(c, bd));
}
