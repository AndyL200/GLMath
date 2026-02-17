#include "Player.h"



void Camera::setCameraUniform(const glm::mat4& perspective, const glm::mat4& view, Shader& s) {
	this->perspective = perspective;
	this->view = view;

	GLuint mvp_location = glGetUniformLocation(s.programID, "perspective");
	if (mvp_location != -1) {
		glm::mat4 mvp = perspective * view;
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));
	}
}

void ControlComponent::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (!scancode) {
		scancode = glfwGetKeyScancode(key);
	}

    if (key == this->left && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		// Handle left key press
		sendSignal(ControlSignals::LEFT);
	}
	else if (key == this->right && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		// Handle right key press
		sendSignal(ControlSignals::RIGHT);
	}
	else if (key == this->up && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		// Handle up key press
		sendSignal(ControlSignals::UP);
	}
	else if (key == this->down && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		// Handle down key press
		sendSignal(ControlSignals::DOWN);
	}
	else if (key == this->action && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		// Handle action key press
		sendSignal(ControlSignals::ACTION);
	}
	else {
		return;
	}
	const char* key_name = glfwGetKeyName(key, scancode);
	if (key_name) {
		std::cout << "Key " << key_name << " pressed" << std::endl;
	}
}

void Player::handleControlSignals() {

	if (signals & ControlSignals::LEFT) {
		// Handle left signal
		std::cout << "Left signal received" << std::endl;
		pos.x += speed;
	}
	if (signals & ControlSignals::RIGHT) {
		// Handle right signal
		std::cout << "Right signal received" << std::endl;
		pos.x -= speed;
	}
	if (signals & ControlSignals::UP) {
		// Handle up signal
		std::cout << "Up signal received" << std::endl;
		pos.z += speed;
	}
	if (signals & ControlSignals::DOWN) {
		// Handle down signal
		std::cout << "Down signal received" << std::endl;
		pos.z -= speed;
	}
	if (signals & ControlSignals::ACTION) {
		// Handle action signal
		std::cout << "Action signal received" << std::endl;
	}

	signals = 0;
}