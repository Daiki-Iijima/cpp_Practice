#pragma once

#include "glm/glm.hpp"

using namespace glm;

struct Rect {
	vec2 m_position;
	vec2 m_size;

	Rect(vec2 const& _position, vec2 const& _size);
	void draw();
};