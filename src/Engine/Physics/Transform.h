#include "glm/glm.hpp"
/*
* A struct for storing information of where an object is in the world.
* Currently just stores the position, but will support rotation.
*/
struct Transform
{
	Transform(const glm::vec2& position) 
		: m_Position(position) 
	{}

	glm::vec2 m_Position;
};