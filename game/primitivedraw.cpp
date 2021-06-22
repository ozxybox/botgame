#include "primitivedraw.h"
#include <glad/include/glad/glad.h>

static glm::vec3 s_cubeVbo[] =
{
	{-1,-1,-1},
	{ 1,-1,-1},
	{ 1, 1,-1},
	{ 1, 1, 1},
	{-1, 1, 1},
	{-1,-1, 1},
	{-1, 1,-1},
	{ 1,-1, 1},
};

static unsigned int s_cubeIbo[] =
{
	// Back face
	2,1,0,
	0,6,2,

	// Front face
	3,4,5,
	5,7,3,

	// Top face
	4,3,2,
	2,6,4,

	// Bottom face
	0,1,7,
	7,5,0,

	// Left face
	5,6,0,
	5,4,6,

	// Right face
	1,2,3,
	3,7,1,
};

DEFINE_SINGLETON(PrimitiveDraw);

CPrimitiveDraw::CPrimitiveDraw()
{
	s_singleton = this;

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
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_cubeVBO);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cubeIBO);
	glDrawElements(GL_TRIANGLES, sizeof(s_cubeIbo) / (sizeof(int)), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
}
