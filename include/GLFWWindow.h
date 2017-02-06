#ifndef GLFWWINDOW_H
#define GLFWWINDOW_H



#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

namespace core
{
namespace utils
{
namespace gl
{
class GLFWWindow
{
public:
    GLFWWindow();

    void init();
    void execute();
    virtual void loop() = 0;
    virtual ~GLFWWindow();

protected:
   static void glfw_error_callback (int error, const char* description);
   static void glfw_window_size_callback (GLFWwindow* window, int width, int height);
   static void glfw_mouse_click_callback (GLFWwindow* window, int button, int action, int mods);
   virtual void setup() = 0;
   virtual void input() = 0;

   GLFWwindow* window;
   double elapsed_seconds;

   int width = 600;
   int height = 800;

private:








   void _update_fps_counter(double elapsed_seconds, int &frame_count);
};
}
}
}

#endif // GLFWWINDOW_H
