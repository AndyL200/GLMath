#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>

#ifndef MATHSHADE
#define MATHSHADE

class Shader {
public:
	GLuint programID;
	
	uint8_t loadShader(const char* vertfile, const char* fragfile) {

		std::string vertex = get_file_contents(vertfile);
		std::string frag = get_file_contents(fragfile);

		if (vertex.empty()) {
        	std::cerr << "Failed to load vertex shader: " << vertfile << std::endl;
        	return 1;
    	}
    	if (frag.empty()) {
        	std::cerr << "Failed to load fragment shader: " << fragfile << std::endl;
        	return 1;
    	}

		programID = glCreateProgram();
		const GLchar* vertptr = vertex.c_str();
		const GLchar* fragptr = frag.c_str();
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
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			return 1 << 1;
		}
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cerr << "Error compiling fragment shader \n" << infoLog << std::endl;
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			return 1 << 2;
		}
		glAttachShader(programID, vertexShader);
		glAttachShader(programID, fragmentShader);
		glLinkProgram(programID);

		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(programID, 512, NULL, infoLog);
			std::cerr << "Error linking shader program \n" << infoLog << std::endl;
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			return 1 << 3;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		std::cout << "Shader successfully compiled" << std::endl;
		return 0; //Success
	}
private:

	std::string get_file_contents(std::string filename)
	{
		std::cout << "Shader filename: [" << filename << "] loading..." << std::endl;

		std::ifstream in(filename, std::ios::binary);
		std::cout << "Opening : [" << filename << "] " << std::endl;
		std::string contents;
		if (!in.good()) {
        	std::cerr << "Error: File [" << filename << "] does not exist or cannot be opened!" << std::endl;
        	return "";
    	}
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			in.seekg(0, std::ios::beg);
			contents.resize(size);
			in.read(&contents[0], size);
			in.close();
			std::cout << "File content slice " << contents.substr(0, 50) << "..." << std::endl; // Print the first 100 characters of the file
			return contents;
		}
		in.close();
		fprintf(stderr, "Cannot read file\n");
		return "";
	}
};

#endif