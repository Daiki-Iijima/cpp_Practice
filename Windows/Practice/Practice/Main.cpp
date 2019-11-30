#include <stdio.h>
#include <time.h>
#include "glm/glm.hpp"	//	*glut.h����ɒ�`����K�v������
#include "glut.h"

#include "font.h"
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

	unsigned char pixels[] =		//	��2�s�N�Z���쐬
	{
		0xff,0x00,0x00, 0x00,0xff,0x00,
		0x00,0x00,0xff, 0x00,0xff,0xff
	};

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	//	1�s�N�Z������e�N�X�`�����쐬�ł���悤�ɐݒ�

	glTexImage2D(						//	Vram�ւ̓]��������Ɛݒ�
		GL_TEXTURE_2D,				//	�^�[�Q�b�g
		0,							//	mipmap�̃��x��
		GL_RGB,						//	�e�N�X�`���̃t�H�[�}�b�g
		2, 2,						//	���ƍ���
		0,							//	�{�[�_�[
		GL_RGB,						//	�s�N�Z���f�[�^�̃t�H�[�}�b�g
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

	glEnable(GL_TEXTURE_2D);		//	�e�N�X�`����L����

	Rect rect = { {100,100},{400,400} };
	rect.draw();

	//	======= ������̕`�� ======
	fontBegin();
	{
		fontSetHeight(FONT_DEFAULT_HEIGHT);
		fontSetPosition(0, 0);
		//fontDraw("");
	}
	fontEnd();
	//	=====================================

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

	switch (key)
	{
	case 0x1b:			//	Escape�L�[�ŏI��
		exit(0);
		break;
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

	glutMainLoop();							//	������glut�Ɉϑ�����(�R�[���o�b�N�n�͂��̃��\�b�h���O�ɏ���)

	return 0;
}