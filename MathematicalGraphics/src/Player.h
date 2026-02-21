#include "mpch.h"
#include <glm/gtc/type_ptr.hpp>
#include "Cursor.h"
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
	glm::vec3 orientation;
	int width;
	int height;
	glm::vec3 up = { 0,1,0 };
	void Matrix(Shader& s);
	void Matrix(Shader& s, glm::mat4& perspective);
	Camera(glm::vec3 pos, glm::vec3 orientation) : pos(pos), orientation(orientation) {};
	Camera(const Camera& other) : pos(other.pos), perspective(other.getPerspective()), orientation(other.orientation), width(other.width), height(other.height) {};
	Camera() {
		this->pos = { 0,0,0 };
		this->orientation = { 0,0,0 };
		perspective = glm::mat4(1);
	}
	glm::mat4 getPerspective() const { return perspective; }
private:
	glm::mat4 perspective;

};

class Player {
public:
	float speed = 0.5f;
	Camera* cam;

	Player(glm::vec3& pos, Camera* cam) : pos(pos), cam(cam) { this->orientation = {0, 0, 0}; this->control = ControlComponent(); };
	Player() {
		this->pos = { 0,0,0 };
		this->cam = new Camera(pos, orientation);
		this->orientation = { 0, 0, 0 };
		this->control = ControlComponent();

		this->control.setSignalCallback([this](ControlSignals signal) {
			std::lock_guard<std::mutex> lock(this->signal_mutex);
			this->signals |= signal;

			this->handleControlSignals();
		});
	};

	void handleControlSignals();
	void handleCursorChange(double xpos, double ypos, float sensitivity);
	~Player() {
		delete cam;
	}

	//Copy Constructor
	Player(const Player& other) : pos(other.pos), orientation(other.orientation), control(other.control), signals(other.signals) {
		this->cam = new Camera(*other.cam);
	}

	Player& operator=(const Player& other) {
		if (this != &other) {
			delete cam;
			this->pos = other.pos;
			this->orientation = other.orientation;
			this->control = other.control;
			this->signals = other.signals;
			this->cam = new Camera(*other.cam);
		}
		return *this;
	}

	glm::vec3 getPosition() { return pos; }
	void setPosition(glm::vec3& pos) { this->pos = pos; this->cam->pos = pos; }
	void setPosition(float x, float y, float z) { pos.x = x; pos.y = y; pos.z = z; this->cam->pos = pos; }

	ControlComponent getControl() { return control; }
private:
	glm::vec3 pos;
	glm::vec3 orientation; //put the euler angles in radians to make things easier (just translate to degrees for the player)
	ControlComponent control; //small enough for stack
	unsigned int signals; //bitfield for control signals
	std::mutex signal_mutex; //mutex for thread safety when modifying signals
};

#endif