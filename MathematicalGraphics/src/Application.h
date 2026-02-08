#ifndef MGRAPH_APPLICATION
#define MGRAPH_APPLICATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include "Shader.h"

#ifdef _WIN32
#include <windows.h>
#endif
#ifndef M_PI
#define M_PI 3.1415926535f
#endif

class Application {
	Application() { primary = nullptr; };

	public:
		std::thread input_thread;
		void run();
	private:
		GLFWmonitor* primary;
		std::unique_ptr<Shader> shader;
};


#endif