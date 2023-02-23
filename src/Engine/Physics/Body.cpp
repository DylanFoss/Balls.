#pragma once
#include "Engine/Physics/Body.h"

#include "Engine/Physics/Integrators/VerletObject.h"


#ifndef TRANSFORM_H_INCLUDED
#include "Engine/Physics/Transform.h"
#define TRANSFORM_H_INCLUDED
#endif

Body::Body(const BodyDefinition& BodyDef, Transform& transform)
	:m_MassData(MassData(BodyDef.m_Mass, BodyDef.m_InvMass)), m_Object(new VerletObject(transform.m_Position))
{
}

Body::~Body()
{
	delete m_Object;
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
