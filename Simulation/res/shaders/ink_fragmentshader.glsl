#version 430 core

struct InkStruct 
{
	int freq;
	int id;
	ivec2 padding;
};

//uniform vec3 freezecolor;

in float ofreq;
in vec4 oflags;
in float inkfreq;
in float inkid;
in vec3 inkcolor;
in vec3 watercolor;
in float intensityout;
in float freezeintensityout;

void main() {
	bvec4 flags = bvec4(oflags.x, oflags.y, oflags.z, oflags.w);
	vec3 result = vec3(0,0,0);
	if (inkfreq > 1000)
		result = inkcolor;
	else if (inkfreq > 300)
		result = inkcolor * (inkfreq / 1000);
	else if (inkfreq > 0)
		result = inkcolor * 0.3;
	else if (inkfreq == 0)
		result = inkcolor * 0.1;
	
	if(inkid == 0) {
		result = inkcolor;
	}
	result = mix(watercolor, result, intensityout);
	if(flags.x) {//Freeze
		result = mix(result, vec3(0.63,0.9,1), freezeintensityout);
	}
	

	gl_FragColor = vec4(result, 1.0);
}