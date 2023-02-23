#include "glm/glm.hpp"

#ifndef TRANSFORM_H_INCLUDED
#include "Engine/Physics/Transform.h"
#define TRANSFORM_H_INCLUDED
#endif

struct IntegratorObject
{

	virtual ~IntegratorObject() {};

	virtual void UpdatePosition(float deltaTime) = 0;

	virtual glm::vec2 GetPosition() const = 0;
	virtual glm::vec2 GetAcceleration() const = 0;
	virtual glm::vec2 GetVelocity() const = 0;

	virtual void Accelerate(const glm::vec2& acc) = 0;
	virtual void OffsetPosition(const glm::vec2& pos) = 0;
	virtual void SetPosition(const glm::vec2& pos) = 0;

	virtual void SetTransform(Transform& transform) = 0;

	virtual IntegratorObject* Clone() const = 0;
};