#pragma once

#include "basesystem.h"
#include "shadermanager.h"

#include <glm/mat4x4.hpp>

enum class MatrixMode
{
	MODEL,
	VIEW,
	PROJECTION
};

DECLARE_SINGLETON_ACCESSOR(Renderer);
class CRenderer : public IBaseSingleton<CRenderer>
{
	DECLARE_SINGLETON(Renderer);
public:
	CRenderer();
	~CRenderer();

	void ClearFrame(glm::vec3 color = {1,1,1});
	void PushFrame();


	void SetPerspective(float fov);
	void SetOrtho();

	void SetMatrix(MatrixMode mode, glm::mat4x4 mat);

	void BindShader(shader_t shader);

private:
	void* m_window;

	glm::mat4x4 m_projection;
	glm::mat4x4 m_view;
	glm::mat4x4 m_model;
};

