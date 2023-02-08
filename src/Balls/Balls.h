#pragma once
#include "GL.h"
#include "GLRenderer.h"
#include "Ball.h"

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

    OrthographicCameraController camera = OrthographicCameraController(400.0f);

    bool m_IsPaused = false;

    std::vector<Ball> m_Balls;
    std::vector<std::pair<Ball*, Ball*>> m_CollidingBalls;

    Ball* m_SelectedBall = nullptr;


   // glm::vec2 GetGameCell(glm::vec2 worldPosition);
};


