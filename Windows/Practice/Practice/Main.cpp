#include "glut.h"

//	�`�悪�K�v�ɂȂ�����
void display()
{

}

int main(int argc, char *argv[])
{
	glutInit(&argc,argv);


	glutInitWindowPosition(640,0);			//	Window�ʒu(���Ȃ��Ă�����)
	glutInitWindowSize(640,640);			//	Window�傫��(���Ȃ��Ă�����)

	glutCreateWindow("���X�^�C�g��");		//	Window�̃^�C�g���ݒ�
	glutDisplayFunc(display);				//	�`�悪�K�v�ɂȂ�����Ă΂��R�[���o�b�N�̐ݒ�
	glutMainLoop();							//	������glut�Ɉϑ�����

	return 0;
}