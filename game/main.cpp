#define SDL_MAIN_HANDLED
#include <SDL2/include/SDL.h>
#include <stdio.h>
#include <glad/include/glad/glad.h>
#include "gameinfo.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <time.h>
#include "transform.h"
#include "log.h"
#include <shadermanager.h>
#include <renderer.h>
#include <primitivedraw.h>
#include "baseentity.h"


class CTestEnt : public CBaseEntity
{
public:
	CTestEnt(const char* classname) : CBaseEntity(classname) {}

	virtual void Render()
	{
		PrimitiveDraw().DrawCube(*this);
	}
	virtual void Update()
	{
		//SetAbsAngles(0, clock() / 1000.0f, clock() / 2000.0f);
	}

};
DECLARE_ENT(CTestEnt, test_ent);

class CPlayer : public CBaseEntity
{
public:
	CPlayer(const char* classname) : CBaseEntity(classname) {}

	virtual void Render()
	{
	}
	virtual void Update()
	{
		glm::vec3 ang = GetAbsAngles();
		glm::vec3 pos = GetAbsOrigin();

		glm::mat4x4 m = glm::identity<glm::mat4x4>();
		m = glm::rotate(m, ang.x, glm::vec3(1, 0, 0));
		m = glm::rotate(m, ang.y, glm::vec3(0, 1, 0));
		m = glm::rotate(m, ang.z, glm::vec3(0, 0, 1));
		m = glm::translate(m, pos * -1.0f);

		Renderer().SetMatrix(MatrixMode::VIEW, m);
	}

};
DECLARE_ENT(CPlayer, player);

int main()
{
	

	CShaderManager sm;
	CRenderer rm;
	CPrimitiveDraw pd;

	Log::Msg("ready\n");
	

	Renderer().SetPerspective(glm::radians(45.0f));

	CBaseEntity* ent = EntManager().CreateEntity("test_ent");
	CBaseEntity* player = EntManager().CreateEntity("player");
	player->SetAbsOrigin(0, 1, 4);

	bool run = true;
	SDL_Event e;
	while (run)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				run = false;
			}
		}
		player->SetAbsAngles(0,sin(clock()/1000.0f)*3.14f*0.25, 0);

		EntManager().Update();

		

		Renderer().ClearFrame();
		EntManager().Render();
		Renderer().PushFrame();

	}

	

	return 0;
}