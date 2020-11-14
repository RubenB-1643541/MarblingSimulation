#version 430 core

struct InkStruct 
{
	int freq;
	int id;
	ivec2 padding;
	vec3 color;
	float padding2;
};

uniform vec3 color;
uniform vec3 freezecolor;

in float ofreq;
in vec4 oflags;
in float inkfreq;
in vec3 inkcolor;

void main() {
	bvec4 flags = bvec4(oflags.x, oflags.y, oflags.z, oflags.w);
	vec3 result = vec3(0,0,0);
	if (inkfreq > 1000)
		result = inkcolor;
	else
		result = inkcolor * (inkfreq / 1000);
	gl_FragColor = vec4(result, 1.0);
}