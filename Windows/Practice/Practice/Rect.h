#pragma once

#include "glm/glm.hpp"

using namespace glm;

struct Rect {
	vec2 m_position;
	vec2 m_size;

	bool isDead;	//	Breakout専用の死亡判定フラグ

	Rect();
	Rect(vec2 const& _size);							//	座標が0でいい場合
	Rect(vec2 const& _position, vec2 const& _size);
	void draw();	//	描画
	bool intersect(vec2 const& _point);	//	渡された位置との当たり判定
	bool intersect(Rect const& _rect);	//	渡されたRect型との当たり判定
	
};