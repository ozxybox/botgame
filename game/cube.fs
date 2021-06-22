#version 330 core


// Ouput data
out vec3 o_color;

void main(){

	// Output color = color specified in the vertex shader, 
	// interpolated between all 3 surrounding vertices
	o_color = vec3(1.0,0.0,0.0);

} 