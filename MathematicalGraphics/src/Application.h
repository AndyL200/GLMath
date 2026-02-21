#ifndef MGRAPH_APPLICATION
#define MGRAPH_APPLICATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "mpch.h"
#include <stb/stb_image.h>

#ifdef _WIN32
#include <windows.h>
#endif
#ifndef M_PI
#define M_PI 3.1415926535f
#endif

class Application {
	public:
		std::thread input_thread;
		Application() { this->primary = nullptr;};
		void run();
	private:
		GLFWmonitor* primary;
};


#endif