#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <functional>
#include <mutex>

#ifndef MATHPLAYER
#define MATHPLAYER


//using the component model
enum ControlSignals {
	LEFT = 1 << 0,
	RIGHT = 1 << 1,
	UP = 1 << 2,
	DOWN = 1 << 3,
	ACTION = 1 << 4
};
class ControlComponent {
public:
	using SignalCallback = std::function<void(ControlSignals)>;

	ControlComponent(GLchar left = 'A', GLchar right = 'D', GLchar up = 'W', GLchar down = 'S', GLchar action = 'E') : left(left), right(right), up(up), down(down), action(action) {}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void setSignalCallback(SignalCallback callback) {
		this->callback = callback;
	}
private:
	GLchar left;
	GLchar right;
	GLchar up;
	GLchar down;
	GLchar action;
	SignalCallback callback;

	void sendSignal(ControlSignals signal) {
		// Handle control signals here
		if (callback) {
			callback(signal);
		}
	}
};


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
	float speed = 0.5f;
	Camera* cam;
	glm::vec3 euler; //put the euler angles in radians to make things easier (just translate to degrees for the player)
	ControlComponent control; //small enough for stack
	CursorComponent cursor;
	unsigned int signals; //bitfield for control signals
	std::mutex signal_mutex; //mutex for thread safety when modifying signals

	Player(glm::vec3& pos, Camera* cam) : pos(pos), cam(cam) { this->euler = {0, 0, 0}; this->control = ControlComponent(); };
	Player() {
		this->pos = { 0,0,0 };
		this->cam = new Camera(pos);
		this->euler = { 0, 0, 0 };
		this->control = ControlComponent();

		this->control.setSignalCallback([this](ControlSignals signal) {
			std::lock_guard<std::mutex> lock(this->signal_mutex);
			this->signals |= signal;

			this->handleControlSignals();
		});
	};

	void handleControlSignals();
	~Player() {
		delete cam;
	}

	//Copy Constructor
	Player(const Player& other) : pos(other.pos), euler(other.euler), control(other.control) {
		this->cam = new Camera(*other.cam);
	}

	Player& operator=(const Player& other) {
		if (this != &other) {
			delete cam;
			this->pos = other.pos;
			this->euler = other.euler;
			this->control = other.control;
			this->cam = new Camera(*other.cam);
		}
		return *this;
	}

};

#endif