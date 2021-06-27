#include "primitivedraw.h"
#include <glad/include/glad/glad.h>
#include <renderer.h>

struct vertex_t
{
	glm::vec3 pos;
	glm::vec2 uv;
};

static vertex_t s_cubeVbo[] =
{
	// Top face
	{{ 1, 1,-1}, {1,0}},
	{{-1, 1,-1}, {0,0}},
	{{-1, 1, 1}, {0,1}},
	{{ 1, 1, 1}, {1,1}},

	// Bottom face
	{{-1,-1,-1}, {1,0}},
	{{ 1,-1,-1}, {0,0}},
	{{ 1,-1, 1}, {0,1}},
	{{-1,-1, 1}, {1,1}},

	// Front face
	{{ 1, 1, 1}, {1,0}},
	{{-1, 1, 1}, {0,0}},
	{{-1,-1, 1}, {0,1}},
	{{ 1,-1, 1}, {1,1}},

	// Back face
	{{-1, 1,-1}, {1,0}},
	{{ 1, 1,-1}, {0,0}},
	{{ 1,-1,-1}, {0,1}},
	{{-1,-1,-1}, {1,1}},

	// Left face
	{{-1, 1, 1}, {1,0}},
	{{-1, 1,-1}, {0,0}},
	{{-1,-1,-1}, {0,1}},
	{{-1,-1, 1}, {1,1}},


	// Right face
	{{ 1, 1,-1}, {1,0}},
	{{ 1, 1, 1}, {0,0}},
	{{ 1,-1, 1}, {0,1}},
	{{ 1,-1,-1}, {1,1}},
};

static unsigned short s_cubeIbo[] =
{
	// Top Face
	0, 1, 2,
	2, 3, 0,

	// Bottom Face
	4, 5, 6,
	6, 7, 4,

	// Front face
	8,   9, 10,
	10, 11,  8,

	// Back face
	12, 13, 14,
	14, 15, 12,
	
	// Left face
	16, 17, 18,
	18, 19, 16,

	// Right face
	20, 21, 22,
	22, 23, 20,
};

DEFINE_SINGLETON(PrimitiveDraw);

CPrimitiveDraw::CPrimitiveDraw()
{
	m_cubeShader = ShaderManager().GetProgram("cube.vs", "cube.fs");

	glGenVertexArrays(1, &m_cubeVAO);
	glBindVertexArray(m_cubeVAO);

	glGenBuffers(1, &m_cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_cubeVbo), s_cubeVbo, GL_STATIC_DRAW);

	glGenBuffers(1, &m_cubeIBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_cubeIBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_cubeIbo), s_cubeIbo, GL_STATIC_DRAW);
}

CPrimitiveDraw::~CPrimitiveDraw()
{

	glDeleteBuffers(1, &m_cubeVBO);
	glDeleteBuffers(1, &m_cubeIBO);
	glDeleteVertexArrays(1, &m_cubeVAO);
}

void CPrimitiveDraw::DrawCube(CTransform& transform)
{
	DrawCube(transform, m_cubeShader);
}

void CPrimitiveDraw::DrawCube(CTransform& transform, shader_t shader)
{
	Renderer().SetMatrix(MatrixMode::MODEL, transform.Matrix());
	Renderer().BindShader(shader);


	glBindVertexArray(m_cubeVAO);

	// 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_cubeVBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(vertex_t),   // stride
		(void*)0            // array buffer offset
	);
	glVertexAttribPointer(
		1,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(vertex_t),   // stride
		(void*)offsetof(vertex_t, uv)            // array buffer offset
	);
	 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cubeIBO);
	glDrawElements(GL_TRIANGLES, sizeof(s_cubeIbo) / (sizeof(short)), GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(0);

}
