#version 330 core

layout(location = 0) in vec2 pos;

uniform mat4 projection;
uniform float verttrans;
uniform float hortrans;
uniform float amplitude;
uniform float period;

void main() {
	//gl_Position = projection * vec4(xpos, ypos, 0.0f ,1.0);
	
	gl_Position = projection * vec4(pos.x *1000 , sin((pos.x - hortrans/1000) * 1000/period) * amplitude + verttrans, 1.0f, 1);

}