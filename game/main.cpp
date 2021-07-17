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
#include "shaders.h"
#include <renderer.h>
#include <primitivedraw.h>
#include "baseentity.h"
#include <lodepng.h>
#include <textures.h>
#include <imgui/imgui.h>
#include <imguibackend.h>
#include "utils.h"
#include <camera.h>
#include "basebillboardent.h"
#include <world.h>
#include "physics.h"

enum class RobotAction
{
	TURN_LEFT,
	TURN_RIGHT,
	FORWARD,
	END,
};

float lerp(float a, float b, float percent)
{
	return a + (b - a) * percent;
}
class CRobot : public CBaseBillboardEnt
{
public:
	CRobot(const char* classname) : CBaseBillboardEnt(classname)
	{
		int i = 0;
		m_acts[i++] = RobotAction::FORWARD;
		m_acts[i++] = RobotAction::TURN_LEFT;
		m_acts[i++] = RobotAction::TURN_LEFT;
		m_acts[i++] = RobotAction::FORWARD;
		m_acts[i++] = RobotAction::TURN_RIGHT;
		m_acts[i++] = RobotAction::END;

		/*
		m_acts[i++] = RobotAction::TURN_LEFT;
		m_acts[i++] = RobotAction::FORWARD;
		m_acts[i++] = RobotAction::TURN_RIGHT;
		m_acts[i++] = RobotAction::FORWARD;
		m_acts[i++] = RobotAction::TURN_RIGHT;
		m_acts[i++] = RobotAction::TURN_RIGHT;
		m_acts[i++] = RobotAction::FORWARD;
		m_acts[i++] = RobotAction::END;
		*/
		m_pos = 0;
		m_actionStartTime = SDL_GetTicks() / 1000.0f;
		m_startYaw = 0;
		m_startPos = GetLocalOrigin();
		//SetLocalScale(0.5, 1, 0.5);
		//SetLocalAngles(0, 3.14, 0);
		SetTileset("player.png", 32, 32);
	}
	int m_pos; 
	RobotAction m_acts[32];
	float m_actionStartTime;
	float m_startYaw;
	glm::vec3 m_startPos;

	virtual void Update()
	{

		{
			float yaw = GetLocalAngles().y;
			float x = sin(yaw);
			float y = -cos(yaw);
		
			if (x > 0.5f)
			{
				if(y > 0.5f)
					SetTilePos(1, 0);
				else if (y < -0.5f)
					SetTilePos(3, 0);
				else
					SetTilePos(2, 0);
			}
			else if (x < -0.5f)
			{
				if (y > 0.5f)
					SetTilePos(7, 0);
				else if (y < -0.5f)
					SetTilePos(5, 0);
				else
					SetTilePos(6, 0);
			}
			else
			{
				if (y < -0.5f)
					SetTilePos(4, 0);
				else if(y > 0.5f)
					SetTilePos(0, 0);
			}

		}


		if (ImGui::Begin("Robot"))
		{
			ImGui::BeginTable("program", 1);
			int i = 0;
			for(RobotAction a = m_acts[0]; a != RobotAction::END; a = m_acts[++i])
			{ 
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				if (i == m_pos)
					ImGui::Text("> ");
				else
					ImGui::Text("* ");
				ImGui::SameLine();
				if (a == RobotAction::TURN_LEFT)
					ImGui::Text("TURN_LEFT");
				else if (a == RobotAction::TURN_RIGHT)
					ImGui::Text("TURN_RIGHT");
				else if (a == RobotAction::FORWARD)
					ImGui::Text("FORWARD");

			}
			ImGui::EndTable();
		}
		ImGui::End();

		float t = SDL_GetTicks() / 1000.0f;
		if (t >= m_actionStartTime + 1.0f)
		{
			m_pos++;
			m_startYaw = GetLocalAngles().y;
			m_actionStartTime = t;
			m_startPos = GetLocalOrigin();
		}

		RobotAction a = m_acts[m_pos];
		if (a == RobotAction::TURN_LEFT)
		{
			SetLocalAngles(0, lerp(m_startYaw, m_startYaw + glm::radians(90.0f), t - m_actionStartTime), 0);
		}
		else if (a == RobotAction::TURN_RIGHT)
		{
			SetLocalAngles(0, lerp(m_startYaw, m_startYaw - glm::radians(90.0f), t - m_actionStartTime), 0);
		}
		else if (a == RobotAction::FORWARD)
		{
			glm::vec3 forward;
			Directions(GetLocalAngles(), &forward, 0, 0);
			SetLocalOrigin(m_startPos + forward * (t - m_actionStartTime));
		}
		else if (a == RobotAction::END)
		{
			m_pos = 0;
		}

	}

};
DECLARE_ENT(CRobot, robot);



class CBarrel : public CBaseBillboardEnt
{
public:
	CBarrel(const char* classname) : CBaseBillboardEnt(classname)
	{
		SetTileset("barrel.png", 32, 32);
	}

};
DECLARE_ENT(CBarrel, barrel);



class CBox : public CBaseEntity , public CPhysicsObject
{
public:
	CBox(const char* classname) : CBaseEntity(classname), CPhysicsObject(*(CTransform*)this)
	{
		m_lastUpdate = g_globals.curTime;
	}
	virtual void Spawn()
	{
		SetVelocity({ 18,0,8 });
		SetResistance({ 3, 0, 3 });
		SetAcceleration({ -4,0,0 });
	}
	double m_lastUpdate;
	virtual void Update()
	{

		if (g_globals.curTime > m_lastUpdate + 0.01f)
		{
			Simulate(g_globals.curTime - m_lastUpdate);
			m_lastUpdate = g_globals.curTime;
		}
	}

	virtual void Render()
	{
		CTransform t = *this;
		t.SetLocalOrigin(t.GetLocalOrigin() + glm::vec3{0, 1, 0});
		Renderer().BindTexture(Textures().ErrorTexture());
		PrimitiveDraw().DrawCube(t);
	}

};
DECLARE_ENT(CBox, box);


int main()
{

	CShaders sm;
	CRenderer rm;
	CPrimitiveDraw pd;
	CTextures tx;
	CCamera cam;

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	
	ImGuiInit();

	g_globals.dt = 0.1f;
	g_globals.curTime = SDL_GetPerformanceCounter() / (double)SDL_GetPerformanceFrequency();;
	Log::Msg("ready\n");
	

	//SDL_SetRelativeMouseMode(SDL_TRUE);
	EntManager().CreateEntity("world");

	CBaseEntity* ent = EntManager().CreateEntity("robot");
	ent->SetAbsOrigin(0, 0, 0);

	ent = EntManager().CreateEntity("barrel");
	ent->SetAbsOrigin(4, 0, 0);

	ent = EntManager().CreateEntity("box");
	ent->SetAbsOrigin(-4, 0, 0);
	World().AddTarget(ent, 5);

	CBaseEntity* player = EntManager().CreateEntity("player");
	player->SetAbsOrigin(0, 0, -4);
	Camera().Setup(player, {0,10,14});
	World().AddTarget(player, 5);

	bool run = true;
	SDL_Event e;
	double lastTime = 0;
	while (run)
	{

		// Lock at 60
		uint32_t wt = 1000.0f / 60.0f;//// std::max(1000.0f / 60.0f - (s_deltaTime * 1000.0f - 1000.0f / 60.0f), 1.0);
		//printf("%d\n", wt);
		SDL_Delay(wt);

		// Higher than 1 ms resolution clock
		double curTime = SDL_GetPerformanceCounter() / (double)SDL_GetPerformanceFrequency();
		g_globals.dt = curTime - lastTime;
		g_globals.curTime = curTime;
		lastTime = curTime;


		// Dear ImGui new frame
		int w, h;
		int display_w, display_h;

		Renderer().GetWindowSize(w, h);
		io.DisplaySize = ImVec2((float)w, (float)h);
		io.DisplayFramebufferScale = ImVec2(1, 1);
		io.DeltaTime = g_globals.dt == 0 ? 0.01f : g_globals.dt;
		

		g_globals.mousedx = 0;
		g_globals.mousedy = 0;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				run = false;
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
				case SDLK_a: g_globals.input |= IN_LEFT;     break;
				case SDLK_d: g_globals.input |= IN_RIGHT;    break;
				case SDLK_w: g_globals.input |= IN_FORWARD;  break;
				case SDLK_s: g_globals.input |= IN_BACKWARD; break;

				case SDLK_ESCAPE: run = false;    break;
				}
				break;
			case SDL_KEYUP:
				switch (e.key.keysym.sym)
				{
				case SDLK_a: g_globals.input &= ~IN_LEFT;     break;
				case SDLK_d: g_globals.input &= ~IN_RIGHT;    break;
				case SDLK_w: g_globals.input &= ~IN_FORWARD;  break;
				case SDLK_s: g_globals.input &= ~IN_BACKWARD; break;
				}
				break;
			case SDL_MOUSEMOTION:
				g_globals.mousedx = e.motion.xrel;
				g_globals.mousedy = e.motion.yrel;
				break;
			}
		}

		int mx, my;
		Uint32 mouse_buttons = SDL_GetMouseState(&mx, &my);
		io.MouseDown[0] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;  // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
		io.MouseDown[1] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
		io.MouseDown[2] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
		io.MousePos = ImVec2((float)mx, (float)my);

		ImGui::NewFrame();


		if (ImGui::Begin("Player"))
		{
			glm::vec3 pos = player->GetAbsOrigin();
			ImGui::InputFloat3("pos", &pos.x);
		}
		ImGui::End();
		// Reset everything 

		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glEnable(GL_STENCIL_TEST); 
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_ALPHA_TEST);

		glEnable(GL_BLEND);

		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
		glBlendEquation(GL_FUNC_ADD);
		

		Camera().Update();
		

		ImGui::ShowDemoWindow();


		EntManager().Update();

		

		Renderer().ClearFrame();
		EntManager().Render();
		//glViewport(0, 0, 800, 800);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		Renderer().PushFrame();
	}

	

	return 0;
}