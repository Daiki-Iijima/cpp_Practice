#pragma once

//	�킩��₷���悤��font�𖍂ɂ���

void fontBegin();	//	�t�H���g�`��J�n
void fontEnd();		//	�t�H���g�`��I��

void fontSetPosition(float _x, float _y);	//	�t�H���g�ʒu
void fontSetSize(float _size);				//	�t�H���g�T�C�Y
void fontSetColor(unsigned char _red, unsigned char _green, unsigned char _blue);	//�t�H���g�J���[
void fontDraw(const char _format, ...);	//	�t�H���g��`�悷��