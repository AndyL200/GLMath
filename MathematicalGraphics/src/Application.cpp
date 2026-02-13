#include "Application.h"
#include "Player.h"

struct Bullet {
	glm::vec2 pos;
};


class Target {	
public:
	bool active;
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 tex;
	float cooldown;
	Target(bool active, glm::vec3 pos, glm::vec3 color, glm::vec2 tex, float cooldown) : active(active), pos(pos), color(color), tex(tex), cooldown(cooldown) {}
	Target() {
		active = false;
		pos = { 1,1,1 };
		color = { 1,1,1}; //white
		tex = { 1,1 };
		cooldown = 10;
	}
	//Copy Constructor
	Target(const Target& t) : active(t.active), pos(t.pos), color(t.color), tex(t.tex), cooldown(t.cooldown) {};
};

void raycasting() {
	for (int x = 0; x < 1000; x++) {
		float ra = 30.f + (60.f * x / 800.f);
		float rx = std::cos(glm::radians(ra)), ry = std::sin(glm::radians(ra));

	}
}
void drawPlayer(Player& p, GLuint vao, GLuint vbo, GLuint ebo) {
	//TODO

}
//recreating VAO, VBO, and EBO each time?
void drawTarget(Target& t, GLuint vao, GLuint vbo, GLuint ebo) {
	GLfloat xpos = t.pos.x, ypos = t.pos.y, zpos = t.pos.z;
	

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	

	//targets are rectangles
	float xoff = 0.1f, yoff = 0.1f, zoff = 0.1f;
	std::vector<GLfloat> vertices = {
		//8 unique vertices
		//position, color, texture
		xpos,		ypos,		zpos,			t.color.r, t.color.g, t.color.b,		t.tex.r, t.tex.g,		//0
		xpos + xoff,ypos,		zpos,			t.color.r, t.color.g, t.color.b,		t.tex.r, t.tex.g,		//1
		xpos,		ypos + yoff,zpos,			t.color.r, t.color.g, t.color.b,		t.tex.r, t.tex.g,		//2
		xpos + xoff,ypos + yoff,zpos,			t.color.r, t.color.g, t.color.b,		t.tex.r, t.tex.g,		//3
		xpos,		ypos,		zpos + zoff,	t.color.r, t.color.g, t.color.b,		t.tex.r, t.tex.g,		//4
		xpos + xoff,ypos,		zpos + zoff,	t.color.r, t.color.g, t.color.b,		t.tex.r, t.tex.g,		//5
		xpos,		ypos + yoff,zpos + zoff,	t.color.r, t.color.g, t.color.b,		t.tex.r, t.tex.g,		//6
		xpos + xoff,ypos + yoff,zpos + zoff,	t.color.r, t.color.g, t.color.b,		t.tex.r, t.tex.g,		//7
	};

	unsigned int indices[] = {
		0,1,2, 1,2,3, //front face
		0,4,2, 4,6,2,  //left face
		1,5,3, 5,3,7, // right face
		4,5,6, 5,6,7, //back face
		0,1,4, 1,5,4, //top face
		2,6,3, 6,3,7//bottom face
	};

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(indices[0]), GL_UNSIGNED_INT, indices);

}

void Application::run() {
	if (!glfwInit()) {
		std::cerr << "Cannot load GLFW" << std::endl;
	}
	int xpos, ypos, width, height;
	primary = glfwGetPrimaryMonitor();
	glfwGetMonitorWorkarea(primary, &xpos, &ypos, &width, &height);
	//pass primary monitor to make fullscreen
	GLFWwindow* main = glfwCreateWindow(width/2, height/2, "MathGame", nullptr, nullptr);
	if (!main) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
	glfwMakeContextCurrent(main);
	
	if (!gladLoaderLoadGL()) {
		std::cerr << "Cannot load Glad" << std::endl;
		glfwDestroyWindow(main);
        glfwTerminate();
        return;
	}

	glfwGetFramebufferSize(main, &width, &height);
	glViewport(0, 0, width, height);

	

	uint8_t grid[5][8] = {
		{1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1}
	};

	float gridxlen = 2;
	float gridylen = 2;

	glm::mat4 perspective = glm::perspective(glm::radians(90.f), (float)width/height, 0.1f, 1000.f); //apply perspective (original code didn't need a matrix though)
	glm::mat4 transform_from = glm::mat4({width/2.f, 0, 0, width/2.f}, {0 , height/2.f, 0, height/2.f}, {0, 0, 1000/2.f, 1000/2.f}, {0, 0, 0, 1}); //transform from normalized points (is thiss going in the wrong direction?)
	glm::mat4 transform_to = glm::mat4({2.f/width, 0, 0, -1 }, { 0 , 2.f/height, 0, -1 }, { 0, 0, 2.f/1000, -1 }, { 0, 0, 0, 1 });
	std::unordered_map<int, Target> t_map;
	Player player1;
	
	uint8_t success = shader->loadShader("shaders/grid.vert", "shaders/grid.frag");
	if (success != 0) {
		std::cerr << "Shader failure, enter character to terminate";
		char x;
		x = getchar();
		return;
	}

	std::cout << "Shader loaded successfully" << std::endl;
	glUseProgram(shader->programID);
	

	glm::vec3 forward = { glm::cos(player1.euler.x) * glm::cos(player1.euler.y), glm::sin(player1.euler.x), glm::cos(player1.euler.x) * glm::sin(player1.euler.y)};
	glm::vec3 player_pos = player1.pos;


	//static camera for now
	glm::vec3 camera_pos = {0, 0, -0.5f};    // Move camera back
	glm::vec3 camera_target = {0, 0, 0}; // Look at origin
	glm::vec3 up = {0, 1, 0};

	glm::mat4 view = glm::lookAt(camera_pos, camera_target, up);
	GLuint mvp_location = glGetUniformLocation(shader->programID, "perspective"); // or whatever your uniform is called
	if (mvp_location != -1) {
    	glm::mat4 mvp = perspective * view;
    	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));
	}
	//player1.cam->setCameraUniform(perspective, glm::lookAt(player_pos, player_pos + forward, { 0,1,0 }), *shader);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);          // Enable depth testing
	glDepthFunc(GL_LESS);  

	GLuint t_vao, t_vbo, t_ebo;
	glGenVertexArrays(1, &t_vao);
	glGenBuffers(1, &t_vbo);
	glGenBuffers(1, &t_ebo);

	GLuint p_vao, p_vbo, p_ebo;
	glGenVertexArrays(1, &p_vao);
	glGenBuffers(1, &p_vbo);
	glGenBuffers(1, &p_ebo);

	if (t_vao == 0 || t_vbo == 0 || t_ebo == 0 || p_vao == 0 || p_vbo == 0 || p_ebo == 0) {
    	std::cerr << "Failed to generate OpenGL buffers" << std::endl;
    	return;
	}

	//threads

	while (!glfwWindowShouldClose(main)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 8; j++) {
				float xpos = (float)width /8 * j;
				float zpos = (float)1000 / 5 * i;
				if (grid[i][j]) {
					if (t_map.find(i * j + j) != t_map.end()) {
						drawTarget(t_map[i * j + j], t_vao, t_vbo, t_ebo);
					}
					else {
						glm::vec3 target_pos = transform_to * glm::vec4( xpos, 0, zpos, 1 );
						Target t(false, target_pos, {1,1,1}, {0,0}, 5);
						t_map[i * j + j] = t;
						drawTarget(t, t_vao, t_vbo, t_ebo);
					}
					
				}
			}
		}
		drawPlayer(player1, p_vao, p_vbo, p_ebo);
		glfwSwapBuffers(main);
		glfwPollEvents();
	}
	// ADD CLEANUP - this is crucial!
    glDeleteVertexArrays(1, &p_vao);
    glDeleteBuffers(1, &p_vbo);
    glDeleteBuffers(1, &p_ebo);
    glDeleteVertexArrays(1, &t_vao);
    glDeleteBuffers(1, &t_vbo);
    glDeleteBuffers(1, &t_ebo);

	glfwDestroyWindow(main);
	glfwTerminate();
}

//TODO(apply perspective camera from the player's position)