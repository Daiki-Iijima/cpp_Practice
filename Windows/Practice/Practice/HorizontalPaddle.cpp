#include "HorizontalPaddle.h"

#include "glut.h"

void HorizontalPaddle::draw()
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
			glVertex2fv((GLfloat*)&(m_position + vec2(m_width, 0)));
		}
		glEnd();
	}
	glPopAttrib();
}

//	ボールとの当たり判定をとる
bool HorizontalPaddle::intersectBall(Ball &const _ball)
{
	if (
		(m_position.y >= _ball.m_position.y && m_position.y < _ball.m_lastposition.y) ||
		(m_position.y < _ball.m_position.y && m_position.y >= _ball.m_lastposition.y)
		)
	{
		if (
			(m_position.x < _ball.m_lastposition.x &&
				m_position.x + m_width >= _ball.m_position.x)
			)
			return true;
	}

	return false;
}