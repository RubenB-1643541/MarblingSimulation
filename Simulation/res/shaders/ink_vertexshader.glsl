#version 430 core

layout(binding=1) buffer vel
{
	vec2 velocities[];	//array of velocities * 1000 -> 1000 == 1
};

struct InkStruct 
{
	float freq;
	int id;
	ivec2 padding;
};

layout(std140, binding=5) buffer ink
{
	InkStruct inkvals[];
};

layout(binding=7) buffer flags
{
	ivec4 flagvals[]; //freeze - unused - unused - unused
};

layout(std140, binding=8) uniform colors
{
	vec3 colorvals[];
};


uniform int width;
uniform int height;
uniform float intensity;
uniform float freezeintensity;

uniform mat4 projection;

out float ofreq;
out vec4 oflags;
out float inkfreq;
out vec3 inkcolor;
out vec3 watercolor;
out float inkid;
out float intensityout;
out float freezeintensityout;
out vec2 force;


void main() {
	//ofreq = float(frequencies[gl_VertexID] * intensity);
	force = vec2(velocities[gl_VertexID]);
	intensityout = intensity;
	freezeintensityout = freezeintensity;
	inkfreq = inkvals[gl_VertexID].freq;
	//inkfreq = inkvals[gl_VertexID].freq;
	//inkcolor = vec3(1,0,0);
	//inkcolor = colorvals[0];
	inkid = inkvals[gl_VertexID].id;
	inkcolor = colorvals[inkvals[gl_VertexID].id];
	watercolor = colorvals[0];
	//oink = float(inkvals[gl_VertexID]);
	oflags = vec4(flagvals[gl_VertexID]);

	//float x_pos = gl_VertexID;
	float x_pos = gl_VertexID - (width * floor(gl_VertexID/width));//- height/2;
	//float y_pos = gl_VertexID;// - width/2;
	float y_pos = floor(gl_VertexID/width);// - width/2;
	gl_Position =  projection * vec4(x_pos,y_pos,0.0,1.0);
}