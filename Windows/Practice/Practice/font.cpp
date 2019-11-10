#include "font.h"

#include "glut.h"

void fontBegin()
{
	glPushMatrix();						//	Œ»İİ’è‚³‚ê‚Ä‚¢‚és—ñî•ñ‚ğ•Û
	glPushAttrib(GL_ALL_ATTRIB_BITS);	//	Œ»İİ’è‚³‚ê‚Ä‚¢‚é•`‰æ‘®«‚ğ•Û
}

void fontEnd()
{
	glPopMatrix();						//	fontBegin‚Å•Û‚µ‚½s—ñî•ñ‚ğ–ß‚·
	glPopAttrib();						//	fontBegin‚Å•Û‚µ‚½•`‰æ‘®«‚ğ–ß‚·
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