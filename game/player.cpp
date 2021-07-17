#include "baseentity.h"
#include "textures.h"
#include <shaders.h>
#include <renderer.h>
#include <primitivedraw.h>
#include <utils.h>
#include "player.h"

DECLARE_ENT(CPlayer, player);

CPlayer::CPlayer(const char* classname) : CBaseBillboardEnt(classname)
{
	SetTileset("player.png", 32, 32);
	SetTilePos(0, 0);
	SetLocalScale(1.75f, 1.75f, 1.75f);
}

void CPlayer::Update()
{
	glm::vec3 ang = GetAbsAngles();
	glm::vec3 pos = GetAbsOrigin();

	glm::vec3 forward, right;
	Directions({ 0,ang.y,0 }, &forward, &right, nullptr);

	glm::vec3 delta = { 0,0,0 };
	delta += right * (g_globals.input & IN_RIGHT ? 1.0f : 0.0f);
	delta -= right * (g_globals.input & IN_LEFT ? 1.0f : 0.0f);
	delta -= forward * (g_globals.input & IN_FORWARD ? 1.0f : 0.0f);
	delta += forward * (g_globals.input & IN_BACKWARD ? 1.0f : 0.0f);
	if (delta.x != 0 || delta.z != 0)
	{

		delta = glm::normalize(delta) * g_globals.dt * 4.0f;

		if(delta.x > 0 && delta.z == 0)
			SetTilePos(2, m_tileY);
		else if (delta.x < 0 && delta.z == 0)
			SetTilePos(6, m_tileY);
		else if (delta.x == 0 && delta.z < 0)
			SetTilePos(0, m_tileY);
		else if (delta.x == 0 && delta.z > 0)
			SetTilePos(4, m_tileY);
		else if (delta.x > 0 && delta.z < 0)
			SetTilePos(1, m_tileY);
		else if (delta.x < 0 && delta.z < 0)
			SetTilePos(7, m_tileY);
		else if (delta.x < 0 && delta.z > 0)
			SetTilePos(5, m_tileY);
		else if (delta.x > 0 && delta.z > 0)
			SetTilePos(3, m_tileY);
	}

	pos += delta;
	SetAbsOrigin(pos);

}

