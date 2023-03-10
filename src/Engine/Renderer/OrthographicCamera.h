#pragma once

#include <glm/glm.hpp>

class OrthographicCamera
{
public:

	OrthographicCamera(float left, float right, float bottom, float top);

	void SetProjection(float left, float right, float bottom, float top);

	const glm::vec3& GetPosition() { return m_Position; }
	void SetPosition(const glm::vec3& position) { m_Position = position; CalculateViewMatrix(); }

	const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
	const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
	const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

private:

	void CalculateViewMatrix();

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ViewProjectionMatrix;

	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	
};

