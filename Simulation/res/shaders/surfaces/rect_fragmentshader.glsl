#version 330 core

uniform vec3 color;
uniform float rendertexture;

in vec2 TexCoord;
uniform sampler2D Texture;


void main() {
	vec4 texColor;
	if(rendertexture == 1) {
		texColor = vec4(color, 1.0) * texture(Texture, TexCoord);
		if(texColor.a < 0.1)
			discard;
		}
	else
		texColor = vec4(color, 1.0);
	gl_FragColor = texColor;
}