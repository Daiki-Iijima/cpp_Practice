#pragma once

#define FONT_DEFAULT_HEIGHT (100.f)				//	�t�H���g����(OpenGL�̃X�g���[�N�t�H���g�̃T�C�Y��100)

enum {

	FONT_FONT_ROMAN,
	FONT_FONT_MONO_ROMAN,
	FONT_FONT_MAX,
};

//	�킩��₷���悤��font�𖍂ɂ���

void fontBegin();	//	�t�H���g�`��J�n
void fontEnd();		//	�t�H���g�`��I��

void fontFont(int _font);			//	�t�H���g��ݒ�

void fontPosition(float _x, float _y);		//	�t�H���g�ʒu
void fontHeight(float _size);				//	�t�H���g�̍�����ݒ�

float fontGetWidth(int _character);							//	�t�H���g�̕����擾
float fontGetHeight();							//	�t�H���g�̍������擾

float fontGetWeightMin();						//	�ݒ�ł���t�H���g�̈�ԍׂ�����
float fontGetWeightMax();						//	�ݒ�ł���t�H���g�̈�ԑ�������

void fontWeight(float _weight);					//	�t�H���g�̑�����ύX
float fontGetWeight();							//	�t�H���g�̑������擾


//void fontSetColor(unsigned char _red, unsigned char _green, unsigned char _blue);	//�t�H���g�J���[
void fontDraw(const char *_format, ...);		//	�t�H���g��`�悷��