#include <stdio.h>
#include <time.h>
#include "glm/glm.hpp"	//	*glut.h����ɒ�`����K�v������
#include "glut.h"

#include "font.h"
#include "tex.h"
#include "audio.h"

#include "Rect.h"

using namespace glm;

ivec2 windowSize = { 800,600 };	//	�E�B���h�E�̃T�C�Y���`

bool keys[256];					//	�ǂ̃L�[��������Ă��邩��ێ�����


//	�`�悪�K�v�ɂȂ�����
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);	//	�F�������Z�b�g����

	//	�A�X�y�N�g�䂪�ς��Ȃ��悤�ɂ���
	glMatrixMode(GL_PROJECTION);	//	�ˉe���[�h��ύX����
	glLoadIdentity();				//	�O��̎ˉe�s�񂪎c��Ȃ��悤�ɍs��̏�����
	gluOrtho2D(						//	2������Ԃ��`(Ortho:���ˉe)
		0, windowSize.x,				//	left,right
		windowSize.y, 0				//	bottom,top
	);

	glMatrixMode(GL_MODELVIEW);		//	���f���r���[�s�񃂁[�h�ɐ؂�ւ�
	glLoadIdentity();				//	�O��̎ˉe�s�񂪎c��Ȃ��悤�ɍs��̏�����

	//	=== �e�N�X�`���̕`�� ===
	//glEnable(GL_TEXTURE_2D);		//	�e�N�X�`����L����
	//glEnable(GL_BLEND);			//	���ߏ�����L����

	//glBlendFunc(					//	�A���t�@�u�����h�ݒ�
	//	GL_SRC_ALPHA,				//
	//	GL_ONE_MINUS_SRC_ALPHA		//	
	//);

	//	==================


	//	======= ������̕`�� ======
	fontBegin();
	{
		fontHeight(FONT_DEFAULT_HEIGHT / 2);
		fontWeight(fontGetWeightMax() / 2);
		fontPosition(fontGetWeight() * 2, fontGetWeight() * 2);

		//fontDraw("abc\nkkk"); fontDraw("\n"); fontDraw("def\n");

	}
	fontEnd();
	//	=====================================

	//	=== �^�����������A���S���Y�� ===
	const int xorBit = 6;									//	���r�b�g�ڂ�XOR���Z���邩
	static int shiftReg = 1 << 14;							//	�V�t�g���W�X�^�[���`
	int result = (shiftReg ^ (shiftReg >> xorBit)) & 1;		//	0bit�ڂ�1bit�ڂ�XOR���Z
	shiftReg >>= 1;											//	���W�X�^�̒l���E��1�V�t�g������
	shiftReg |= result << 14;								//	or���Z���Čv�Z���ʂ�14�r�b�g�ڂɑ��

	for (int i = 14; i >= 0; i--)
	{
		switch (i)											//	�r�b�g�ԍ��ŐF����
		{
		case 0:glColor3ub(0x00, 0xff, 0x00); break;
		case xorBit:glColor3ub(0xff, 0x00, 0x00); break;
		case 14:glColor3ub(0xff, 0xff, 0x00); break;
		default:glColor3ub(0xff, 0xff, 0xff); break;
		}

		fontDraw("%d", (shiftReg >> i) & 1);

		if (i % 4 == 0)										//	4������؂�ŃJ���}����
		{
			glColor3ub(0xff, 0xff, 0xff);
			fontDraw(",");
		}
	}
	glColor3ub(0xff, 0xff, 0x00);
	fontDraw("\n%d", result);

	glColor3ub(0xff, 0xff, 0xff);
	fontDraw("\n%#x(%d)", shiftReg, shiftReg);
	//	===============================

	glutSwapBuffers();	//	�_�u���o�b�t�@�̕\�Ɨ���؂�ւ���(�X���b�v����)
	//getchar();		//	�L�[�{�[�h���͑҂����
}

void idle(void)
{
	//	=== �e��A�b�v�f�[�g���� ===
	audioUpdate();
	//	============================

	glutPostRedisplay();	//	�ĕ`�施��
}

//	�E�B���h�E�T�C�Y���ύX���ꂽ�Ƃ��ɌĂ�
void reshape(int width, int height)
{
	printf("reshape:width%d height:%d\n", width, height);

	glViewport(			//	�r���[�|�[�g���X�V(�X�V���Ȃ��Ǝw��T�C�Y���傫���Ȃ������ɕ\���ł����ɐ؂�Ă��܂�)
		0, 0,			//	���W(x,y)
		width, height	//	�T�C�Y(w,h)
	);

	windowSize = ivec2(width, height);	//	���T�C�Y���ꂽ�l�ŃT�C�Y�萔������������
}

void keybord(unsigned char key, int x, int y)
{
	printf("keybord: %d,(%#x)\n", key, key);

	switch (key)
	{
	case 0x1b:			//	Escape�L�[�ŏI��
		exit(0);
		break;
	case 'p':
		audioStop();
		audioWaveform(AUDIO_WAVEFORM_NOISE_LONG);
		audioFreq(audioIndexToFreq(14));	//	1789772.5f = �t�@�~�R����CPU�̎��g��
		//audioDecay(.9f);
		//audioSweep(.9f, 1789772.5f / 4068);
		audioPlay();
	break;
	}

	/*if ((key >= '0') && (key <= '4'))
	{
		audioStop();
		audioWaveform(key - '0');
		audioFreq(440);
		audioSweep(.99, 440 / 2);
		audioPlay();
	}*/

	if ((key >= '0') && (key <= '9'))
	{
		audioStop();
		int k = key - '0';
		audioWaveform(AUDIO_WAVEFORM_PULSE_12_5);
		audioFreq(440 * powf(2, (1 + k / 12.f)));
		//audioDecay(.9f);
		audioSweep(.99, 440 / 2);
		audioPlay();
	}


	keys[key] = true;	//	�L�[�������ꂽ
}

void keybordUp(unsigned char key, int x, int y)
{
	printf("keybordUp: %d,(%#x)\n", key, key);

	keys[key] = false;	//	�L�[�������ꂽ
}

void passiveMotion(int _x, int _y)
{
}

int main(int argc, char *argv[])
{
	audioInit();

	glutInit(&argc, argv);

	glutInitDisplayMode(GL_DOUBLE);			//	�_�u���o�b�t�@���g�p����(���Ȃ��ꍇ�V���O���o�b�t�@)

	glutInitWindowPosition(640, 0);			//	Window�ʒu(���Ȃ��Ă�����)
	glutInitWindowSize(windowSize.x, windowSize.y);			//	Window�傫��(���Ȃ��Ă�����)

	glutCreateWindow("���X�^�C�g��");		//	Window�̃^�C�g���ݒ�

	//unsigned char colorKey[] = { 0x00,0xff,0x00 };			//	�����ɂ������F
	//int result = texFromBMP("UnityChan.bmp", colorKey);		//	.bmp�t�@�C�����J��

	glutDisplayFunc(display);				//	�`�悪�K�v�ɂȂ�����Ă΂��R�[���o�b�N�̐ݒ�

	glutIdleFunc(idle);						//	GLUT�̎肪�󂢂����ɌĂ΂��R�[���o�b�N�̐ݒ�

	glutReshapeFunc(reshape);				//	Window�̃T�C�Y���ς������Ă΂��R�[���o�b�N�̐ݒ�

	glutKeyboardFunc(keybord);				//	�L�[�{�[�h�C�x���g���擾
	glutIgnoreKeyRepeat(GL_TRUE);			//	�L�[�{�[�h�̉������ςȂ���Ԃ𖳌��ɂ��邱�Ƃ�True�ɂ���
	glutKeyboardUpFunc(keybordUp);			//	�L�[�{�[�h�������ꂽ�Ƃ��C�x���g

	glutPassiveMotionFunc(passiveMotion);	//	�}�E�X�̈ړ��C�x���g���擾

	glutMainLoop();							//	������glut�Ɉϑ�����(�R�[���o�b�N�n�͂��̃��\�b�h���O�ɏ���)

	return 0;
}