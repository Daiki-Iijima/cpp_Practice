#pragma once

#include "Ball.h"
#include "glm/glm.hpp"

using namespace glm;

struct HorizontalPaddle
{
	float m_width;
	vec2 m_position;

	void draw();
	bool intersectBall(Ball &const _ball);		//	�{�[���Ƃ̓����蔻����Ƃ�
};