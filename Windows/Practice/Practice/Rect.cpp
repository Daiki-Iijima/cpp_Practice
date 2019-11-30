#include "Rect.h"

#include "glut.h"

Rect::Rect()
{

}

Rect::Rect(vec2 const& _position, vec2 const& _size)
	:m_position(_position)
	, m_size(_size)
{

}

//	++ 描画 ++
void Rect::draw()
{
	glBegin(GL_QUADS);	//	QUADS : 四角形の描画(ワイヤーフレーム対応)
	{
		glTexCoord2f(0, 0);												//	テクスチャ座標を設定
		glVertex2fv((GLfloat *)&m_position);							//	左上

		glTexCoord2f(0, 1);												//	テクスチャ座標を設定
		glVertex2fv((GLfloat *)&(m_position + vec2(0, m_size.y)));		//	左下

		glTexCoord2f(1, 1);												//	テクスチャ座標を設定
		glVertex2fv((GLfloat *)&(m_position + m_size));					//	右下

		glTexCoord2f(1, 0);												//	テクスチャ座標を設定
		glVertex2fv((GLfloat *)&(m_position + vec2(m_size.x, 0)));		//	右上
	}
	glEnd();
}

//	++ 当たり判定(ポイント) ++
bool Rect::intersect(vec2 const& _point)
{
	if (
		(_point.x >= m_position.x) &&
		(_point.x < m_position.x + m_size.x) &&
		(_point.y >= m_position.y) &&
		(_point.y < m_position.y + m_size.y)
		)
		return true;

	return false;
}

//	++ 当たり判定(四角形) ++
bool Rect::intersect(Rect const& _rect)
{
	if (
		(m_position.x + m_size.x >= _rect.m_position.x) &&
		(m_position.x < _rect.m_position.x + _rect.m_size.x) &&
		(m_position.y + m_size.y >= _rect.m_position.y) &&
		(m_position.y < _rect.m_position.y + _rect.m_size.y)
		)
		return true;

	return false;
}

