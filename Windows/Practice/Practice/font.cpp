#include <stdio.h>
#include <stdarg.h>

#include "font.h"

#include "glm/glm.hpp"
#include "glut.h"

using namespace glm;

static vec2 position;
static float size = FONT_DEFAULT_SIZE;

static unsigned char color[3];

static float weight = 1;

void fontBegin()
{
	glPushMatrix();						//	���ݐݒ肳��Ă���s�����ێ�
	glPushAttrib(GL_ALL_ATTRIB_BITS);	//	���ݐݒ肳��Ă���`�摮����ێ�

	glMatrixMode(GL_PROJECTION);		//	�ˉe���[�h��ύX����
	glLoadIdentity();					//	�O��̎ˉe�s�񂪎c��Ȃ��悤�ɍs��̏�����

	GLint viewport[4];					//	�r���[�|�[�g�̏���ۑ�����
	glGetIntegerv(						//	�r���[�|�[�g�̏����擾����
		GL_VIEWPORT,
		viewport
	);

	gluOrtho2D(							//	2������Ԃ��`(Ortho:���ˉe)
		0, viewport[2],					//	left,right
		viewport[3], 0					//	bottom,top
	);

	glMatrixMode(GL_MODELVIEW);		//	���f���r���[�s�񃂁[�h�ɐ؂�ւ�
	glLoadIdentity();				//	�O��̎ˉe�s�񂪎c��Ȃ��悤�ɍs��̏�����


	//	�e��ݒ�𖳌��ɂ���
	glDisable(GL_DEPTH_TEST);		//	�[�x�e�X�g
	glDisable(GL_LIGHTING);			//	���C�e�B���O
	glDisable(GL_TEXTURE_2D);		//	2D�e�N�X�`��
}

void fontEnd()
{
	glPopMatrix();						//	fontBegin�ŕێ������s�����߂�
	glPopAttrib();						//	fontBegin�ŕێ������`�摮����߂�
}

void fontSetPosition(float _x, float _y)
{
	position = vec2(_x, _y);
}

void fontSetSize(float _size)
{
	size = _size;
}

float fontGetSize()
{
	return size;
}

float fontGetWeightMin()
{
	GLfloat weight[2];
	glGetFloatv(
		GL_LINE_WIDTH_RANGE,
		weight
	);

	return weight[0];
}

float fontGetWeightMax()
{
	GLfloat weight[2];
	glGetFloatv(
		GL_LINE_WIDTH_RANGE,
		weight
	);

	return weight[1];
}

void fontSetWeight(float _weight)
{
	weight = _weight;
}

float fontGetWeight()
{
	return weight;
}

void fontSetColor(unsigned char _red, unsigned char _green, unsigned char _blue)
{
	color[0] = _red;
	color[1] = _green;
	color[2] = _blue;
}

void fontDraw(const char *_format, ...)
{
	va_list argList;
	va_start(argList, _format);

	char str[256];

	vsprintf_s(
		str,
		_format,
		argList
	);

	glLineWidth(weight);		//	���̑�����ݒ�

	va_end(argList);

	glColor3ub(color[0], color[1], color[2]);	//	�t�H���g�̐F��ύX

	char* p = str;

	glPushMatrix();						//	�ʒu�A�傫��(�㉺���])��ύX�������̂ōs���ۑ� 
	{
		glTranslatef(position.x, position.y + size, 0);		//	�ʒu��ύX
		float s = size / FONT_DEFAULT_SIZE;
		glScalef(s, -s, s);									//	�傫����ύX

		for (; *p != '\0' && *p != '\n'; p++)				//	*p��\0(�󕶎�)��\n(���s�R�[�h)�̏ꍇ���[�v���I���
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);		//	������`��

	}
	glPopMatrix();						//	���ɖ߂�

	if (*p == '\n')	//	���s�R�[�h�ŏI�����Ă����ꍇ
	{
		glTranslatef(0, size + weight * 2, 0);	//	�����`��ʒu�����ɂ��炷
		fontDraw(++p);				//	�ēx�`�施�߂��o��(�ċA����)
	}
}



