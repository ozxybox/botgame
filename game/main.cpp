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
#include <lodepng.h>
#include <textures.h>
#include <imgui/imgui.h>
#include <imguibackend.h>


enum
{
	IN_NONE     = 0x0,
	IN_FORWARD  = 0x1,
	IN_BACKWARD = 0x2,
	IN_LEFT     = 0x4,
	IN_RIGHT    = 0x8,
};

static unsigned int s_in = 0;
static double s_deltaTime = 0;
static float s_mousedx = 0;
static float s_mousedy = 0;


void Directions(glm::vec3 angles, glm::vec3* forward, glm::vec3* right, glm::vec3* up)
{
	const float pitch = angles.x;
	const float yaw = angles.y;
	const float roll = -angles.z;

	glm::vec3 _forward;
	_forward.x = sin(yaw) * cos(pitch);
	_forward.y = -sin(pitch);
	_forward.z = cos(yaw) * cos(pitch);
	_forward = glm::normalize(_forward);

	if (forward)
		*forward = _forward;


	glm::vec3 _right;
	_right.x = cos(roll) * cos(yaw);
	_right.y = sin(roll);
	_right.z = -cos(roll) * sin(yaw);
	_right = glm::normalize(_right);


	if (right)
		*right = _right;

	if (up)
		*up = -glm::normalize(glm::cross(_right, _forward));
	/*
		if (up)
		{
			glm::vec3 _up;
			_up.x = cos(-(roll + yaw)) * sin(pitch);
			_up.y = cos(-roll) * cos(pitch);
			_up.z = sin(-roll) * cos(yaw) * ((sin(pitch) + 1)/2);
			_up = glm::normalize(_up);

			*up = _up;
		}
		*/
}

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
class CRobot : public CBaseEntity
{
public:
	CRobot(const char* classname) : CBaseEntity(classname) 
	{
		int i = 0;
		m_acts[i++] = RobotAction::TURN_LEFT;
		m_acts[i++] = RobotAction::FORWARD;
		m_acts[i++] = RobotAction::TURN_RIGHT;
		m_acts[i++] = RobotAction::FORWARD;
		m_acts[i++] = RobotAction::TURN_RIGHT;
		m_acts[i++] = RobotAction::TURN_RIGHT;
		m_acts[i++] = RobotAction::FORWARD;
		m_acts[i++] = RobotAction::END;
		m_pos = 0;
		m_actionStartTime = SDL_GetTicks() / 1000.0f;
		m_startYaw = 0;
		m_startPos = GetLocalOrigin();
		SetLocalScale(0.5, 1, 0.5);
	}
	int m_pos; 
	RobotAction m_acts[32];
	float m_actionStartTime;
	float m_startYaw;
	glm::vec3 m_startPos;
	virtual void Render()
	{
		PrimitiveDraw().DrawCube(*this);
	}
	virtual void Update()
	{
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

class CPlayer : public CBaseEntity
{
public:
	CPlayer(const char* classname) : CBaseEntity(classname) {}

	virtual void Render()
	{
		PrimitiveDraw().DrawCube(*this);

	}
	virtual void Update()
	{
		glm::vec3 ang = GetAbsAngles();
		glm::vec3 pos = GetAbsOrigin();
//#define FPS_MODE 1
#ifdef FPS_MODE
		glm::vec3 angleDelta = glm::vec3(s_mousedy, -s_mousedx, 0);
		angleDelta *= 2 / 1000.0f;
		ang += angleDelta;
		SetAbsAngles(ang);
#endif

		glm::vec3 forward, right;
		Directions({ 0,ang.y,0 }, &forward, &right, nullptr);

		glm::vec3 delta = {0,0,0};
		delta -= right * (s_in & IN_RIGHT ? 1.0f : 0.0f);
		delta += right * (s_in & IN_LEFT ? 1.0f : 0.0f);
		delta += forward * (s_in & IN_FORWARD ? 1.0f : 0.0f);
		delta -= forward * (s_in & IN_BACKWARD ? 1.0f : 0.0f);
		delta *= s_deltaTime * 4;

		pos += delta;
		SetAbsOrigin(pos);

	}

};
DECLARE_ENT(CPlayer, player);


class CGround : public CBaseEntity
{
public:
	CGround(const char* classname) : CBaseEntity(classname)
	{
		SetLocalScale(10, 0.01f, 10);
	}

	virtual void Render()
	{
		PrimitiveDraw().DrawCube(*this);

	}
	virtual void Update()
	{
	}

};
DECLARE_ENT(CGround, ground);

int main()
{
	

	CShaderManager sm;
	CRenderer rm;
	CPrimitiveDraw pd;
	CTextures tx;

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	
	ImGuiInit();


	Log::Msg("ready\n");
	
	auto img = Textures().LoadTexture("test.png");

	//SDL_SetRelativeMouseMode(SDL_TRUE);

	CBaseEntity* ent = EntManager().CreateEntity("robot");
	ent->SetAbsOrigin(0, 1, 0);

	CBaseEntity* player = EntManager().CreateEntity("player");
	player->SetAbsOrigin(0, 1, -4);

	EntManager().CreateEntity("ground");

	bool run = true;
	SDL_Event e;
	double lastTime = 0;
	while (run)
	{

		// Lock at 60
		uint32_t wt = 1000.0f / 60.0f;//// std::max(1000.0f / 60.0f - (s_deltaTime * 1000.0f - 1000.0f / 60.0f), 1.0);
		//printf("%d\n", wt);
		//SDL_Delay(wt);

		// Higher than 1 ms resolution clock
		double curTime = SDL_GetPerformanceCounter() / (double)SDL_GetPerformanceFrequency();
		s_deltaTime = (curTime - lastTime);
		lastTime = curTime;

		

		// Dear ImGui new frame
		int w, h;
		int display_w, display_h;

		Renderer().GetWindowSize(w, h);
		io.DisplaySize = ImVec2((float)w, (float)h);
		io.DisplayFramebufferScale = ImVec2(1, 1);
		io.DeltaTime = s_deltaTime == 0 ? 0.01f : s_deltaTime;
		

		s_mousedx = 0;
		s_mousedy = 0;
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
				case SDLK_a: s_in |= IN_LEFT;     break;
				case SDLK_d: s_in |= IN_RIGHT;    break;
				case SDLK_w: s_in |= IN_FORWARD;  break;
				case SDLK_s: s_in |= IN_BACKWARD; break;

				case SDLK_ESCAPE: run = false;    break;
				}
				break;
			case SDL_KEYUP:
				switch (e.key.keysym.sym)
				{
				case SDLK_a: s_in &= ~IN_LEFT;     break;
				case SDLK_d: s_in &= ~IN_RIGHT;    break;
				case SDLK_w: s_in &= ~IN_FORWARD;  break;
				case SDLK_s: s_in &= ~IN_BACKWARD; break;
				}
				break;
			case SDL_MOUSEMOTION:
				s_mousedx = e.motion.xrel;
				s_mousedy = e.motion.yrel;
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

		if (ImGui::Begin("Time"))
		{
			ImGui::InputDouble("curTime", &curTime);
			ImGui::InputDouble("s_deltaTime", &s_deltaTime);
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
		Renderer().SetPerspective(glm::radians(45.0f));
		glm::vec3 pos = player->GetAbsOrigin();
		glm::mat4x4 m = glm::lookAt(pos + glm::vec3{ 0.0f,16.0f,-14.0f }, pos, { 0,1,0 });
		Renderer().SetMatrix(MatrixMode::VIEW, m);
		

		ImGui::ShowDemoWindow();


		EntManager().Update();

		

		Renderer().ClearFrame();
		Renderer().BindTexture(img);
		EntManager().Render();
		//glViewport(0, 0, 800, 800);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		Renderer().PushFrame();
	}

	

	return 0;
}