#include "Ball.h"
#include "glut.h"


void Ball::update()
{
	m_lastposition = m_position;
	m_position += m_speed;
}
void Ball::draw()
{
	glPushMatrix();	//	�s���ۑ�
	{
		glTranslatef(m_position.x, m_position.y,0);	//	�ʒu�X�V
		glScalef(16, 16, 0);						//	�T�C�Y��ύX

		glutSolidSphere(
			1,		//	���a
			16,		//	�f�B�e�B�[��
			16		//	�f�B�e�B�[��
		);
	}
	glPopMatrix();	//	�ۑ����Ă������̂�K�p
}