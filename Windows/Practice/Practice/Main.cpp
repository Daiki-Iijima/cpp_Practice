#include <stdio.h>
#include "glm/glm.hpp"	//	*glut.h����ɒ�`����K�v������
#include "glut.h"

#include "font.h"
#include "Rect.h"

using namespace glm;

ivec2 windowSize = { 800,600 };	//	�E�B���h�E�̃T�C�Y���`

bool keys[256];		//	�ǂ̃L�[��������Ă��邩��ێ�����

Rect rect1 = Rect(vec2(100, 100), vec2(100, 200));
Rect rect2 = Rect(vec2(windowSize.x / 2, windowSize.y / 2), vec2(200, 100));

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


	if (rect1.intersect(rect2))
		glColor3ub(0xff, 0x00, 0x00);
	else
		glColor3ub(0x00, 0x00, 0xff);
	//	��`�̕`��
	rect1.draw();

	glColor3ub(0x00, 0xff, 0x00);

	rect2.draw();

	//	======= ������̕`��(font.cpp) ======
	fontBegin();
	fontSetColor(0, 0xff, 0);
	fontSetPosition(0, windowSize.y - fontGetSize() * 2);
	fontSetSize(FONT_DEFAULT_SIZE / 2);
	fontEnd();
	//	=====================================

	glutSwapBuffers();	//	�_�u���o�b�t�@�̕\�Ɨ���؂�ւ���(�X���b�v����)
	//glFlush();			//	�V���O���o�b�t�@�̏ꍇ
}

void idle(void)
{
	//	�`�悵�Ă�����̂𓮂����ꍇ�A�����ŏ�������
	float f = 5;
	if (keys['w'])	rect1.m_position.y -= f;
	if (keys['s'])	rect1.m_position.y += f;
	if (keys['a'])	rect1.m_position.x -= f;
	if (keys['d'])	rect1.m_position.x += f;

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
	if (key == 0x1b)	//	Escape�L�[�ŏI��
		exit(0);

	//printf("keybord: %d,(%#x)\n", key, key);

	keys[key] = true;	//	�L�[�������ꂽ
}

void keybordUp(unsigned char key, int x, int y)
{
	//printf("keybordUp: %d,(%#x)\n", key, key);

	keys[key] = false;	//	�L�[�������ꂽ
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

	glutMainLoop();							//	������glut�Ɉϑ�����(�R�[���o�b�N�n�͂��̃��\�b�h���O�ɏ���)


	return 0;
}