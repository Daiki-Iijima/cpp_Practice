#include "Paddle.h"

#include "glut.h"

void Paddle::draw()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	{
		GLfloat range[2];				//	0:�ŏ� 1:�ő�
		glGetFloatv(					//	���C���̑����͈̔͂��擾
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

//	�{�[���Ƃ̓����蔻����Ƃ�
bool Paddle::intersectBall(Ball &const _ball)
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