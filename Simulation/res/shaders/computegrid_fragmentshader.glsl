#version 430 core

uniform vec3 color;
uniform vec3 freezecolor;

in float ofreq;
in vec4 oflags;
in float oink;

void main() {
	bvec4 flags = bvec4(oflags.x, oflags.y, oflags.z, oflags.w);
	//vec3 floatfreq = vec3(ofreq, ofreq, ofreq);
	vec3 result;
	if(ofreq == 0) 
		result = vec3(0.0,0.0,ofreq);
	
	
	else
		result = vec3(0.0,0.0,ofreq);
	//vec3 result = color;
	//vec3 result = color - floatfreq;
	
	if(oink > 0)
		result = vec3(oink/100, oink/100, 0.0f);
	if(flags.x)
		result *= freezecolor;

	gl_FragColor = vec4(result, 1.0);
	//gl_FragColor = vec4(0.0,0.0,0.0,0.0);
	//gl_FragColor = vec4(1.0,0.0,0.0,1.0);
}