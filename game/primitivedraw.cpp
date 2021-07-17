#include "primitivedraw.h"
#include <glad/include/glad/glad.h>
#include <renderer.h>

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


static vertex_t s_quadVbo[] =
{
	{{ 1, 0, 0}, {0,1}},
	{{ 0, 0, 0}, {1,1}},
	{{ 0, 1, 0}, {1,0}},
	{{ 1, 1, 0}, {0,0}},
};

static unsigned short s_quadIbo[] =
{
	0, 1, 2,
	2, 3, 0
};

DEFINE_SINGLETON(PrimitiveDraw);

CPrimitiveDraw::CPrimitiveDraw()
{
	m_cubeShader = Shaders().GetProgram("cube.vs", "cube.fs");

	glGenVertexArrays(1, &m_cubeVAO);
	glBindVertexArray(m_cubeVAO);

	glGenBuffers(1, &m_cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_cubeVbo), s_cubeVbo, GL_STATIC_DRAW);

	glGenBuffers(1, &m_cubeIBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_cubeIBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_cubeIbo), s_cubeIbo, GL_STATIC_DRAW);


	glGenVertexArrays(1, &m_quadVAO);
	glBindVertexArray(m_quadVAO);

	glGenBuffers(1, &m_quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_quadVbo), s_quadVbo, GL_STATIC_DRAW);

	glGenBuffers(1, &m_quadIBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadIBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_quadIbo), s_quadIbo, GL_STATIC_DRAW);
}

CPrimitiveDraw::~CPrimitiveDraw()
{

	glDeleteBuffers(1, &m_cubeVBO);
	glDeleteBuffers(1, &m_cubeIBO);
	glDeleteVertexArrays(1, &m_cubeVAO);


	glDeleteBuffers(1, &m_quadVBO);
	glDeleteBuffers(1, &m_quadIBO);
	glDeleteVertexArrays(1, &m_quadVAO);
}

void CPrimitiveDraw::DrawCube(CTransform& transform)
{
	DrawCube(transform, m_cubeShader);
}

void CPrimitiveDraw::DrawCube(CTransform& transform, shader_t shader)
{
	Renderer().SetMatrix(MatrixMode::MODEL, transform.Matrix());
	Renderer().BindShader(shader);

	Renderer().DrawObject(m_cubeVAO, m_cubeVBO, m_cubeIBO, sizeof(s_cubeIbo) / sizeof(short));

}



void CPrimitiveDraw::DrawQuad(CTransform& transform, shader_t shader)
{
	Renderer().SetMatrix(MatrixMode::MODEL, transform.Matrix());
	Renderer().BindShader(shader);

	Renderer().DrawObject(m_quadVAO, m_quadVBO, m_quadIBO, sizeof(s_quadIbo) / sizeof(short));
}
void CPrimitiveDraw::DrawQuad(CTransform& transform)
{
	DrawQuad(transform, m_cubeShader);
}