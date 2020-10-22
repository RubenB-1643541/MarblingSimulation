#include "pch.h"
#include "WindowBase.h"

namespace RenderEngine {

    WindowBase::WindowBase(std::string name) : b_closing(false)
    {
        /* Initialize the library */
        if (!glfwInit())
            std::cerr << "Could not create OpenGL context" << std::endl;

        glfwWindowHint(GLFW_SAMPLES, 4);//Give framebuffer 4 samples per pixel -> Anti Aliasing

        /* Create a windowed mode window and its OpenGL context */
        _window = glfwCreateWindow(640, 480, name.c_str(), NULL, NULL);
        if (!_window)
        {
            glfwTerminate();
            std::cerr << "Could not create Window" << std::endl;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(_window);

        GLenum err = glewInit();
        if (GLEW_OK != err)
        {
            std::cerr << "Glew Error: " << glewGetErrorString(err) << std::endl;
        }

        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);

        glfwSetWindowUserPointer(_window, (void*)&_props);

        glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            Properties* props = (Properties*)glfwGetWindowUserPointer(window);
            if (action == GLFW_PRESS) {
                KeyPressEvent e(key);
                props->callback(e);
            }
            else if (action == GLFW_RELEASE) {
                KeyReleaseEvent e(key);
                props->callback(e);
            }
        });

        glfwSetCharCallback(_window, [](GLFWwindow* window, unsigned int character) {
            Properties* props = (Properties*)glfwGetWindowUserPointer(window);
            KeyTypeEvent e(character);
            props->callback(e);
        });

        glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xpos, double ypos) {
            Properties* props = (Properties*)glfwGetWindowUserPointer(window);
            MouseMoveEvent e(xpos, ypos);
            props->callback(e);
        });

        glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
            Properties* props = (Properties*)glfwGetWindowUserPointer(window);
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            if (action == GLFW_PRESS) {
                MousePressEvent e(button, xpos, ypos);
                props->callback(e);
            }
            else if (action == GLFW_RELEASE) {
                MouseReleaseEvent e(button, xpos, ypos);
                props->callback(e);
            }
        });

        glfwSetScrollCallback(_window, [](GLFWwindow* window, double xoffset, double yoffset) {
            Properties* props = (Properties*)glfwGetWindowUserPointer(window);
            MouseScrollEvent e(xoffset, yoffset);
            props->callback(e);
        });

        glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
            Properties* props = (Properties*)glfwGetWindowUserPointer(window);
            props->width = width;
            props->height = height;
            WindowResizeEvent e(width, height);
            props->callback(e);
        });

        glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
            Properties* props = (Properties*)glfwGetWindowUserPointer(window);
            WindowCloseEvent e;
            props->callback(e);
        });
        LoadDefaultCursors();
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback([](GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar* message,
            const void* userParam) 
        {
            fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
                (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
                type, severity, message);
        }, 0);
    }

    WindowBase::~WindowBase()
    {
        glfwDestroyWindow(_window);
    }

    void WindowBase::Start()
    {
        OnCreate();
    }

    void WindowBase::Update()
    {
        OnUpdate();
        /* Poll for and process events */
        glfwPollEvents();

        if (glfwWindowShouldClose(_window))
            b_closing = true;
    }

    void WindowBase::StartDraw()
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        OnDraw();
    }

    void WindowBase::EndDraw()
    {

        /* Swap front and back buffers */
        glfwSwapBuffers(_window);
    }

    void WindowBase::Destroy()
    {
        glfwTerminate();
    }

    void WindowBase::SetTitle(std::string name)
    {
        glfwSetWindowTitle(_window, name.c_str());
    }

    void WindowBase::SetBackgroundColor(float R, float G, float B, float A)
    {
        glClearColor(R, G, B, A);
    }
    /*
    void WindowBase::SetIcon(std::string& icon, std::string& smallIcon)
    {
        GLFWimage images[2];
        images[0] = load_icon()
    }
    */
    void WindowBase::SetSize(int width, int height)
    {
        glfwSetWindowSize(_window, width, height);
    }

    int WindowBase::GetWidth()
    {
        return _props.width;
    }

    int WindowBase::GetHeight()
    {
        return _props.height;
    }

    void WindowBase::Minimize()
    {
        glfwIconifyWindow(_window);
    }

    void WindowBase::UndoMinimize()
    {
        glfwRestoreWindow(_window);
    }

    void WindowBase::Maximize()
    {
        glfwMaximizeWindow(_window);
    }

    void WindowBase::UndoMaximize()
    {
        glfwRestoreWindow(_window);
    }

    void WindowBase::Hide(bool b)
    {
        if (b)
            glfwHideWindow(_window);
        else
            glfwShowWindow(_window);
    }

    void WindowBase::FullScreen()
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwSetWindowMonitor(_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }

    void WindowBase::ExitFullScreen()
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwSetWindowMonitor(_window, NULL, 320, 240, 640, 480, mode->refreshRate);
    }

    void WindowBase::UnableDepthTest()
    {
        // Enable depth test
        glEnable(GL_DEPTH_TEST);
    }

    void WindowBase::DisableDepthTest()
    {
        // Enable depth test
        glDisable(GL_DEPTH_TEST);
    }

    void WindowBase::SetDepthFunction(GLenum func)
    {
        glDepthFunc(func);
    }

    bool WindowBase::isClosing()
    {
        return b_closing;
    }

    void WindowBase::SetCursor(std::string name)
    {
        auto it = _cursors.find(name);
        if (it == _cursors.end())
            std::cerr << "Cursor with name " << name << " Does not exist" << std::endl;
        else
            glfwSetCursor(_window, it->second.cursorPtr);
    }

    void WindowBase::SetCursor(Cursor cursor)
    {
        auto it = _cursors.find(cursor.name);
        if (it == _cursors.end())
            CreateCursor(cursor);
        glfwSetCursor(_window, _cursors.find(cursor.name)->second.cursorPtr);
    }

    void WindowBase::CreateCursor(Cursor cursor)
    {
        auto it = _cursors.find(cursor.name);
        if (it != _cursors.end()) {
            std::cerr << "Cursor with name: " << cursor.name << "Already Exists" << std::endl;
        }
        else {
            GLFWimage image;
            image.width = cursor.image.width;
            image.height = cursor.image.height;
            image.pixels = cursor.image.data;

            GLFWcursor* glfwcursor = glfwCreateCursor(&image, cursor.centerx, cursor.centery);
            WindowCursor wincursor = { cursor, glfwcursor };
            _cursors.insert(std::pair<std::string, WindowCursor>(cursor.name, wincursor));
        }
    }

    void WindowBase::CreateCursor(Cursor cursor, std::string name)
    {
        auto it = _cursors.find(name);
        if (it != _cursors.end()) {
            std::cerr << "Cursor with name: " << name << "Already Exists" << std::endl;
        }
        else {
            GLFWimage image;
            image.width = cursor.image.width;
            image.height = cursor.image.height;
            image.pixels = cursor.image.data;

            GLFWcursor* glfwcursor = glfwCreateCursor(&image, 8, 8);
            WindowCursor wincursor = { cursor, glfwcursor };
            _cursors.insert(std::pair<std::string, WindowCursor>(name, wincursor));
        }
    }

    void WindowBase::SetDefaultCursor()
    {
        glfwSetCursor(_window, nullptr);
    }

    void WindowBase::HideCursor(bool state)
    {
        if (state)
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        else
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void WindowBase::LoadDefaultCursors()
    {
        Cursor cursor;
        GLFWcursor* glfwcursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        WindowCursor wincursorarrow = { cursor, glfwcursor };
        _cursors.insert(std::pair<std::string, WindowCursor>(CURSOR_ARROW, wincursorarrow));

        glfwcursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
        WindowCursor wincursoribeam = { cursor, glfwcursor };
        _cursors.insert(std::pair<std::string, WindowCursor>(CURSOR_IBEAM, wincursoribeam));

        glfwcursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
        WindowCursor wincursorcrosshair = { cursor, glfwcursor };
        _cursors.insert(std::pair<std::string, WindowCursor>(CURSOR_CROSSHAIR, wincursorcrosshair));

        glfwcursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
        WindowCursor wincursorhand = { cursor, glfwcursor };
        _cursors.insert(std::pair<std::string, WindowCursor>(CURSOR_HAND, wincursorhand));

        glfwcursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
        WindowCursor wincursorhresize = { cursor, glfwcursor };
        _cursors.insert(std::pair<std::string, WindowCursor>(CURSOR_HRESIZE, wincursorhresize));

        glfwcursor = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
        WindowCursor wincursorvresize = { cursor, glfwcursor };
        _cursors.insert(std::pair<std::string, WindowCursor>(CURSOR_VRESIZE, wincursorvresize));
    }

}