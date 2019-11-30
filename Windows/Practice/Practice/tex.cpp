#include <stdio.h>
#include <Windows.h>

#include "glut.h"

#include "tex.h"

int texFromBMP(const char* _fileName)
{
	FILE *pFile;
	fopen_s(&pFile, _fileName, "rb");					//	�t�@�C�����J��

	if (pFile == nullptr)
	{
		printf("%s open failed!\n", _fileName);
		return 1;
	}
	else
	{
		printf("%s file Opend!\n", _fileName);
	}

	BITMAPFILEHEADER bf;								//	�w�b�_�[�̃f�[�^���i�[����\���̂��`
	fread(&bf, sizeof(BITMAPFILEHEADER), 1, pFile);		//	�ǂݍ���
	//printf("bfSize : %d\n", bf);

	BITMAPINFOHEADER bi;								//	�w�b�_�[�̏����i�[����\���̂��`
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, pFile);		//	�ǂݍ���
	//printf("biWidth:%d biHeight:%d\n", bi.biWidth, bi.biHeight);
	//printf("biBitCout:%d\n", bi.biBitCount);

	typedef struct
	{
		unsigned char r, g, b, a;
	}RGBA;	//	32�r�b�g�Ȃ̂�

	RGBA *pixels = (RGBA*)malloc(sizeof RGBA * bi.biWidth * bi.biHeight);	//	�s�N�Z���̃��������m��
	fread(pixels, sizeof RGBA, bi.biWidth * bi.biHeight, pFile);			//	�s�N�Z���f�[�^�̓ǂݍ���


	glTexImage2D(					//	Vram�ւ̓]��������Ɛݒ�
		GL_TEXTURE_2D,				//	�^�[�Q�b�g
		0,							//	mipmap�̃��x��
		GL_RGBA,					//	�e�N�X�`���̃t�H�[�}�b�g
		bi.biWidth, bi.biHeight,	//	���ƍ���
		0,							//	�{�[�_�[
		GL_RGBA,						//	�s�N�Z���f�[�^�̃t�H�[�}�b�g
		GL_UNSIGNED_BYTE,			//	�s�N�Z���ЂƂ��̌^(Char�^��1�o�C�g�Ȃ̂Ńo�C�g)
		pixels						//	�쐬�����s�N�Z���f�[�^	
	);

	glTexParameteri(				//	�e�N�X�`���̃p�����[�^�̐ݒ�
		GL_TEXTURE_2D,				//	�^�[�Q�b�g
		GL_TEXTURE_MAG_FILTER,		//	�e�N�X�`���̊g�厞
		GL_NEAREST					//	��Ԑݒ�(NEAREST : ��Ԃ����Ȃ�)
	);

	glTexParameteri(				//	�e�N�X�`���̃p�����[�^�̐ݒ�
		GL_TEXTURE_2D,				//	�^�[�Q�b�g
		GL_TEXTURE_MIN_FILTER,		//	�e�N�X�`���̏k����
		GL_NEAREST					//	��Ԑݒ�(NEAREST : ��Ԃ����Ȃ�)
	);

	free(pixels);										//	�m�ۂ��Ă����s�N�Z���̃����������
	fclose(pFile);										//	�t�@�C�������

	return 0;
}