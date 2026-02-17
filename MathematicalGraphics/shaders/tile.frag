#version 330 core
in vec3 fragCol;
in vec2 fragTex;

out vec4 FragColor;

uniform sampler2D tex0;

void main()
{
FragColor = vec4(fragCol, 1) * texture(tex0, fragTex);
}