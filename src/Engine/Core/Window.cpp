#include "Window.h"
#include "Events/WindowEvent.h"
#include "Events/KeyEvent.h"

#include "Input.h"

#include <iostream>

Window::Window(const WindowProps& data)
{
    Init(data);
}

Window::~Window()
{
    Shutdown();
}

void Window::Shutdown()
{
    glfwDestroyWindow(m_Window);
}

void Window::Init(const WindowProps& data)
{
    if (!glfwInitialised)
    {
        int initialised = glfwInit();
        if (initialised)
            glfwInitialised = true;
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    /*
    * Correct window size based on input, if the content size is larger than the monitor, screenspace conversions will not work as intended. 
    * Also considers the windows title size, which can offset the content offscreen.
    */
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    m_Data.Title = data.Title;
    m_Data.Height = data.Height;
    m_Data.Width = data.Width;

    m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
    if (!m_Window)
    {
        glfwTerminate();
        glfwInitialised = false;
    }

    //check created window isn't larger than the monitor, if it is, change the size.
    int left, top, right, bottom;
    glfwGetWindowFrameSize(m_Window, &left, &top, &right, &bottom);

    //not shifing by 1 causes inaccuracies at higher game sizes at fullscreen; I suspect the window is 1 pixel off screen. Further confirmation needed.
    m_Data.Title = data.Title;
    m_Data.Height = data.Height > mode->height-(top+1) ? mode->height-(top+1) : data.Height;
    m_Data.Width = data.Width > mode->width ? mode->width : data.Width;

    glfwSetWindowSize(m_Window, m_Data.Width, m_Data.Height);

    //move the window so it's in the top left corner of the monitor byu default.
    glfwSetWindowPos(m_Window, 0, 0 + top);

    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVsync(true);

    //set resize / input callbacks here

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		WindowCloseEvent event;
		data.EventCallback(event);
	});

    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        data.Width = width;
        data.Height = height;

        WindowResizeEvent event(width, height);
        data.EventCallback(event);
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        //hack, we are only care about ascii codes atm
        if (key > 255)
            return;

        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        // Input Polling
        switch (action)
        {
            case GLFW_PRESS:
            {
                Input::Get().KeysDown(key);
                KeyPressedEvent e(key);
                data.EventCallback(e);
                break;
            }
            case GLFW_RELEASE:
            {
                Input::Get().KeysUp(key);
                break;
            }
        }
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
    {
        switch (action)
        {
        case(GLFW_RELEASE):
            Input::Get().MouseButtonsUp(button);
            break;
        case(GLFW_PRESS):
            Input::Get().MouseButtonsDown(button);
            break;
        }
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
    {
         Input::Get().MousePosition(xpos, ypos);
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
    {
        Input::Get().MouseScroll(yoffset);
    });
}

void Window::OnUpdate()
{
    /* Swap front and back buffers */
    glfwSwapBuffers(m_Window);
    /* Poll for and process events */
    glfwPollEvents();
}

void Window::SetVsync(bool enable)
{
    if (enable)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    m_Data.vsync = enable;
}

bool Window::IsVsync() const
{
    return m_Data.vsync;
}

Window* Window::Create(const WindowProps& data)
{
    return new Window(data);
}
