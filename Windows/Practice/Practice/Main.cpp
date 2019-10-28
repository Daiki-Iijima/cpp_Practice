#include "glut.h"

//	描画が必要になったら
void display()
{

}

int main(int argc, char *argv[])
{
	glutInit(&argc,argv);


	glutInitWindowPosition(640,0);			//	Window位置(やらなくてもいい)
	glutInitWindowSize(640,640);			//	Window大きさ(やらなくてもいい)

	glutCreateWindow("多々タイトル");		//	Windowのタイトル設定
	glutDisplayFunc(display);				//	描画が必要になったら呼ばれるコールバックの設定
	glutMainLoop();							//	処理をglutに委託する

	return 0;
}