#include "cell.h"
#include <textures.h>
#include <renderer.h>
#include <primitivedraw.h>

DECLARE_ENT(CCell, cell);

texture_t Grass()
{
	static texture_t s_grass = Textures().LoadTexture("grass.png");
	return s_grass;
}

CCell::CCell(const char* classname) : CBaseEntity(classname)
{

}

void CCell::Render()
{
	CTransform t(*this);
	t.SetLocalScale(10, 10, 1);
	t.SetLocalAngles(GetLocalAngles() + glm::vec3{ acos(-1) * 0.5f, 0, 0.0f });
	Renderer().BindTexture(Grass());
	PrimitiveDraw().DrawQuad(t);

}
