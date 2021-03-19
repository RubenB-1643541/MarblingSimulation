#version 330 core

layout(location = 0) in vec2 pos;

uniform mat4 projection;
uniform float xpos;
uniform float ypos;
uniform float width;
uniform float height;
uniform bool center;
uniform mat4 rotation;
uniform mat4 translation;

out vec2 TexCoord;

void main() {
	//gl_Position = projection * vec4(xpos, ypos, 0.0f ,1.0);
	TexCoord = pos;
	if(center)
		gl_Position = projection * translation * rotation *  vec4(pos.x * width - width/2, pos.y * height - height/2, 1.0f, 1.0f);
		//gl_Position = projection * rotation * vec4((xpos + pos.x * width) - width/2, (ypos + pos.y * height) - height/2, 1.0f, 1.0f);
	else
		gl_Position = projection * translation * rotation *  vec4(pos.x * width, pos.y * height, 1.0f, 1.0f);

}