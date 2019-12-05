#pragma once

//	- �킩��₷���悤��font�𖍂ɂ��� -

#define FONT_DEFAULT_HEIGHT (119.05f)					//	�t�H���g����(OpenGL�̃��t�@�����X�ɂ��)
#define FONT_DEFAULT_LINE_SPACE (33.33)					//	�t�H���g�̍s��

enum {												//	�t�H���g
	FONT_FONT_ROMAN,
	FONT_FONT_MONO_ROMAN,
	FONT_FONT_MAX,
};

void fontBegin();									//	�t�H���g�`��J�n
void fontEnd();										//	�t�H���g�`��I��

void fontFont(int _font);							//	�t�H���g��ݒ�

void fontScreenSize(float _width, float _height);	//	��ʃT�C�Y��ݒ�

void fontPosition(float _x, float _y);				//	�t�H���g�ʒu
void fontHeight(float _size);						//	�t�H���g�̍�����ݒ�

float fontGetWidth(int _character);					//	�t�H���g�̕����擾
float fontGetLength(const unsigned char* _string);	//	������̕����擾
float fontGetHeight();								//	�t�H���g�̍������擾

float fontGetLineHeight();							//	�s�̍������擾

float fontGetWeightMin();							//	�ݒ�ł���t�H���g�̈�ԍׂ�����
float fontGetWeightMax();							//	�ݒ�ł���t�H���g�̈�ԑ�������

void fontWeight(float _weight);						//	�t�H���g�̑�����ύX
float fontGetWeight();								//	�t�H���g�̑������擾

void fontDraw(const char *_format, ...);			//	�t�H���g��`�悷��