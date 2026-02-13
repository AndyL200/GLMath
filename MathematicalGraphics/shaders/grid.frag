#version 330 core
in vec3 fragCol;
in vec2 fragTex;

out vec4 FragColor;


void main()
{
FragColor = vec4(fragCol, 1);
}