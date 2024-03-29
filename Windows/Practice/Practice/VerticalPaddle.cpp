#include "VerticalPaddle.h"

#include "glut.h"

void VerticalPaddle::draw()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	{
		GLfloat range[2];				//	0:最小 1:最大
		glGetFloatv(					//	ラインの太さの範囲を取得
			GL_LINE_WIDTH_RANGE,
			range
		);

		glLineWidth(range[1]);

		glBegin(GL_LINES);
		{
			glVertex2f(m_position.x, m_position.y);
			vec2 v = m_position + vec2(0, m_height);
			glVertex2f(v.x, v.y);
		}
		glEnd();
	}
	glPopAttrib();
}

//	ボールとの当たり判定をとる
bool VerticalPaddle::intersectBall(Ball &const _ball)
{
	if (
		(m_position.x < _ball.m_lastposition.x && m_position.x >= _ball.m_position.x) ||
		(m_position.x >= _ball.m_lastposition.x && m_position.x < _ball.m_position.x) 
		)
	{
		if (
			(m_position.y < _ball.m_lastposition.y && 
			 m_position.y + m_height >= _ball.m_position.y)
			)
		return true;
	}

	return false;
}