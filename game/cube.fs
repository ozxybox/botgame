#version 330 core

in vec2 v_UV;

uniform sampler2D Texture;

// Ouput data
layout (location = 0) out vec3 o_color;

void main(){

	// Output color = color specified in the vertex shader, 
	// interpolated between all 3 surrounding vertices
	vec4 pix = texture(Texture, v_UV);//vec3(1.0,0.0,0.0);
	o_color = pix.rgb;
} 