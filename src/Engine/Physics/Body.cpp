#pragma once
#include "Engine/Physics/Body.h"

#include "Engine/Physics/Integrators/VerletObject.h"


#ifndef TRANSFORM_H_INCLUDED
#include "Engine/Physics/Transform.h"
#define TRANSFORM_H_INCLUDED
#endif

Body::Body(const BodyDefinition& BodyDef, Transform& transform)
	:m_MassData(MassData(BodyDef.m_Mass, BodyDef.m_InvMass)), m_Object(new VerletObject(&transform))
{
}

Body::~Body()
{
	delete m_Object;
}

Body::Body(const Body& other)
{
	m_MassData = other.m_MassData;
	m_Object = other.m_Object->Clone();
}

Body Body::operator=(const Body& other)
{
	m_MassData = other.m_MassData;
	m_Object = other.m_Object->Clone();

	return *this;
}

Body::Body(Body&& other) noexcept
{
	m_MassData = other.m_MassData;
	m_Object = other.m_Object;

	other.m_Object = nullptr;
}

Body Body::operator=(Body&& other) noexcept
{
	m_MassData = other.m_MassData;
	m_Object = other.m_Object;

	other.m_Object = nullptr;

	return *this;
}

void Body::UpdateMass(float mass)
{
	m_MassData.m_Mass = mass;

	if (mass == 0)
	{
		m_MassData.m_InvMass = 0;
	}
	else
	{
		m_MassData.m_InvMass = 1.0f / m_MassData.m_Mass;
	}
}

void Body::SetTransform(Transform& transform)
{
	m_Object->SetTransform(transform);
}

void Body::Update(float deltaTime)
{
	m_Object->UpdatePosition(deltaTime);
}

void Body::Accelerate(const glm::vec2& acc)
{
	m_Object->Accelerate(acc);
}

void Body::SetPosition(const glm::vec2& pos)
{
	m_Object->SetPosition(pos);
}

void Body::SetVelocity(const glm::vec2& vel)
{
	m_Object->SetVelocity(vel);
}

glm::vec2 Body::GetPosition() const
{
	return m_Object->GetPosition();
}

glm::vec2 Body::GetAcceleration() const
{
	return m_Object->GetAcceleration();
}

glm::vec2 Body::GetVelocity() const
{
	return m_Object->GetVelocity();
}
