#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

void Camera::Matrix(Shader& s) {
	glm::mat4 view = glm::lookAt(pos, pos + orientation, up);

	GLuint mvp_location = glGetUniformLocation(s.programID, "perspective");
	if (mvp_location != -1) {
		glm::mat4 mvp = this->perspective * view;
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));
	}
}
void Camera::Matrix(Shader& s, glm::mat4& perspective) {
	glm::mat4 view = glm::lookAt(pos, pos + orientation, up);
	this->perspective = perspective;
	// Width and height should be set separately via Camera members

	GLuint mvp_location = glGetUniformLocation(s.programID, "perspective");
	if (mvp_location != -1) {
		glm::mat4 mvp = this->perspective * view;
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
		setPosition(pos.x - speed, pos.y, pos.z);
	}
	if (signals & ControlSignals::RIGHT) {
		// Handle right signal
		std::cout << "Right signal received" << std::endl;
		setPosition(pos.x + speed, pos.y, pos.z);
	}
	if (signals & ControlSignals::UP) {
		// Handle up signal
		std::cout << "Up signal received" << std::endl;
		setPosition(pos.x, pos.y, pos.z + speed);
	}
	if (signals & ControlSignals::DOWN) {
		// Handle down signal
		std::cout << "Down signal received" << std::endl;
		setPosition(pos.x, pos.y, pos.z - speed);
	}
	if (signals & ControlSignals::ACTION) {
		// Handle action signal
		std::cout << "Action signal received" << std::endl;
	}

	signals = 0;
}

void Player::handleCursorChange(double xpos, double ypos, float sensitivity) {
	int height = cam->height;
	int width = cam->width;
	
	float rotx = sensitivity * (float)(ypos - (height / 2)) / height;
	float roty = sensitivity * (float)(xpos - (width / 2)) / width;

	// Vertical rotation (pitch)
	glm::vec3 newOrientationX = glm::rotate(cam->orientation, glm::radians(-rotx), glm::normalize(glm::cross(cam->orientation, cam->up)));
	if (!(glm::angle(newOrientationX, cam->up) <= glm::radians(5.0f)) && !(glm::angle(newOrientationX, -cam->up) <= glm::radians(5.0f))) {
		cam->orientation = newOrientationX;
	}

	// Horizontal rotation (yaw)
	cam->orientation = glm::rotate(cam->orientation, glm::radians(-roty), cam->up);
}