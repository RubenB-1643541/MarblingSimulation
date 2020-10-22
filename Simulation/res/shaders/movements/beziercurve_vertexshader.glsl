#version 330 core

layout(location = 0) in float t;

uniform vec2 control [4];
uniform mat4 projection;

void main() {
	//gl_Position = projection * vec4(xpos, ypos, 0.0f ,1.0);
	
	vec2 pos = pow((1-t), 3) * control[0] + 3 * pow((1-t), 2) * t * control[1] + 3 * (1-t) * pow(t, 2) * control[2] + pow(t, 3) * control[3];
	gl_Position = projection * vec4(pos.x, pos.y, 1.0f, 1);

}