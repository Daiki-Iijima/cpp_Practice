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

//	++ �`�� ++
void Rect::draw()
{
	glBegin(GL_QUADS);	//	QUADS : �l�p�`�̕`��(���C���[�t���[���Ή�)
	{
		glTexCoord2f(0, 0);												//	�e�N�X�`�����W��ݒ�
		glVertex2fv((GLfloat *)&m_position);							//	����

		glTexCoord2f(0, 1);												//	�e�N�X�`�����W��ݒ�
		glVertex2fv((GLfloat *)&(m_position + vec2(0, m_size.y)));		//	����

		glTexCoord2f(1, 1);												//	�e�N�X�`�����W��ݒ�
		glVertex2fv((GLfloat *)&(m_position + m_size));					//	�E��

		glTexCoord2f(1, 0);												//	�e�N�X�`�����W��ݒ�
		glVertex2fv((GLfloat *)&(m_position + vec2(m_size.x, 0)));		//	�E��
	}
	glEnd();
}

//	++ �����蔻��(�|�C���g) ++
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

//	++ �����蔻��(�l�p�`) ++
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

