#include "GLFWWindow.h"
#include "Utils.h"
using namespace core::utils::gl;

#include <iostream>

GLFWWindow::GLFWWindow()
{
}

GLFWWindow::~GLFWWindow()
{
    glfwTerminate();
}

void GLFWWindow::execute()
{
    double previous_seconds = glfwGetTime ();
    int frame_count = 0;
	while (!glfwWindowShouldClose (this->window)) {
	     // update timers
		double current_seconds = glfwGetTime ();
		elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;
		_update_fps_counter (elapsed_seconds, frame_count);


        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->loop();
         // update other events like input handling
		glfwPollEvents ();

		this->input();

		if (GLFW_PRESS == glfwGetKey (this->window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose (this->window, 1);
		}
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers (this->window);
	}

}

void GLFWWindow::glfw_mouse_click_callback (GLFWwindow* window, int button, int action, int mods)
{
    std::cout << action << " " << button << " " << mods << "\n";
}
void GLFWWindow::init()
{
   log("starting GLFW %s", glfwGetVersionString());
   glfwSetErrorCallback (GLFWWindow::glfw_error_callback);
	if (!glfwInit ()) {
		fprintf (stderr, "ERROR: could not start GLFW3\n");
        return ;
	}
	 /* We must specify 3.2 core if on Apple OS X -- other O/S can specify
     anything here. I defined 'APPLE' in the makefile for OS X */
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow (
		width, height, "Extended Init.", NULL, NULL
	);
	if (!window) {
		fprintf (stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return;
	}
	glfwSetWindowSizeCallback (window, glfw_window_size_callback);
	glfwSetMouseButtonCallback (window, glfw_mouse_click_callback);
	glfwMakeContextCurrent (window);

	glfwWindowHint (GLFW_SAMPLES, 4);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit ();

	// get version info
	const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString (GL_VERSION); // version as a string

	log ("renderer: %s\nversion: %s\n", renderer, version);

    glEnable (GL_DEPTH_TEST); // enable depth-testing
	glEnable (GL_CULL_FACE); // cull face
	glCullFace (GL_BACK); // cull back face
	glFrontFace (GL_CCW); // GL_CCW for counter clock-wise
	///glClearColor (0.2, 0.2, 0.2, 1.0); // grey background to help spot mistakes
	glClearColor (0.0, 0.0, 0.0, 1.0);
	glViewport (0, 0, width, height);


	this->setup();
}
void GLFWWindow::glfw_error_callback (int error, const char* description) {
	fputs (description, stderr);
	log ("%s\n", description);
}

void GLFWWindow::glfw_window_size_callback (GLFWwindow* window, int width, int height) {


	printf ("width %i height %i\n", width, height);
	glViewport (0, 0, width, height);
	/* update any perspective matrices used here */
}



void GLFWWindow::_update_fps_counter(double elapsed_seconds, int &frame_count)
{
	if (elapsed_seconds > 0.01) {
		double fps = (double)frame_count / elapsed_seconds;
		char tmp[128];
		 sprintf (tmp, "opengl @ fps: %.2f", fps);
		 glfwSetWindowTitle (window, tmp);
		 frame_count = 0;
	}
	frame_count++;
}
