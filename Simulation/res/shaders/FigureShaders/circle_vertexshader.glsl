#version 330 core

layout(location = 0) in vec2 pos;

uniform mat4 projection;
uniform float xpos;
uniform float ypos;
uniform float radius;

void main() {
	//gl_Position = projection * vec4(xpos, ypos, 0.0f ,1.0);
	gl_Position = projection * vec4(xpos + pos.x * radius, ypos + pos.y * radius, 1.0f, 1);

}