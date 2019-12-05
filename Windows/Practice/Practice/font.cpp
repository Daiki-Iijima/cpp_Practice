#include <stdio.h>
#include <stdarg.h>

#include "font.h"

#include "glm/glm.hpp"
#include "glut.h"

using namespace glm;

static void* font = GLUT_STROKE_ROMAN;	//	�t�H���g
static vec2 position;
static vec2 origin;
static float height = FONT_DEFAULT_HEIGHT;
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

void fontFont(int _font)
{
	switch (_font)				//	�ԍ��ɂ���ăt�H���g��؂�ւ���
	{
	case FONT_FONT_ROMAN:
		font = font = GLUT_STROKE_ROMAN;
		break;
	case FONT_FONT_MONO_ROMAN:
		font = GLUT_STROKE_MONO_ROMAN;			//	���Ԋu�̃t�H���g
		break;
	}
}

void fontPosition(float _x, float _y)
{
	origin = position = vec2(_x, _y);
}

void fontHeight(float _height)
{
	height = _height;
}

float fontGetWidth(int _character)
{
	return glutStrokeWidth(				//	�����̕����擾
		font,							//	�g�p���Ă���t�H���g
		_character						//	�Ώۂ̕���
	)* height / FONT_DEFAULT_HEIGHT;	//	�����ɂ������Ă���{����������
}

float fontGetLength(const unsigned char* _string)
{
	return glutStrokeLength(	//	������̕����擾
		font,					//	������m�肽���t�H���g
		_string					//	������
	) * height / FONT_DEFAULT_HEIGHT;
}

float fontGetHeight()
{
	return height;
}

float fontGetLineHeight()
{
	return height + FONT_DEFAULT_LINE_SPACE * height / FONT_DEFAULT_HEIGHT;
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

void fontWeight(float _weight)
{
	weight = _weight;
}

float fontGetWeight()
{
	return weight;
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

	glLineWidth(weight);									//	���̑�����ݒ�

	va_end(argList);

	char* p = str;

	//	=== �`�悷�镶�����1�������`�悷�� ===
	{
		for (; *p != '\0' && *p != '\n'; p++)						//	*p��\0(�󕶎�)��\n(���s�R�[�h)�̏ꍇ���[�v���I���
		{
			glPushMatrix();											//	�ʒu�A�傫��(�㉺���])��ύX�������̂ōs���ۑ� 
			{
				glTranslatef(position.x, position.y + height, 0);	//	�ʒu��ύX
				float s = height / FONT_DEFAULT_HEIGHT;
				glScalef(s, -s, s);									//	�傫����ύX

				glutStrokeCharacter(font, *p);						//	������`��
				position.x += fontGetWidth(*p);						//	��ŕ`�����������̍��W�𓮂����Ă���(���ɕ`�悷�镶�����d�Ȃ�Ȃ��悤��)
			}
			glPopMatrix();											//	���ɖ߂�

		}
	}
	//	=======================================

	if (*p == '\n')											//	���s�R�[�h�ŏI�����Ă����ꍇ
	{
		position.x = origin.x;								//	x�̈ʒu�͍ŏ��ɃZ�b�g�����ʒu�ɖ߂�
		position.y += fontGetLineHeight();					//	�����`��ʒu�����ɂ��炷

		fontDraw(++p);										//	�ēx�`�施�߂��o��(�ċA����)
	}
}



