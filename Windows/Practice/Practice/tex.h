#pragma once

int texFromBMP(const char* _filename, unsigned char* _colorKey = nullptr);		//	.bmp��ǂݍ���Ńe�N�X�`���𐶐�����(�J���[�L�[�͐������Ă���n��)
int texFromBMP(const char* _filename,											//	�J���[�L�[�𐶐����āAtexFromBMP�ɃJ���[�L�[��n��
	unsigned char _colorKeyR,
	unsigned char _colorKeyG,
	unsigned char _colorKeyB);		
