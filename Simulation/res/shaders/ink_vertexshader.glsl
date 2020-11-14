#version 430 core

layout(binding=3) buffer freq
{
	int frequencies[];	//array of freqs
};

struct InkStruct 
{
	int freq;
	int id;
	ivec2 padding;
	vec3 color;
	float padding2;
};

layout(std140, binding=5) buffer ink
{
	InkStruct inkvals[];
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
out float inkfreq;
out vec3 inkcolor;


void main() {
	ofreq = float(frequencies[gl_VertexID]);
	inkfreq = inkvals[gl_VertexID].freq;
	inkcolor = inkvals[gl_VertexID].color;
	//oink = float(inkvals[gl_VertexID]);
	oflags = flagvals[gl_VertexID];

	//float x_pos = gl_VertexID;
	float x_pos = gl_VertexID - (width * floor(gl_VertexID/width));//- height/2;
	//float y_pos = gl_VertexID;// - width/2;
	float y_pos = floor(gl_VertexID/width);// - width/2;
	gl_Position =  projection * vec4(x_pos,y_pos,0.0,1.0);
}