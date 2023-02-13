#pragma once
#include "GL.h"
#include "GLRenderer.h"
#include "Ball.h"

class Balls:
    public Application
{
    struct custom_comparator {
        bool operator()(std::pair<Ball&, Ball&> a,
                        std::pair<Ball&, Ball&> b) const
        {
            return (std::minmax(a.first.ID(), a.second.ID()) > std::minmax(b.first.ID(), b.second.ID()));
        }
    };

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

    std::set<std::pair<Ball&, Ball&>, custom_comparator> m_Pairs;
    std::vector<std::pair<Ball&, Ball&>> m_CollidingBalls;

    Ball* m_SelectedBall = nullptr;

    int m_ID;
};


