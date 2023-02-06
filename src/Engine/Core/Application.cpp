#include "pch.h"
#include "Application.h"

#include <chrono>

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& name, uint32_t width, uint32_t height)
{
	if (!s_Instance)
		s_Instance = this;

	m_Input = std::unique_ptr<Input>(new Input());

	m_Window = std::unique_ptr<Window>(Window::Create({ name, width, height }));
	m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
}

int Application::Run()
{
	std::chrono::time_point <std::chrono::high_resolution_clock> t1, t2;
	t1 = std::chrono::high_resolution_clock::now();
	float windowCount = 0.0f;
	float avgfps = 0;

	while (m_Running)
	{
		t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsedTime = t2 - t1;
		float deltaTime = (float)elapsedTime.count();

		Update(deltaTime);
		Draw(deltaTime);

		m_Input->InputReset();

		m_Window->OnUpdate();
		t1 = t2;

		float fps = 1.0f / deltaTime;
		avgfps = 0.9f * avgfps + (1.0f - 0.9f) * fps;
		float drawTime = deltaTime * 1000;

		windowCount += deltaTime;

		if (windowCount > 0.1f)
		{
			std::stringstream ss;
			ss.precision(4);
			ss << "Balls. - [ Avg. FPS: " << avgfps << ", " << "Draw Time: " << drawTime << " ms " << ']' << '\n';
			glfwSetWindowTitle((GLFWwindow*)m_Window.get()->GetNativeWindow(), ss.str().c_str());
			windowCount = 0;
		}
	}

	return 0;
}

void Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}
