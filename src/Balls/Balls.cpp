#include "Balls.h"
#include "pch.h"
#include <fstream>

#include "Core//MouseCodes.h"
#include "Core//KeyCodes.h"

#include "GLErrorHandler.h"

Balls::Balls(const std::string& name, uint32_t width, uint32_t height)
	:Application(name, width, height), m_WindowHalfHeight(m_Window->GetHeight() * 0.5f), m_WindowHalfWidth(m_Window->GetWidth() * 0.5f)
{
	float windowAspect = static_cast<float>(m_Window->GetWidth()) / static_cast<float>(m_Window->GetHeight());
	//float multiplier = m_GameAspect / windowAspect;

	camera = Utils::OrthographicCameraController(m_WindowHalfWidth, m_WindowHalfHeight);
    //camera.SetMaxZoom(10.0f / static_cast<float>(gameHeight));

	Init();
}

Balls::~Balls()
{
	Shutdown();
}

struct Vertex
{
	glm::vec2 Position;
	glm::vec2 TexCoord;
};

Utils::Shader shader;
glm::mat4 model;
Vertex verticies[] = {
    {{-100, -100}, {0.0f, 0.0f}},
	{{ 100, -100}, {1.0f, 0.0f}},
	{{ 100,  100}, {1.0f, 1.0f}},
	{{-100,  100}, {0.0f, 1.0f}}
};

void Balls::Init()
{
	if (glewInit() != GLEW_OK)
		std::cout << "ERROR!" << std::endl;

	std::cout << glGetString(GL_VERSION) << '\n';

	{
		GLint size;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
		std::cout << "Max texture size : " << size << '\n';
	}

	m_Window.get()->SetVsync(true);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glClearColor(0.0f, 0.0f, 0.20f, 1.0f);

	//openGL
	GLCall(glGenVertexArrays(1, &VA));
	GLCall(glBindVertexArray(VA));

	GLCall(glCreateBuffers(1, &VB));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VB));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));

	GLCall(glEnableVertexArrayAttrib(VA, 0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position)));

	GLCall(glEnableVertexArrayAttrib(VA, 1));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoord)));

	std::vector<unsigned int> indices;
	indices.reserve(6);
	for (int i = 0; i < 1; i += 6)
	{
		indices.emplace_back(0);
		indices.emplace_back(1);
		indices.emplace_back(2);
		indices.emplace_back(2);
		indices.emplace_back(3);
		indices.emplace_back(0);
	}

	GLCall(glCreateBuffers(1, &IB));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW));

	model = glm::mat4(1.0f);
	glm::mat4 mvp = camera.GetCamera().GetViewProjectionMatrix() * model;
	shader.CreateShader("basic.vert.shader", "basic.frag.shader");
	shader.Bind();
	GLCall(glUniformMatrix4fv(glGetUniformLocation(shader.ID(), "u_MVP"), 1, GL_FALSE, &mvp[0][0]));
}

void Balls::Shutdown()
{
	//delete shaders

	//destructors handle this

	//delete textures
}

/*
* This section updates the texture by drawing the state texture (initally noise) to a gamespace quad using the GoL shader, which updates the state accordingly.
* This is performed on buffer object so the texture can be bound to the other texture of the same size.
* Each time this operation is perforemd to the other texture, allowing for timed state updates.
*/
void Balls::Update(float deltaTime)
{
	camera.Update(deltaTime);

}

/*
* This section simpy draws the most recent state texture to a screenspace quad
*/
void Balls::Draw(float deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT);

	GLsizeiptr size = sizeof(verticies);
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VB));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, verticies));

	GLCall(glBindVertexArray(VA));
	GLCall(glBindVertexArray(VB));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB));
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

//glm::vec2 Balls::GetGameCell(glm::vec2 worldPosition)
//{
//	return glm::vec2(
//		std::floor((worldPosition.x) / ((m_Window->GetWidth() * m_GameAspectRatioMultipliers.x) / static_cast<float>(m_GameWidth))) + m_GameWidth * 0.5f,
//		std::floor((worldPosition.y) / ((m_Window->GetHeight() * m_GameAspectRatioMultipliers.y) / static_cast<float>(m_GameHeight))) + m_GameHeight * 0.5f
//	);
//}
