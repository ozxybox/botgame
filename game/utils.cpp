#include "utils.h"
#include <glm/glm/geometric.hpp>

globals_t g_globals;

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


void Angles(glm::vec3 dir, glm::vec3& angle)
{
	angle.x = -asin(dir.y);
	angle.y = atan2(dir.x, dir.z);
	angle.z = 0;
}