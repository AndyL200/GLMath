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

class Target {
private:
	bool active;
	glm::vec3 pos;
	glm::vec4 color;
	glm::vec2 tex;
	float cooldown;
public:
	Target(bool active, glm::vec3 pos, glm::vec4 color, glm::vec2 tex, float cooldown) : active(active), pos(pos), color(color), tex(tex), cooldown(cooldown) {}
	Target() {
		active = false;
		pos = { 1,1,1 };
		color = { 1,1,1,1}; //white
		tex = { 1,1 };
		cooldown = 10;
	}
};

void raycasting() {
	std::vector<float> zbuffer(800, 999.f);
	for (int x = 0; x < 800; x++) {
		float ra = 30.f + (60.f * x / 800.f);
		float rx = std::cos(glm::radians(ra)), ry = std::sin(glm::radians(ra));

	}
}
//recreating VAO, VBO, and EBO each time?
void drawTarget(Target t) {
	GLfloat xpos = t.pos.x, ypos = t.pos.y, zpos = t.pos.z;
	GLuint vao, vbo, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	

	//targets are rectangles
	int xoff = 10, yoff = 10, zoff = 10;
	std::vector<GLfloat> vertices = {
		//8 unique vertices
		//position, color, texture
		xpos,		ypos,		zpos,			t.color.r, t.color.g, t.color.b, t.color.a,	t.tex.r, t.tex.g,		//0
		xpos + xoff,ypos,		zpos,			t.color.r, t.color.g, t.color.b, t.color.a,	t.tex.r, t.tex.g,		//1
		xpos,		ypos + yoff,zpos,			t.color.r, t.color.g, t.color.b, t.color.a,	t.tex.r, t.tex.g,		//2
		xpos + xoff,ypos + yoff,zpos,			t.color.r, t.color.g, t.color.b, t.color.a,	t.tex.r, t.tex.g,		//3
		xpos,		ypos,		zpos + zoff,	t.color.r, t.color.g, t.color.b, t.color.a,	t.tex.r, t.tex.g,		//4
		xpos + xoff,ypos,		zpos + zoff,	t.color.r, t.color.g, t.color.b, t.color.a,	t.tex.r, t.tex.g,		//5
		xpos,		ypos + yoff,zpos + zoff,	t.color.r, t.color.g, t.color.b, t.color.a,	t.tex.r, t.tex.g,		//6
		xpos + xoff,ypos + yoff,zpos + zoff,	t.color.r, t.color.g, t.color.b, t.color.a,	t.tex.r, t.tex.g,		//7
	};

	unsigned int indices[] = {
		0,1,2, 1,2,3, //front face
		0,4,2, 4,6,2,  //left face
		1,5,3, 5,3,7, // right face
		4,5,6, 5,6,7, //back face
		0,1,4, 1,5,4, //top face
		2,6,3, 6,3,7//bottom face
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)3);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)6);

	glDrawElements(GL_TRIANGLES, sizeof(vertices), GL_FLOAT, indices);

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

	float gridxlen = 2;
	float gridylen = 2;

	glm::mat4 perspective = glm::perspective(fovy, aspect, zNear, zFar); //apply perspective (original code didn't need a matrix though)
	glm::mat4 transform = glm::mat4({width, 0, 0, 0}, {0 , height, 0, 0}, {0, 0, zFar, 0}, {0, 0, 0, 0}); //transform from normalized points
	std::unordered_map<int, Target> t_map;

	shader->loadShader("shader/grid.vert", "shaders/grid.frag");
	glUseProgram(shader->programID);
	
	glEnable(GL_BLEND);

	//threads

	while (!glfwWindowShouldClose(main)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 8; j++) {
				if (grid[i][j]) {
					if (t_map.find(i * j + j) != t_map.end()) {
						drawTarget(t_map[i * j + j]);
					}
					else {
						Target t(false, {}, {}, {}, 5);
						t_map[i * j + j] = t;
						drawTarget(t);
					}
					
				}
			}
		}
		glfwSwapBuffers(main);
		glfwPollEvents();
	}

	glfwDestroyWindow(main);
	glfwTerminate();
}