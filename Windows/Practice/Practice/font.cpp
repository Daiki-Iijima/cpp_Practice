#include "font.h"

#include "glut.h"

void fontBegin()
{
	glPushMatrix();						//	���ݐݒ肳��Ă���s�����ێ�
	glPushAttrib(GL_ALL_ATTRIB_BITS);	//	���ݐݒ肳��Ă���`�摮����ێ�
}

void fontEnd()
{
	glPopMatrix();						//	fontBegin�ŕێ������s�����߂�
	glPopAttrib();						//	fontBegin�ŕێ������`�摮����߂�
}

void fontSetPosition(float _x, float _y)
{

}

void fontSetSize(float _size)
{

}

void fontSetColor(unsigned char _red, unsigned char _green, unsigned char _blue)
{

}

void fontDraw(const char _format, ...)
{

}