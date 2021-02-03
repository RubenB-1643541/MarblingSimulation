#version 330 core

layout(location = 0) in vec2 pos;

uniform mat4 projection;
uniform vec2 p1;
uniform vec2 p2;

void main() {
	//gl_Position = projection * vec4(xpos, ypos, 0.0f ,1.0);
	if(pos == vec2(0,0))
		gl_Position = projection * vec4(p1.x, p1.y, 1.0f, 1.0f);
	else
		gl_Position = projection * vec4(p2.x, p2.y, 1.0f, 1.0f);

}