#pragma once
#include "GL.h"
#include "GLUtils.h"

#include "Ball.h"

struct Vertex
{
    glm::vec2 Position;
    glm::vec2 TexCoord;
};

class Balls:
    public Application
{
public:
    Balls(const std::string& name, uint32_t width, uint32_t height);
    ~Balls();

    void Init();
    void Shutdown();

    void Update(float deltatime);
    void Draw(float deltatime);

private:

    int32_t m_WindowHalfWidth;
    int32_t m_WindowHalfHeight;

    Utils::OrthographicCameraController camera = Utils::OrthographicCameraController(400.0f);

    bool m_IsPaused = false;

    std::vector<Ball> m_Balls;
    std::vector<Vertex> m_Vertices;

    GLuint VA = 0;
    GLuint VB = 0;
    GLuint IB = 0;

    size_t m_MaxBallCount = 100;
    size_t m_MaxVertexCount = m_MaxBallCount * 4;
    size_t m_MaxIndexCount = m_MaxBallCount * 6;

   // glm::vec2 GetGameCell(glm::vec2 worldPosition);
};


