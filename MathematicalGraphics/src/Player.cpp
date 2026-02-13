#include "Player.h"



void Camera::setCameraUniform(const glm::mat4& perspective, const glm::mat4& view, Shader& s) {
	this->perspective = perspective;
	this->view = view;

	glUniformMatrix4fv(glGetUniformLocation(s.programID, "perspective"), 1, GL_FALSE, glm::value_ptr(perspective * view));
}