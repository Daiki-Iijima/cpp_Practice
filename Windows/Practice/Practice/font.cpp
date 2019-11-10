#include "font.h"

#include "glut.h"

void fontBegin()
{
	glPushMatrix();						//	現在設定されている行列情報を保持
	glPushAttrib(GL_ALL_ATTRIB_BITS);	//	現在設定されている描画属性を保持
}

void fontEnd()
{
	glPopMatrix();						//	fontBeginで保持した行列情報を戻す
	glPopAttrib();						//	fontBeginで保持した描画属性を戻す
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