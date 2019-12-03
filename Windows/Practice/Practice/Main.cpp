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


	//	======= ������̕`�� ======
	fontBegin();
	{
		fontHeight(FONT_DEFAULT_HEIGHT);
		fontWeight(fontGetWeightMax());
		fontPosition(fontGetWeight() * 2, fontGetWeight() * 2);

		for (int i = 0; i < 128; i++)
		{
			if (keys[i])
				fontDraw("%c\n", i);	//	������Ă���L�[��`�悷��
		}
	}
	fontEnd();

	glutSwapBuffers();	//	�_�u���o�b�t�@�̕\�Ɨ���؂�ւ���(�X���b�v����)
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

	keys[key] = true;	//	�L�[�������ꂽ

	if ((key >= '0') && (key <= '3'))
	{
		int channel = key - '0';
		audioStop(channel);
		audioGain(channel, AUDIO_DEFAULT_GAIN * .1f);
		audioPlay(channel);
	}

	switch (key)
	{
	case 0x1b:			//	Escape�L�[�ŏI��
		exit(0);
		break;
		//case 'p':
		//	audioStop();
		//	audioWaveform(AUDIO_WAVEFORM_NOISE_LONG);
		//	audioFreq(audioIndexToFreq(14));	//	1789772.5f = �t�@�~�R����CPU�̎��g��
		//	//audioDecay(.9f);
		//	//audioSweep(.9f, 1789772.5f / 4068);
		//	audioPlay();
		//break;
	}
}

void keybordUp(unsigned char key, int x, int y)
{
	printf("keybordUp: %d,(%#x)\n", key, key);

	keys[key] = false;	//	�L�[�������ꂽ

	if ((key >= '0') && (key <= '3'))
	{
		int channel = key - '0';
		audioStop(channel);
	}
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