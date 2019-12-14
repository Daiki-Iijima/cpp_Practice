#include <stdio.h>
#include <Windows.h>

#include "glut.h"

#include "tex.h"

int texFromBMP(const char* _fileName, unsigned char* _colorKey)
{
	FILE *pFile;
	fopen_s(&pFile, _fileName, "rb");					//	�t�@�C�����J��

	if (pFile == nullptr)								//	null�`�F�b�N
	{
		printf("%s open failed!\n", _fileName);
		return 1;
	}
	else
		printf("%s file Opend!\n", _fileName);

	BITMAPFILEHEADER bf;								//	�w�b�_�[�̃f�[�^���i�[����\���̂��`
	fread(&bf, sizeof(BITMAPFILEHEADER), 1, pFile);		//	�ǂݍ���
	//printf("bfSize : %d\n", bf);

	BITMAPINFOHEADER bi;								//	�w�b�_�[�̏����i�[����\���̂��`
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, pFile);		//	�ǂݍ���
	//printf("biWidth:%d biHeight:%d\n", bi.biWidth, bi.biHeight);
	//printf("biBitCout:%d\n", bi.biBitCount);

	//	=== �p�f�B���O�����߂� ===
	int padding = (4 - bi.biWidth * (bi.biBitCount / 8) % 4);	//	4(btmap��4�̔{���̃s�N�Z����������)-�摜�̃s�N�Z���P�ʂ̕� * (�r�b�g�[�x(24) / 8) % 4(�]���)
	padding %= 4;												//	4�̔{���������ꍇ4�ɂȂ��Ă��܂��̂�h��
	//	==========================

	//	===	24�r�b�g(RGB)�摜��32�r�b�g(RGBA)�摜�ɕϊ� ===
	typedef struct
	{
		unsigned char r, g, b, a;
	}RGBA;	//	32�r�b�g�Ȃ̂�

	RGBA *pixels = (RGBA*)malloc(sizeof RGBA * bi.biWidth * bi.biHeight);	//	�s�N�Z���̃��������m��

	//	=== RGB�������R�s�[ ===
	for (int y = 0; y < bi.biHeight; y++)
	{
		for (int x = 0; x < bi.biWidth; x++)
		{
			RGBA *pPixel = &pixels[y*bi.biWidth + x];	//	pixels�̃|�C���^���擾

			fread(pPixel, 3, 1, pFile);					//	�t�@�C���̓ǂݍ���

			//	=== ���ߏ��� ===

			pPixel->a = (_colorKey != nullptr) &&		//	�n���ꂽColorKey����A���ׂĂ̐F��񂪈�v���Ă�����
				(pPixel->r == _colorKey[0]) &&
				(pPixel->g == _colorKey[1]) &&
				(pPixel->b == _colorKey[2])
				? 0x00									//	��v������
				: 0xff;									//	��v���Ă��Ȃ�������
			//	================
		}
		fseek(pFile, padding, SEEK_CUR);				//	�p�f�B���O���ǂݔ�΂�
	}

	// =============================================

	//	=== �F�̕␳ ===
	for (int y = 0; y < bi.biHeight; y++)
		for (int x = 0; x < bi.biWidth; x++)
		{
			RGBA *pPixel = &pixels[y * bi.biWidth + x];	//	����ւ���s�N�Z���̃A�h���X
			unsigned char temp = pPixel->r;				//	R(��)����ۑ�
			pPixel->r = pPixel->b;						//	R(��)��B(��)�����R�s�[
			pPixel->b = temp;							//	B(��)��R(��)�����R�s�[
		}
	//	================

	//	=== �㉺�̔��] ===
	for (int y = 0; y < bi.biHeight / 2; y++)	//	�㔼������
		for (int x = 0; x < bi.biWidth; x++)
		{
			RGBA *pPixel0 = &pixels[y * bi.biWidth + x];						//	��ԏ�̍s -> ����
			RGBA *pPixel1 = &pixels[(bi.biHeight - 1 - y)* bi.biWidth + x];		//	��ԉ��̍s -> ���

			RGBA temp = *pPixel0;												//	�X���b�v����
			*pPixel0 = *pPixel1;
			*pPixel1 = temp;
		}
	//	================

	glTexImage2D(					//	Vram�ւ̓]��������Ɛݒ�
		GL_TEXTURE_2D,				//	�^�[�Q�b�g
		0,							//	mipmap�̃��x��
		GL_RGBA,						//	�e�N�X�`���̃t�H�[�}�b�g	RGB:24�r�b�g RGBA:32�r�b�g
		bi.biWidth, bi.biHeight,	//	���ƍ���
		0,							//	�{�[�_�[
		GL_RGBA,						//	�s�N�Z���f�[�^�̃t�H�[�}�b�g	RGB:24�r�b�g RGBA:32�r�b�g
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

	glTexParameteri(
		GL_TEXTURE_2D,				//	�e�N�X�`���̃p�����[�^�ݒ�
		GL_TEXTURE_WRAP_S,			//	���̃��[�v
		GL_CLAMP					//	���s�[�g�ŎQ�Ƃ��Ȃ��悤�ɃN�����v(�؂�)
	);

	glTexParameteri(
		GL_TEXTURE_2D,				//	�e�N�X�`���̃p�����[�^�ݒ�
		GL_TEXTURE_WRAP_T,			//	�c�̃��[�v
		GL_CLAMP					//	���s�[�g�ŎQ�Ƃ��Ȃ��悤�ɃN�����v(�؂�)
	);


	free(pixels);										//	32�r�b�g�摜�ϊ��p�̃����������

	fclose(pFile);										//	�t�@�C�������

	return 0;
}

int texFromBMP(const char* _filename,
	unsigned char _colorKeyR,
	unsigned char _colorKeyG,
	unsigned char _colorKeyB)
{
	unsigned char colorKey[] = {			//	�J���[�L�[�𐶐�
		_colorKeyR,
		_colorKeyG,
		_colorKeyB
	};

	return texFromBMP(_filename, colorKey);	//	�ϊ��p�̃��\�b�h�ɐ��������J���[�L�[��n���ď���
}
