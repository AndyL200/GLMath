#include "Application.h"
#include "Player.h"


struct Bullet {
	glm::vec2 pos;
};

class InputManager {
public:
	static InputManager& getInstance() {
		static InputManager instance;
		return instance;
	}

	void setControl(ControlComponent* control) {
		this->control = control;
	}
	void setCursor(CursorComponent* cursor) {
		this->cursor = cursor;
	}

	CursorComponent* getCursor() {
		return cursor;
	}

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		getInstance().handleKey(window, key, scancode, action, mods);
	}

	static void cursorCallback(GLFWwindow* window, double xpos, double ypos) {
		getInstance().handleCursor(window, xpos, ypos);
	}
private:
	ControlComponent* control;
	CursorComponent* cursor;

	void handleKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (control) {
			control->key_callback(window, key, scancode, action, mods);
		}

	}
	void handleCursor(GLFWwindow* window, double xpos, double ypos) {
		if (cursor) {
			cursor->cursor_callback(window, xpos, ypos);
		}
	}
};

glm::vec3 hex_to_vec(std::string hex_string) {
	glm::vec4 color = {};
	if (hex_string.substr(0, 2) == "0x") {
		hex_string = hex_string.substr(2);
	}
	for (int i = 0; i < hex_string.size(); i += 2) {
		int value = std::stoi(hex_string.substr(i, 2), nullptr, 16);

		switch (i / 2) {
		case 0: color.r = value / 255.0f; break;
		case 1: color.g = value / 255.0f; break;
		case 2: color.b = value / 255.0f; break;
		case 3: color.a = value / 255.0f; break;
		default: break; // ignore extra data
		}
	}
	return color;
}

class Object {
public:
	glm::vec3 pos;
	glm::mat4 rotation;
	Object(glm::vec3 pos) { this->pos = pos; };
	Object() {};
};

class Target : public Object{	
public:
	bool active;
	glm::vec3 color;
	float cooldown;
	Target(bool active, glm::vec3 pos, glm::vec3 color, float cooldown) : active(active), Object(pos), color(color), cooldown(cooldown) {}
	Target() {
		active = false;
		this->pos = { 1,1,1 };
		color = { 1,1,1}; //white
		cooldown = 10;
	}
	//Copy Constructor
	Target(const Target& t) : active(t.active), Object(t.pos), color(t.color), cooldown(t.cooldown) {};
};

void raycasting() {
	for (int x = 0; x < 1000; x++) {
		float ra = 30.f + (60.f * x / 800.f);
		float rx = std::cos(glm::radians(ra)), ry = std::sin(glm::radians(ra));

	}
}
void drawPlayer(Player& p, GLuint vao, GLuint vbo, GLuint ebo, Shader& s) {
	//TODO
}
void drawFloor(GLuint vao, GLuint vbo, GLuint ebo, Shader& tile, glm::vec3& color) {
    GLfloat xpos = 0.f, ypos = -0.1f, zpos = 0.f;  // Start below player, centered
    glUseProgram(tile.programID);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    GLfloat xoff = 150.f, yoff = 0.1f, zoff = 150.f;  // Large flat surface
    
    // Create just the top face (a flat floor)
    std::vector<GLfloat> vertices = {
        // Just a quad for the floor (4 vertices)
        xpos,       ypos, zpos,         color.r, color.g, color.b,    0.0f, 0.0f,
        xpos + xoff, ypos, zpos,        color.r, color.g, color.b,    10.0f, 0.0f,
        xpos,       ypos, zpos + zoff,  color.r, color.g, color.b,    0.0f, 10.0f,
        xpos + xoff, ypos, zpos + zoff, color.r, color.g, color.b,    10.0f, 10.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,  // First triangle
        1, 2, 3   // Second triangle
    };

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
//recreating VAO, VBO, and EBO each time?
void drawTarget(Target& t, GLuint vao, GLuint vbo, GLuint ebo) {
	GLfloat xpos = t.pos.x, ypos = t.pos.y, zpos = t.pos.z;
	

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	

	//targets are rectangles
	GLfloat xoff = 2.f, yoff = 2.f, zoff = 2.f;
	std::vector<GLfloat> vertices = {
		//8 unique vertices
		//position, color, texture
		xpos,		ypos,		zpos,			t.color.r, t.color.g, t.color.b,		0.0f, 0.0f,		//0
		xpos + xoff,ypos,		zpos,			t.color.r, t.color.g, t.color.b,		1.0f, 0.0f,		//1
		xpos,		ypos + yoff,zpos,			t.color.r, t.color.g, t.color.b,		0.0f, 1.0f,		//2
		xpos + xoff,ypos + yoff,zpos,			t.color.r, t.color.g, t.color.b,		1.0f, 1.0f,		//3
		xpos,		ypos,		zpos + zoff,	t.color.r, t.color.g, t.color.b,		0.0f, 0.0f,		//4
		xpos + xoff,ypos,		zpos + zoff,	t.color.r, t.color.g, t.color.b,		1.0f, 0.0f,		//5
		xpos,		ypos + yoff,zpos + zoff,	t.color.r, t.color.g, t.color.b,		0.0f, 1.0f,		//6
		xpos + xoff,ypos + yoff,zpos + zoff,	t.color.r, t.color.g, t.color.b,		1.0f, 1.0f, 	//7
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

	glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(indices[0]), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Application::run() {
	if (!glfwInit()) {
		std::cerr << "Cannot load GLFW" << std::endl;
	}
	int x_mon, y_mon, width, height;
	primary = glfwGetPrimaryMonitor();
	glfwGetMonitorWorkarea(primary, &x_mon, &y_mon, &width, &height);
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


	Shader prime;
	Shader text;
	Shader tile;
	Shader sky;

	uint8_t success = prime.loadShader("shaders/grid.vert", "shaders/grid.frag");
	if (success != 0) {
		std::cerr << "Shader failure, enter character to terminate";
		char x;
		x = getchar();
		return;
	}
	std::cout << "Prime Shader loaded successfully" << std::endl;


	success = tile.loadShader("shaders/tile.vert", "shaders/tile.frag");
	if (success != 0) {
		std::cerr << "Shader failure, enter character to terminate";
		char x;
		x = getchar();
		return;
	}

	std::cout << "Tile Shader loaded successfully" << std::endl;

	glm::vec3 floor_color = hex_to_vec("6589A6");

	
	
	//slice in the xz plane (consistent y)
	uint8_t grid[5][8] = {
		{1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1}
	};

	float gridxlen = 2;
	float gridylen = 2;

	//stbi uses the top right as origin
	stbi_set_flip_vertically_on_load(true);
	int widthImg, heightImg, numColCh;
	unsigned char* floor_bytes = stbi_load("floor_img.png", &widthImg, &heightImg, &numColCh, 0);

	GLuint floor_texture;
	
	glGenTextures(1, &floor_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, floor_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, floor_bytes);

	GLuint sky_texture;
	unsigned char* sky_bytes = stbi_load("sky_img.png", &widthImg, &heightImg, &numColCh, 0);

	glGenTextures(1, &sky_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sky_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, sky_bytes);

	glGenerateMipmap(GL_TEXTURE_2D);

	//set uniforms
	GLuint tex0Uni = glGetUniformLocation(tile.programID, "tex0");
	glUniform1i(tex0Uni, 0);
	

	glm::mat4 perspective = glm::perspective(glm::radians(90.f), (float)width/height, 0.1f, 100.f); //apply perspective (original code didn't need a matrix though)
	// Create scale matrix
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f/width, 2.0f/height, 2.0f/1000.0f));
	// Create translation matrix  
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, -1.0f));
	glm::mat4 transform_to = translate * scale;
	glm::mat4 transform_from = glm::inverse(transform_to);
	std::unordered_map<int, Target> t_map;


	CursorComponent cursor;
	Player player1;
	player1.cam->Matrix(prime, perspective);
	cursor.set_callback([&player1](double xpos, double ypos, float sensitivity) {
		player1.handleCursorChange(xpos, ypos, sensitivity);
	});
	unsigned char pixels[16 * 16 * 4];
	memset(pixels, 0xff, sizeof(pixels));

	GLFWimage image;
	image.width = 16;
	image.height = 16;
	image.pixels = pixels;

	cursor.useCursor(image, main);
	player1.setPosition(5.f, 1.f, 4.f);

	InputManager::getInstance().setControl(&player1.getControl());
	InputManager::getInstance().setCursor(&cursor);

	glfwSetCursorPosCallback(main, InputManager::cursorCallback);
	glfwSetKeyCallback(main, InputManager::keyCallback);

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
		
		glUseProgram(prime.programID);
		player1.cam->Matrix(prime);
		for (int i = 4; i >= 0; i--) {
			for (int j = 0; j < 8; j++) {
				float xpos = j * 5.f;
				float zpos = i * 5.f;
				if (grid[i][j]) {
					if (t_map.find(i * 8 + j) != t_map.end()) {
						drawTarget(t_map[i * 8 + j], t_vao, t_vbo, t_ebo);
					}
					else {
						glm::vec3 target_pos = glm::vec3( xpos, 0, zpos);
						Target t(false, target_pos, hex_to_vec("2795F5"), 5);
						t_map[i * 8 + j] = t;
						drawTarget(t, t_vao, t_vbo, t_ebo);
					}
					
				}
			}
		}
		
		glUseProgram(tile.programID);
		drawFloor(t_vao, t_vbo, t_ebo, tile, floor_color);
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
	glDeleteTextures(1, &floor_texture);

	glfwDestroyWindow(main);
	glfwTerminate();
}

//TODO(apply perspective camera from the player's position)