#ifndef MATHCURSOR
#define MATHCURSOR
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>


class CursorComponent {
public:
	void useCursor(GLFWimage& image, GLFWwindow* window) {
		GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);
		this->custom_cursor = cursor;
		glfwSetCursor(window, cursor);
	}
	double x = 0.0f, y = 0.0f;
	using CursorCallback = std::function<void(double, double, float)>;
	CursorComponent() : x(0), y(0) {}
	void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
		if (xpos == this->x && ypos == this->y) { return; }
		this->x = xpos;
		this->y = ypos;
		if (callback) {
			callback(xpos, ypos, 1.0f);
		}
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glfwSetCursorPos(window, width / 2.f, height / 2.f);
	}
	void set_callback(std::function<void(double, double, float)> f) { this->callback = f; }
private:
	CursorCallback callback;
	GLFWcursor* custom_cursor;
};

#endif