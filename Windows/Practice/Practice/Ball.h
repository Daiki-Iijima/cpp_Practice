#pragma once

#include "glm/glm.hpp"

using namespace glm;

struct Ball
{
	vec2 m_lastposition;	//	1�t���[���O�̈ʒu
	vec2 m_position;		//	���݂̃t���[���̈ʒu
	vec2 m_speed;			//	���x

	void update();			//	�v�Z
	void draw();			//	�`��
};