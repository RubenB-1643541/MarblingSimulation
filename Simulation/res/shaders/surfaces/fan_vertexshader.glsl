#version 330 core

layout(location = 0) in vec2 pos;

uniform float xpos;
uniform float ypos;
uniform float angle;
uniform float len;
uniform float rot;
uniform mat4 projection;
uniform mat4 rotation;
uniform mat4 translation;


void main() {

	if(pos.y == 0)
		gl_Position = projection * translation * rotation * vec4(pos.x, pos.y, 1.0f, 1.0f);
	else {
		gl_Position = projection * translation * rotation * vec4(pos.x * sin(angle + rot) * len/10, pos.y * cos(angle + rot) * len/2, 1.0f, 1.0f);
	}
	//if(pos.y == 0)
	//	gl_Position = projection * rotation * vec4(pos.x + xpos, pos.y + ypos, 1.0f, 1.0f);
	//else {		
	//	gl_Position = projection * rotation * vec4(pos.x  * 10 * sin(angle) + xpos, pos.y * len * cos(angle) + ypos, 1.0f, 1.0f);
	//}
	
	//gl_Position = projection * translation * rotation *  vec4(pos.x * width, pos.y * height, 1.0f, 1.0f);

}