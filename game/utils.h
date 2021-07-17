#include <glm/glm/ext/vector_float3.hpp>

enum
{
	IN_NONE = 0x0,
	IN_FORWARD = 0x1,
	IN_BACKWARD = 0x2,
	IN_LEFT = 0x4,
	IN_RIGHT = 0x8,
};

struct globals_t
{
	float dt;
	float curTime;

	unsigned int input = 0;
	float mousedx = 0;
	float mousedy = 0;

};
extern globals_t g_globals;

void Directions(glm::vec3 angles, glm::vec3* forward, glm::vec3* right, glm::vec3* up);
void Angles(glm::vec3 dir, glm::vec3& angle);