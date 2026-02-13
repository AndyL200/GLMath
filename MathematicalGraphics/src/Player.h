#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

#ifndef MATHPLAYER
#define MATHPLAYER

class Camera {
public:
	glm::vec3 pos;
	void setCameraUniform(const glm::mat4& perspective, const glm::mat4& view, Shader& s);
	Camera(glm::vec3 pos) : pos(pos) {};
	Camera(const Camera& other) : pos(other.pos), perspective(other.getPerspective()), view(other.getView()) {};
	Camera() { this->pos = { 0,0,0 }; };

	glm::mat4 getPerspective() const { return perspective; }
	glm::mat4 getView() const { return view; }
private:
	glm::mat4 perspective;
	glm::mat4 view;

};

class Player {
public:
	glm::vec3 pos;
	Camera* cam;
	glm::vec3 euler; //put the euler angles in radians to make things easier (just translate to degrees for the player)

	Player(glm::vec3& pos, Camera* cam) : pos(pos), cam(cam) { this->euler = {0, 0, 0}; };
	Player() {
		this->pos = { 0,0,0 };
		this->cam = new Camera(pos);
		this->euler = { 0, 0, 0 };
	};
	~Player() {
		delete cam;
	}

	//Copy Constructor
	Player(const Player& other) : pos(other.pos), euler(other.euler) {
		this->cam = new Camera(*other.cam);
	}

	Player& operator=(const Player& other) {
		if (this != &other) {
			delete cam;
			this->pos = other.pos;
			this->euler = other.euler;
			this->cam = new Camera(*other.cam);
		}
		return *this;
	}

};

#endif