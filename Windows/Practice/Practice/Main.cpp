#include <stdio.h>
#include "glut.h"

int windowWidth = 800;		//	�E�B���h�E�̉���
int windowHeight = 600;		//	�E�B���h�E�̍���

bool keys[256];		//	�ǂ̃L�[��������Ă��邩��ێ�����

//	�`�悪�K�v�ɂȂ�����
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);	//	�F�������Z�b�g����


	//	�A�X�y�N�g�䂪�ς��Ȃ��悤�ɂ���
	glMatrixMode(GL_PROJECTION);	//	�ˉe���[�h��ύX����
	glLoadIdentity();				//	�O��̎ˉe�s�񂪎c��Ȃ��悤�ɍs��̏�����
	gluOrtho2D(						//	2������Ԃ��`(Ortho:���ˉe)
		0, windowWidth,				//	left,right
		windowHeight, 0				//	bottom,top
	);

	//	�e�B�[�|�b�g���f���ɑ΂���ϊ�
	glMatrixMode(GL_MODELVIEW);		//	���f���r���[�s�񃂁[�h�ɐ؂�ւ�
	glLoadIdentity();				//	�O��̎ˉe�s�񂪎c��Ȃ��悤�ɍs��̏�����

	//	���̏����ōs������������Ă���̂�1�x�ς������̂�0�x�ɖ߂��Ă��܂����߉���glRotatef�̉�]�p�x��1���n�[�h�R�[�h���Ă���]���Ȃ�
	//	�ϐ��Ɋp�x��~�ς����ĉ�]������
	static float angle;
	//angle += 1;

	//	�L�[�������ꂽ�Ƃ��ɉ�]�ł���悤��
	if (keys['d'])	angle += 1;
	if (keys['a'])	angle -= 1;


	glTranslatef(	//	�e�B�[�|�b�g�̈ʒu��ύX����
		windowWidth / 2, windowHeight / 2, 0	//	x,y,z
	);

	glRotatef(
		angle,		//	��]����p�x
		0, 0, 1	//	��]��
	);

	glScalef(		//	�e�B�[�|�b�g�̑傫����ύX����	
		256, 256, 1	//	x,y,z
	);

	

	glutWireTeapot(1);	//	�e�B�[�|�b�g��`�悷��

	glutSwapBuffers();	//	�_�u���o�b�t�@�̕\�Ɨ���؂�ւ���(�X���b�v����)
	//glFlush();			//	�V���O���o�b�t�@�̏ꍇ
}

void idle(void)
{
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

	windowWidth = width;
	windowHeight = height;
}

void keybord(unsigned char key, int x, int y)
{
	if (key == 0x1b)	//	Escape�L�[�ŏI��
		exit(0);

	printf("keybord: %d,(%#x)\n", key, key);

	keys[key] = true;	//	�L�[�������ꂽ
}

void keybordUp(unsigned char key, int x, int y)
{
	printf("keybordUp: %d,(%#x)\n", key, key);

	keys[key] = false;	//	�L�[�������ꂽ
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GL_DOUBLE);			//	�_�u���o�b�t�@���g�p����(���Ȃ��ꍇ�V���O���o�b�t�@)

	glutInitWindowPosition(640, 0);			//	Window�ʒu(���Ȃ��Ă�����)
	glutInitWindowSize(windowWidth, windowHeight);			//	Window�傫��(���Ȃ��Ă�����)

	glutCreateWindow("���X�^�C�g��");		//	Window�̃^�C�g���ݒ�
	glutDisplayFunc(display);				//	�`�悪�K�v�ɂȂ�����Ă΂��R�[���o�b�N�̐ݒ�

	glutIdleFunc(idle);						//	GLUT�̎肪�󂢂����ɌĂ΂��R�[���o�b�N�̐ݒ�

	glutReshapeFunc(reshape);				//	Window�̃T�C�Y���ς������Ă΂��R�[���o�b�N�̐ݒ�

	glutKeyboardFunc(keybord);				//	�L�[�{�[�h�C�x���g���擾
	glutIgnoreKeyRepeat(GL_TRUE);			//	�L�[�{�[�h�̉������ςȂ���Ԃ𖳌��ɂ��邱�Ƃ�True�ɂ���
	glutKeyboardUpFunc(keybordUp);			//	�L�[�{�[�h�������ꂽ�Ƃ��C�x���g

	glutMainLoop();							//	������glut�Ɉϑ�����(�R�[���o�b�N�n�͂��̃��\�b�h���O�ɏ���)


	return 0;
}