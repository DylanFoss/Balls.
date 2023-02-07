#include "Balls.h"
#include "pch.h"
#include <fstream>

#include "Core/MouseCodes.h"
#include "Core/KeyCodes.h"

#include "GLErrorHandler.h"

Balls::Balls(const std::string& name, uint32_t width, uint32_t height)
	:Application(name, width, height), m_WindowHalfHeight(m_Window->GetHeight() * 0.5f), m_WindowHalfWidth(m_Window->GetWidth() * 0.5f)
{
	float windowAspect = static_cast<float>(m_Window->GetWidth()) / static_cast<float>(m_Window->GetHeight());
	//float multiplier = m_GameAspect / windowAspect;

	camera = OrthographicCameraController(m_WindowHalfWidth, m_WindowHalfHeight);
    //camera.SetMaxZoom(10.0f / static_cast<float>(gameHeight));

	Init();
}

Balls::~Balls()
{
	Shutdown();
}

Shader shader;
glm::mat4 model;

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

	m_Window.get()->SetVsync(false);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glClearColor(0.0f, 0.0f, 0.20f, 1.0f);

	//openGL
	GLCall(glGenVertexArrays(1, &VA));
	GLCall(glBindVertexArray(VA));

	GLCall(glCreateBuffers(1, &VB));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VB));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_MaxVertexCount, nullptr, GL_DYNAMIC_DRAW));

	GLCall(glEnableVertexArrayAttrib(VA, 0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position)));

	GLCall(glEnableVertexArrayAttrib(VA, 1));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoord)));

	std::vector<unsigned int> indices;
	indices.reserve(m_MaxIndexCount);
	int offset = 0;
	for (int i = 0; i < m_MaxIndexCount; i += 6)
	{
		indices.emplace_back(0 + offset);
		indices.emplace_back(1 + offset);
		indices.emplace_back(2 + offset);
		indices.emplace_back(2 + offset);
		indices.emplace_back(3 + offset);
		indices.emplace_back(0 + offset);
		offset += 4;
	}

	GLCall(glCreateBuffers(1, &IB));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW));

	model = glm::mat4(1.0f);
	glm::mat4 mvp = camera.GetCamera().GetViewProjectionMatrix() * model;
	shader.CreateShader("basic.vert.shader", "basic.frag.shader");
	shader.Bind();
	GLCall(glUniformMatrix4fv(glGetUniformLocation(shader.ID(), "u_MVP"), 1, GL_FALSE, &mvp[0][0]));

	// end of opengl

	m_Balls.reserve(100);
	for (int i = 0; i < 100; i++)
	{
		m_Balls.push_back(Ball(rand() % 800 - 399, rand() % 800 - 399, rand() % 40 + 21));
	}
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
	auto distance = [](glm::vec2 point1, glm::vec2 point2)
	{
		return glm::sqrt(((point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y)));
	};

	auto distanceSqr = [](glm::vec2 point1, glm::vec2 point2)
	{
		return (point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y);
	};


	auto circleOverlap = [&](Ball& ballA, Ball& ballB)
	{
		return fabs(distanceSqr(ballA.Position(), ballB.Position()) <= (ballA.Radius() + ballB.Radius()) * (ballA.Radius() + ballB.Radius()));
	};

	camera.Update(deltaTime);

	if (Input::Get().IsMouseHeld(KC_MOUSE_BUTTON_LEFT))
	{
		glm::vec2 pos = { Input::Get().GetMousePos().first, Input::Get().GetMousePos().second };
		m_Balls[0].SetPosition(camera.ScreenToWorldSpace(pos));
	}

	for (int i = 0; i < m_Balls.size(); i++)
	{
		m_Balls[i].Update(deltaTime);
	}

	std::vector<std::pair<Ball*, Ball*>> collidingBalls;

	// check collisions

	for (int i = 0; i < m_Balls.size(); i++)
	{
		for (int j = 0; j < m_Balls.size(); j++)
		{
			if (j != i)
			{
				if (circleOverlap(m_Balls[i], m_Balls[j]))
					collidingBalls.push_back(std::pair<Ball*, Ball*>(&m_Balls[i], &m_Balls[j]));
			}
		}
	}

	// resolve collisions

	for (auto pair : collidingBalls)
	{
		Ball* ball = pair.first;
		Ball* target = pair.second;

		float fDistance = distance(ball->Position(), target->Position());
		float fOverlap = 0.5f * (fDistance - ball->Radius() - target->Radius());

		ball->SetPosX(ball->PosX() - fOverlap * (ball->PosX() - target->PosX()) / fDistance);
		ball->SetPosY(ball->PosY() - fOverlap * (ball->PosY() - target->PosY()) / fDistance);

		target->SetPosX(target->PosX() + fOverlap * (ball->PosX() - target->PosX()) / fDistance);
		target->SetPosY(target->PosY() + fOverlap * (ball->PosY() - target->PosY()) / fDistance);
	}

}

/*
* This section simpy draws the most recent state texture to a screenspace quad
*/
void Balls::Draw(float deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT);

	m_Vertices.reserve(m_Balls.size() * 4);
	glm::vec2 ballPos = { 0,0 };
	float ballRad = 0;
	for (Ball &ball : m_Balls)
	{
		ballPos = ball.Position();
		ballRad = ball.Radius();

		m_Vertices.push_back({ {ballPos.x - ballRad, ballPos.y - ballRad}, {0.0f, 0.0f} });
		m_Vertices.push_back({ {ballPos.x + ballRad, ballPos.y - ballRad}, {1.0f, 0.0f} });
		m_Vertices.push_back({ {ballPos.x + ballRad, ballPos.y + ballRad}, {1.0f, 1.0f} });
		m_Vertices.push_back({ {ballPos.x - ballRad, ballPos.y + ballRad}, {0.0f, 1.0f} });
	}
	
	// copy CPU side memory to GPU
	GLsizeiptr size = m_Vertices.size() * sizeof(Vertex);
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VB));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, &m_Vertices[0]));

	shader.Bind();
	glm::mat4 mvp = camera.GetCamera().GetViewProjectionMatrix() * model;
	GLCall(glUniformMatrix4fv(glGetUniformLocation(shader.ID(), "u_MVP"), 1, GL_FALSE, &mvp[0][0]));

	GLCall(glBindVertexArray(VA));
	GLCall(glBindVertexArray(VB));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB));
	GLCall(glDrawElements(GL_TRIANGLES, m_MaxIndexCount, GL_UNSIGNED_INT, nullptr));

	m_Vertices.clear();
}