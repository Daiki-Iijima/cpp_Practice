#include "glut.h"

//	�`�悪�K�v�ɂȂ�����
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);	//	�F�������Z�b�g����

	glRotatef(
		1,		//	��]����p�x
		0,0,1	//	��]��
	);

	glutWireTeapot(1);	//	�e�B�[�|�b�g��`�悷��

	glutSwapBuffers();	//	�_�u���o�b�t�@�̕\�Ɨ���؂�ւ���(�X���b�v����)
	//glFlush();			//	�V���O���o�b�t�@�̏ꍇ
}

void timer(int value)
{
	glutPostRedisplay();	//	�ĕ`�施��

	//	�ċA�I�ɌĂяo��
	glutTimerFunc(			//	�w�莞�Ԍ�ɋN������^�C�}�[
		1000/60,			//	��ms��Ƀ^�C�}�[���N�����邩(1000ms = 1s)
		timer,				//	�N�����郁�\�b�h	
		0					//	�n���l
	);
}

int main(int argc, char *argv[])
{
	glutInit(&argc,argv);

	glutInitDisplayMode(GL_DOUBLE);			//	�_�u���o�b�t�@���g�p����(���Ȃ��ꍇ�V���O���o�b�t�@)

	glutInitWindowPosition(640,0);			//	Window�ʒu(���Ȃ��Ă�����)
	glutInitWindowSize(640,640);			//	Window�傫��(���Ȃ��Ă�����)

	glutCreateWindow("���X�^�C�g��");		//	Window�̃^�C�g���ݒ�
	glutDisplayFunc(display);				//	�`�悪�K�v�ɂȂ�����Ă΂��R�[���o�b�N�̐ݒ�

	glutTimerFunc(							//	�w�莞�Ԍ�ɋN������^�C�}�[
		0,		//	��ms��Ƀ^�C�}�[���N�����邩(1000ms = 1s)
		timer,	//	�N�����郁�\�b�h	
		0		//	�n���l
	);
	
	glutMainLoop();							//	������glut�Ɉϑ�����(�R�[���o�b�N�n�͂��̃��\�b�h���O�ɏ���)

	
	return 0;
}