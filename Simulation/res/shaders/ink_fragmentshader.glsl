#version 430 core

struct InkStruct 
{
	int freq;
	int id;
	ivec2 padding;
};

//uniform vec3 freezecolor;
uniform float renderforce;

in float ofreq;
in vec4 oflags;
in float inkfreq;
in float inkid;
in vec3 inkcolor;
in vec3 watercolor;
in float intensityout;
in float freezeintensityout;
in vec2 force;

void main() {

	if(renderforce == 0) {
		bvec4 flags = bvec4(oflags.x, oflags.y, oflags.z, oflags.w);
		vec3 result = vec3(0,0,0);
		float intensscale = 1.0f;

		result = inkcolor * inkfreq;
		intensscale = inkfreq ;
	
		if(inkid == 0) {
			result = inkcolor;
		}
		result = mix(watercolor, result, intensityout * intensscale);
		if(flags.x) {//Freeze
			result = mix(result, vec3(0.63,0.9,1), freezeintensityout);
		}
	
		gl_FragColor = vec4(result, 1.0);
	}
	else {
		gl_FragColor = vec4(force.x + 0.5, force.y + 0.5,0,1.0);
	}
}