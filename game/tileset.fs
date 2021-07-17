#version 330 core

in vec2 v_UV;

uniform sampler2D Texture;
uniform vec4 u_subrect;

// Ouput data
layout (location = 0) out vec4 o_color;

void main(){

	// Output color = color specified in the vertex shader, 
	// interpolated between all 3 surrounding vertices
	vec4 pix = texture(Texture, v_UV * u_subrect.zw + u_subrect.xy);//vec3(1.0,0.0,0.0);
	o_color = pix.rgba;
} 