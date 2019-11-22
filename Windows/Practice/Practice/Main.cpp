#include <stdio.h>
#include <time.h>
#include "glm/glm.hpp"	//	*glut.h����ɒ�`����K�v������
#include "glut.h"

#include "font.h"
#include "Rect.h"

#include "Ball.h"
#include "Paddle.h"

#include "audio.h"

#define	BALL_MAX 2

using namespace glm;

ivec2 windowSize = { 800,600 };	//	�E�B���h�E�̃T�C�Y���`

bool keys[256];		//	�ǂ̃L�[��������Ă��邩��ێ�����

Ball balls[BALL_MAX];
Paddle paddle;

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

	//	�e�B�[�|�b�g���f���ɑ΂���ϊ�
	glMatrixMode(GL_MODELVIEW);		//	���f���r���[�s�񃂁[�h�ɐ؂�ւ�
	glLoadIdentity();				//	�O��̎ˉe�s�񂪎c��Ȃ��悤�ɍs��̏�����

	glColor3ub(0xff, 0xff, 0xff);
	paddle.draw();

	unsigned char colors[6][3] = {	//	�{�[���Ŏg�p����\��̐F�p���b�g���쐬
		{0xff,0x00,0x00},
		{0x00,0xff,0x00},
		{0x00,0x00,0xff},
		{0xff,0xff,0x00},
		{0x00,0xff,0xff},
		{0xff,0x00,0xff},
	};

	for (int i = 0; i < BALL_MAX; i++)
	{
		glColor3ub(					//	�F��ݒ�
			colors[i % 6][0],
			colors[i % 6][1],
			colors[i % 6][2]
		);

		balls[i].draw();			//	�{�[����`��
	}
	//	======= ������̕`��(font.cpp) ======

	fontBegin();
	fontSetColor(0, 0xff, 0);
	fontSetSize(FONT_DEFAULT_SIZE);

	float lineHeight = fontGetSize() * 1.5;
	float y = fontGetWeight() / 2;

	fontSetPosition(0, y);
	fontSetWeight(fontGetWeightMax());
	fontDraw("AA\nBBB\nCCCC\nEEEEE\n");

	fontEnd();
	//	=====================================

	glutSwapBuffers();	//	�_�u���o�b�t�@�̕\�Ɨ���؂�ւ���(�X���b�v����)
	//glFlush();			//	�V���O���o�b�t�@�̏ꍇ
}

void idle(void)
{
	for (int i = 0; i < BALL_MAX; i++)
	{
		balls[i].update();

		if (paddle.intersectBall(balls[i]))
		{
			balls[i].m_position = balls[i].m_lastposition;
			balls[i].m_speed.x *= -1;
		}

		if (balls[i].m_position.x >= windowSize.x)
		{
			balls[i].m_position = balls[i].m_lastposition;
			balls[i].m_speed.x = -fabs(balls[i].m_speed.x);	//	��Βl�ɕϊ����Ă���A�}�C�i�X�ɕϊ�
		}

		if (balls[i].m_position.x < 0)
		{
			balls[i].m_position = balls[i].m_lastposition;
			balls[i].m_speed.x = fabs(balls[i].m_speed.x);	//	��Βl�ɕϊ�
		}


		if (balls[i].m_position.y >= windowSize.y)
		{
			balls[i].m_position = balls[i].m_lastposition;
			balls[i].m_speed.y = -fabs(balls[i].m_speed.y);	//	��Βl�ɕϊ����Ă���A�}�C�i�X�ɕϊ�
		}

		if (balls[i].m_position.y < 0)
		{
			balls[i].m_position = balls[i].m_lastposition;
			balls[i].m_speed.y = fabs(balls[i].m_speed.y);	//	��Βl�ɕϊ����Ă���A�}�C�i�X�ɕϊ�
		}
	}

	audioUpdate();

	glutPostRedisplay();	//	�ĕ`�施��
}

//	�E�B���h�E�T�C�Y���ύX���ꂽ�Ƃ��ɌĂ�
void reshape(int width, int height)
{
	//printf("reshape:width%d height:%d\n", width, height);

	glViewport(			//	�r���[�|�[�g���X�V(�X�V���Ȃ��Ǝw��T�C�Y���傫���Ȃ������ɕ\���ł����ɐ؂�Ă��܂�)
		0, 0,			//	���W(x,y)
		width, height	//	�T�C�Y(w,h)
	);

	windowSize = ivec2(width, height);	//	���T�C�Y���ꂽ�l�ŃT�C�Y�萔������������
}

void keybord(unsigned char key, int x, int y)
{
	printf("keybord: %d,(%#x)\n", key, key);

	//audioLength(500);			//	�Đ����Ԃ�ݒ�
	//audioDecay(.9f);			//	��������������(�t�F�[�h�A�E�g)
	//audioPitchTarget(4.0f);	//	��������s�b�`�̐ݒ�
	//audioSweep(1.05f);		//	���̃s�b�`��ω�������
	//audioFreq(440 * 2);			//	���K�̐ݒ�

	switch (key)
	{
	case 0x1b:			//	Escape�L�[�ŏI��
		exit(0);
		break;
	case 'p':
		audioPlay();		//	���𗬂�
		break;
	case's':
		audioStop();		//	�����~�߂�
		break;
	}

	//if ((key >= '1') && (key <= '5'))
	//{
	//	audioWaveform(key - '1');	//	1�`5�L�[�Ŕg�`��؂�ւ���
	//	audioStop();				//	�����~�߂�	�Đ����ɔg�`�؂�ւ��͂ł��Ȃ�
	//	audioPlay();				//	���𗬂�
	//}

	if ((key >= '0' && (key <= '9')))
	{
		audioStop();
		int k = key - '0';
		audioDecay(.9f);
		audioWaveform(AUDIO_WAVEFORM_PULSE_50);		//	�g�`��؂�ւ���
		audioFreq(440 * powf(2, (1 + k / 12.f)));	//	12�t���b�g
		audioPlay();
	}

	keys[key] = true;	//	�L�[�������ꂽ
}

void keybordUp(unsigned char key, int x, int y)
{
	//printf("keybordUp: %d,(%#x)\n", key, key);

	keys[key] = false;	//	�L�[�������ꂽ
}

void passiveMotion(int _x, int _y)
{
	paddle.m_position = vec2(_x, _y);
}

int main(int argc, char *argv[])
{
	if (audioInit() != 0)
		return 1;

	srand(time(NULL));		//	�����_���p�ϐ������݂̎��Ԃŏ�����

	for (int i = 0; i < BALL_MAX; i++)
	{
		balls[i].m_position =					//	�ʒu��ݒ�
			vec2(
				rand() % windowSize.x,			//	x:0~1�̗����ŋ��߂� 
				rand() % windowSize.y			//	y:0~1�̗����ŋ��߂�
			);

		balls[i].m_speed =						//	�{�[���̃X�s�[�h��ݒ�
			normalize(							//	�X�s�[�h�����ɂ��邽�߂ɐ��K��
				vec2(
				(float)rand() / RAND_MAX - .5f,		//	x:0~1�̗����ŋ��߂� 
					(float)rand() / RAND_MAX - .5f		//	y:0~1�̗����ŋ��߂�
				)
			);
	}

	paddle.m_height = 300;

	glutInit(&argc, argv);

	glutInitDisplayMode(GL_DOUBLE);			//	�_�u���o�b�t�@���g�p����(���Ȃ��ꍇ�V���O���o�b�t�@)

	glutInitWindowPosition(640, 0);			//	Window�ʒu(���Ȃ��Ă�����)
	glutInitWindowSize(windowSize.x, windowSize.y);			//	Window�傫��(���Ȃ��Ă�����)

	glutCreateWindow("���X�^�C�g��");		//	Window�̃^�C�g���ݒ�
	glutDisplayFunc(display);				//	�`�悪�K�v�ɂȂ�����Ă΂��R�[���o�b�N�̐ݒ�

	glutIdleFunc(idle);						//	GLUT�̎肪�󂢂����ɌĂ΂��R�[���o�b�N�̐ݒ�

	glutReshapeFunc(reshape);				//	Window�̃T�C�Y���ς������Ă΂��R�[���o�b�N�̐ݒ�

	glutKeyboardFunc(keybord);				//	�L�[�{�[�h�C�x���g���擾
	glutIgnoreKeyRepeat(GL_TRUE);			//	�L�[�{�[�h�̉������ςȂ���Ԃ𖳌��ɂ��邱�Ƃ�True�ɂ���
	glutKeyboardUpFunc(keybordUp);			//	�L�[�{�[�h�������ꂽ�Ƃ��C�x���g

	glutPassiveMotionFunc(passiveMotion);	//	�}�E�X�̈ړ��C�x���g���擾
	//glutMotionFunc(motion);					//	�}�E�X���N���b�N���ꂽ��Ԃ̈ړ��C�x���g���擾

	glutMainLoop();							//	������glut�Ɉϑ�����(�R�[���o�b�N�n�͂��̃��\�b�h���O�ɏ���)


	return 0;
}