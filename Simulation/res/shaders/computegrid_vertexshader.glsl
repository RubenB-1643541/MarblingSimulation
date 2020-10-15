#version 430 core

layout(binding=3) buffer freq
{
	int frequencies[];	//array of freqs
};

layout(binding=5) buffer ink
{
	int inkvals[];
};

layout(binding=7) buffer flags
{
	vec4 flagvals[]; //freeze - unused - unused - unused
};


uniform int width;
uniform int height;

uniform mat4 projection;

out float ofreq;
out vec4 oflags;
out float oink;

void main() {
	ofreq = float(frequencies[gl_VertexID])/100;
	oink = 0;
	//oink = float(inkvals[gl_VertexID]);
	oflags = flagvals[gl_VertexID];

	//float x_pos = gl_VertexID;
	float x_pos = gl_VertexID - (width * floor(gl_VertexID/width));//- height/2;
	//float y_pos = gl_VertexID;// - width/2;
	float y_pos = floor(gl_VertexID/width);// - width/2;
	gl_Position =  projection * vec4(x_pos,y_pos,0.0,1.0);
	//gl_Position =  projection *  vec4(x_pos,y_pos,0.0,1.0);
	//gl_Position =  projection * translation *  vec4(x_pos * 2,y_pos * 2,0.0,1.0);
	//gl_Position = projection * vec4(x_pos/10, y_pos/10, 0.0, 1.0);
	//gl_Position = vec4(-0.0052, 0.01, 0.0, 1.0);
	//gl_Position =  projection * vec4(x_pos/10,y_pos/10,0.0,1.0);
	//gl_Position = vec4(x_pos/10, y_pos/10, 0.0, 1.0);
}