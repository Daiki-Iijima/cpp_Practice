#include "Rect.h"

#include "glut.h"

Rect::Rect(vec2 const& _position, vec2 const& _size) 
:m_position(_position)
,m_size(_size) 
{

}

void Rect::draw()
{
	//	��`��`�悷��
	glRectf(
		m_position.x, m_position.y,							//	����x,y
		m_position.x + m_size.x, m_position.y + m_size.y	//	�E��x,y
	);
}

