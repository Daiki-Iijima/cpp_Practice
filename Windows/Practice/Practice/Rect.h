#pragma once

#include "glm/glm.hpp"

using namespace glm;

struct Rect {
	vec2 m_position;
	vec2 m_size;

	bool isDead;	//	Breakout��p�̎��S����t���O

	Rect();
	Rect(vec2 const& _size);							//	���W��0�ł����ꍇ
	Rect(vec2 const& _position, vec2 const& _size);
	void draw();	//	�`��
	bool intersect(vec2 const& _point);	//	�n���ꂽ�ʒu�Ƃ̓����蔻��
	bool intersect(Rect const& _rect);	//	�n���ꂽRect�^�Ƃ̓����蔻��
	
};