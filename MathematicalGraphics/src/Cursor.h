#ifndef MATHCURSOR
#define MATHCURSOR
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class CursorComponent {
public:
	double x = 0.0f, y = 0.0f;
	using CursorCallback = std::function<void(double, double)>;
	CursorComponent() : x(0), y(0) {}
	void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
		if (xpos == this->x && ypos == this->y) { return; }
		this->x = xpos;
		this->y = ypos;
		if (callback) {
			callback(xpos, ypos);
		}
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glfwSetCursorPos(window, width / 2.f, height / 2.f);
	}
private:
	CursorCallback callback;
};

#endif