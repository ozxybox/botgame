#include "basebillboardent.h"
#include "utils.h"
#include "camera.h"
#include <renderer.h>
#include <primitivedraw.h>


void CBaseBillboardEnt::SetTileset(const char* path, unsigned int tilew, unsigned int tileh)
{
	m_tileset = Textures().LoadTileset(path, tilew, tileh);
	m_shader  = Shaders().GetProgram("tileset.vs", "tileset.fs");
	m_tileX   = 0;
	m_tileY   = 0;
}

void CBaseBillboardEnt::Render()
{
	CTransform t = *this;
	t.SetAbsAngles(Camera().GetLookAtCamAng());
	glm::vec3 offset = t.GetLocalScale() * 0.5f;
	offset.y = 0;
	t.SetLocalOrigin(t.GetLocalOrigin() + offset);

	Renderer().BindShader(m_shader);
	Renderer().BindTile(m_tileset, m_tileX, m_tileY);
	PrimitiveDraw().DrawQuad(t, m_shader);


}
