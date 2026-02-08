#ifndef MATHSHADE
#define MATHSHADE

class Shader {
public:
	GLuint programID;
	
	void loadShader(const char* vertfile, const char* fragfile) {
		std::string vertex = readGLSL(fopen(vertfile, "r"));
		std::string frag = readGLSL(fopen(fragfile, "r"));


		programID = glCreateProgram();
		const GLchar* vertptr = vertex.c_str();
		const GLchar* fragptr = vertex.c_str();
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertptr, NULL);
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragptr, NULL);

		glCompileShader(vertexShader);
		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cerr << "Error compiling vertex shader \n" << infoLog << std::endl;
		}
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cerr << "Error compiling fragment shader \n" << infoLog << std::endl;
		}
		glAttachShader(programID, vertexShader);
		glAttachShader(programID, fragmentShader);
		glLinkProgram(programID);

		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(programID, 512, NULL, infoLog);
			std::cerr << "Error linking shader program \n" << infoLog << std::endl;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

	}
private:

	std::string readGLSL(FILE* f) {
		std::string res = "";
		GLchar temp;
		while (fscanf_s(f, "%c", temp) == 1) {
			res += temp;
		}

		return res;
	};
};

#endif