#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Application.h"


int main() {
	std::unique_ptr<Application> app;
	app->run();
	

	return 0;
}