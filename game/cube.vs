#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 i_vertPos;
layout(location = 1) in vec2 i_UV;

// Values that stay constant for the whole mesh.
uniform mat4 u_mvp;

out vec2 v_UV;

void main()
{	

	// Output position of the vertex, in clip space : MVP * position
	gl_Position = u_mvp * vec4(i_vertPos,1);
	v_UV = i_UV;
}

