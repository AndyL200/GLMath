#include "Application.h"


struct ObjectRect {
	glm::vec2 pos;
	glm::vec3 color;

};

struct Bullet {
	glm::vec2 pos;
};

struct Player {
	glm::vec2 pos;
};

struct Target {
	bool active;
	glm::vec2 pos;
	float cooldown;
};

void raycasting() {
	std::vector<float> zbuffer(800, 999.f);
	for (int x = 0; x < 800; x++) {
		float ra = 30.f + (60.f * x / 800.f);
		float rx = std::cos(glm::radians(ra)), ry = std::sin(glm::radians(ra));

	}
}

void drawTarget(GLfloat xpos, GLfloat ypos, GLfloat zpos) {
	//targets are rectangles
	int xoff = 10, yoff = 10, zoff = 10;
	std::vector<glm::vec3> verticies = {
		//8 unique vertices
		{xpos, ypos, zpos}, //0
		{xpos + xoff, ypos, zpos}, //1
		{xpos, ypos + yoff, zpos}, //2
		{xpos + xoff, ypos + yoff, zpos}, //3
		{xpos, ypos, zpos + zoff}, //4
		{xpos + xoff, ypos, zpos + zoff}, //5
		{xpos, ypos + yoff, zpos + zoff}, //6
		{xpos + xoff, ypos + yoff, zpos + zoff} //7
	};

	unsigned int indices[] = {
		0,1,2, 1,2,3, //front face
		0,4,2, 4,6,2,  //left face
		1,5,3, 5,3,7, // right face
		4,5,6, 5,6,7, //back face
		0,1,4, 1,5,4, //top face
		//bottom face
	};


}

void Application::run() {
	if (!glfwInit()) {
		std::cerr << "Cannot load GLFW" << std::endl;
	}
	int xpos, ypos, width, height;
	primary = glfwGetPrimaryMonitor();
	glfwGetMonitorWorkarea(primary, &xpos, &ypos, &width, &height);
	GLFWwindow* main = glfwCreateWindow(width/2, height/2, "MathGame", primary, nullptr);

	glfwMakeContextCurrent(main);

	if (!gladLoaderLoadGL()) {
		std::cerr << "Cannot load Glad" << std::endl;
	}

	uint8_t grid[5][8] = {
		{1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1}
	};





	glEnable(GL_BLEND);

	//threads

	while (!glfwWindowShouldClose(main)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glfwSwapBuffers(main);
		glfwPollEvents();
	}

	glfwDestroyWindow(main);
	glfwTerminate();
}