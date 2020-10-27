#version 330 core

layout(location = 0) in vec2 pos;

uniform mat4 projection;
uniform float xpos;
uniform float ypos;
uniform float size;

void main() {
	
	gl_PointSize = size;
	gl_Position = projection * vec4(xpos + pos.x, ypos + pos.y, 1.0f, 1.0f);
}