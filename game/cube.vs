#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 i_vertPos;

// Values that stay constant for the whole mesh.
uniform mat4 u_mvp;

void main()
{	

	// Output position of the vertex, in clip space : MVP * position
	gl_Position = u_mvp * vec4(i_vertPos,1);

}

